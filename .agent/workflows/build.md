---
description: How to correctly build SandboxSX2 after code changes
---

To ensure all C++ and Kotlin changes are correctly applied, especially after Milestone updates:

1. **Sync Project with Gradle Files**: Click the elephant icon or `File > Sync Project with Gradle Files`.
2. **Clean Project**: Go to `Build > Clean Project`. This removes old compiled C++ files.
3. **Generate APK / Run**: Go to `Build > Build Bundle(s) / APK(s) > Build APK(s)` or simply hit the **Run** button to deploy to your device/emulator.

// turbo
4. If building from command line:
   `./gradlew assembleDebug`
