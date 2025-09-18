import os
import subprocess
import time
import requests
import argparse
from google.genai import Client
from dotenv import load_dotenv
from rich.console import Console
from rich.panel import Panel
from rich.progress import Progress, SpinnerColumn, TextColumn
from rich.table import Table

# ----------------- Console -----------------
console = Console()


def style_panel(title, text, color="cyan"):
    return Panel(text, title=title, style=color, expand=True)


# ----------------- Config & Args -----------------
parser = argparse.ArgumentParser(description="Auto Commit & PR Creator")
parser.add_argument("--no-pr", action="store_true", help="Skip Pull Request creation")
parser.add_argument("--no-labels", action="store_true", help="Skip PR labeling")
parser.add_argument(
    "--no-push", action="store_true", help="Do not push branch to remote"
)
parser.add_argument(
    "--silent", action="store_true", help="Minimal output (no spinners)"
)
parser.add_argument("--dry-run", action="store_true", help="Simulate without executing")
args = parser.parse_args()

# Load env
load_dotenv()
api_key = os.getenv("GEMINI_API_KEY")
if not api_key:
    console.print("[bold red]Error:[/bold red] GEMINI_API_KEY must be set in .env file")
    raise ValueError("Missing GEMINI_API_KEY")

base_branch = "main"


# ----------------- Helpers -----------------
def run_verbose(cmd, description="", capture_output=False):
    if args.dry_run:
        console.print(f"[DRY-RUN] {description}: {' '.join(cmd)}")
        return None
    console.print(style_panel("Running", " ".join(cmd) + f"\n{description}", "magenta"))
    if capture_output:
        result = subprocess.run(cmd, text=True, capture_output=True)
        return result
    else:
        subprocess.run(cmd, check=True)
        return None


def gemini_generate(prompt, task_name):
    if args.silent:
        response = client.models.generate_content(
            model="gemini-2.5-flash", contents=prompt
        )
        return response.text.strip()

    console.print(f"[bold blue]{task_name} via Gemini...[/bold blue]")
    with Progress(
        SpinnerColumn(),
        TextColumn("{task.description}"),
        transient=True,
        console=console,
    ) as progress:
        task = progress.add_task(f"{task_name} in progress", start=False)
        progress.start_task(task)
        response = client.models.generate_content(
            model="gemini-2.5-flash", contents=prompt
        )
    return response.text.strip()


# ----------------- Main -----------------
console.print(style_panel("🚀 Auto Commit & PR Script", "Starting...", "cyan"))

# Stage all changes
run_verbose(["git", "add", "."], "Staging all changes")

# Save staged diff
diff_file = "changes.diff"
if not args.dry_run:
    with open(diff_file, "w") as f:
        subprocess.run(["git", "diff", "--staged"], stdout=f, check=True)
    with open(diff_file, "r") as f:
        diff_content = f.read()
else:
    diff_content = "DRY RUN DIFF CONTENT"
console.print(f"[dim]Diff length: {len(diff_content)} characters[/dim]")

# Initialize Gemini client
client = Client(api_key=api_key)

# Detect commit type
type_prompt = f"""
Analyze the following code diff and determine all applicable change types.
Possible labels: feature, fix, docs, refactor, test, chore.
YOU MUST RETURN THE LABELS, AND ONLY THE LABELS, IN A COMMA SEPERATED LIST.
(e.g. 'feature, docs')

Diff:
{diff_content}
"""
labels_text = gemini_generate(type_prompt, "Detecting change types")
labels = [label.strip().lower() for label in labels_text.split(",") if label.strip()]
if not labels:
    labels = ["feature"]

# Generate commit message
commit_prompt = f"Generate a concise commit message for this code diff.\n{diff_content}"
commit_message = gemini_generate(commit_prompt, "Commit message generation")
if not commit_message:
    commit_message = "Auto commit with generated message"

# Generate PR title
pr_title_prompt = (
    f"Generate a short, descriptive Pull Request title for this diff.\n{diff_content}"
)
pr_title = gemini_generate(pr_title_prompt, "PR title generation") or commit_message

# Show summary
summary = Table(title="Commit Summary", header_style="bold cyan")
summary.add_column("Labels", style="yellow")
summary.add_column("Commit Message", style="green")
summary.add_column("PR Title", style="magenta")
summary.add_row(", ".join(labels), commit_message, pr_title)
console.print(summary)

# Create branch
branch_name = f"{labels[0]}/auto-update-{int(time.time())}"
run_verbose(["git", "checkout", "-b", branch_name], f"Creating branch {branch_name}")

# Commit
run_verbose(["git", "commit", "-m", commit_message], "Committing changes")

# Push
if not args.no_push:
    run_verbose(["git", "push", "origin", branch_name], "Pushing branch to remote")
else:
    console.print("[INFO] Skipping push (--no-push)")

# Create PR
if not args.no_pr:
    repo = os.getenv("GITHUB_REPO", "ProjectSaveGH/NoteManager")
    token = os.getenv("GITHUB_TOKEN")
    if not token:
        raise ValueError("GITHUB_TOKEN not set")

    headers = {"Authorization": f"token {token}"}
    url_create_pr = f"https://api.github.com/repos/{repo}/pulls"
    payload = {
        "title": pr_title,
        "head": branch_name,
        "base": base_branch,
        "body": f"Commit Message:\n{commit_message}\n\nGenerated automatically.",
    }
    if not args.dry_run:
        response = requests.post(url_create_pr, json=payload, headers=headers)
        if response.status_code == 201:
            pr_number = response.json()["number"]
            console.print(
                style_panel("✅ PR Created", f"Pull Request #{pr_number}", "green")
            )
            if not args.no_labels:
                url_labels = (
                    f"https://api.github.com/repos/{repo}/issues/{pr_number}/labels"
                )
                payload_labels = {"labels": labels}
                resp_labels = requests.post(
                    url_labels, json=payload_labels, headers=headers
                )
                if resp_labels.status_code == 200:
                    console.print(f"✅ Labels added: {', '.join(labels)}")
                else:
                    console.print(f"❌ Failed to add labels: {resp_labels.text}")
    else:
        console.print("[DRY-RUN] PR creation skipped")
else:
    console.print("[INFO] Skipping PR (--no-pr)")

# Cleanup
if os.path.exists(diff_file):
    os.remove(diff_file)
