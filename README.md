# Guns‑And‑Choices

**Guns‑And‑Choices** is a blueprint-based Unreal Engine 4.27 project focused on weapon mechanics and gameplay choices.

---

## 📁 Repository Structure

```
Guns‑And‑Choices/
├── Build/WindowsNoEditor/
│   └── FileOpenOrder        # Build / packaging outputs (excluded in source)
├── Config/                   # Engine & project configuration files
├── Content/                  # Game assets: Blueprints, meshes, textures, etc.
├── Intermediate/             # Intermediate build files (ignored)
├── Saved/                    # Autosaves, logs, etc. (ignored)
├── .gitignore
├── Guns_And_Choices.uproject # Main Unreal project file
└── run.py                    # Helper Python script (e.g. to launch or automate tasks)
```

---

## 🔧 Prerequisites & Setup

To work with this project, you’ll need:

- **Unreal Engine 4.27** installed and configured  
- A compatible version of **Visual Studio** (or your preferred IDE) with C++ support (for plugin/recompile needs)  
- Python (for running `run.py`, if relevant)  
- (Optional) Git Large File Storage (LFS) if heavy assets are tracked  

### Cloning & Initial Setup

1. Clone the repository:

```
git clone https://github.com/mrcool7387/Guns-And-Choices.git
cd Guns-And-Choices
```

2. If you don’t have LFS or large assets are missing, fetch LFS assets:

```
git lfs install
git lfs pull
```

3. Open the project in Unreal Engine:

   - Double‑click `Guns_And_Choices.uproject`  
   - Or open from the Unreal Launcher via **Browse**  

4. Let Unreal do any needed conversions, compile, or rebuild modules.

---

## 🚀 Usage

### Running the Project via `run.py`

There is a `run.py` script included in the root. This is likely intended as a helper—perhaps to launch the editor, package, or automate some workflows. You can inspect it to see supported arguments.

Example:

```
python run.py --help
```

Use it to streamline your development workflow where applicable.

### Working with Blueprints & Assets

- All core logic is implemented in Blueprints under the `Content/` folder.
- Add or modify weapons, characters, UI, or gameplay logic through Blueprints.
- Remember to save and cook assets before packaging for distribution.

---

## 🧪 Development Workflow

1. Create a new branch for features or bug fixes.
2. Commit blueprint or asset changes with descriptive messages.
3. Test in-editor and in packaged builds.
4. Open pull requests / merge back into `main` or `master` when ready.

---

## 📦 Packaging & Distribution

When you're ready to build a deployable version:

1. Use the Unreal Editor’s **Package Project** feature (e.g. target Windows, etc.).
2. Alternatively, adapt or extend `run.py` or custom scripts to automate builds.
3. Ensure all assets are cooked, packaged, and dependencies resolved.

---

## 🤝 Contributing

Contributions are welcome! Please:

- Fork the repository  
- Make your changes or enhancements  
- Submit a pull request  
- Provide descriptions and testing notes  

---

## 📄 License & Attribution

You should add a **LICENSE** file (e.g. MIT, Apache, etc.) to clearly define how others may use your project.

---

## 📝 Notes & TODOs

- Consider cleaning up build artifacts, ignoring unnecessary files.
- Document the purpose and usage of `run.py`.
- Add more in-editor documentation/comments in Blueprints for collaborators.
- Include screenshots or demo videos in the README for better presentation.
