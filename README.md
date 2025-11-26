# Anvil Engine

## Requirements
- Visual Studio 2022 (Desktop development with C++) or compatible MSVC toolset
- CMake 3.27+
- Git with submodule support enabled

## Configure & Build
```powershell
git submodule update --init --recursive
cmake -S . -B build -G "Visual Studio 17 2022" -T host=x64 -A win32 `
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --config Debug --target ALL_BUILD -j 12 --
```
* Switch `-A win32` to `-A x64` for 64-bit builds.
* Add `-DBUILD_SHARED_LIBS=OFF` to force static linking if desired.

## Run the Sample Game
The default Synodia sample lives under `apps/synodiaGame`. After a successful build:
```powershell
build\apps\synodiaGame\Debug\synodiaGame.exe
```
DLLs and assets are copied automatically via the post-build step in `apps/synodiaGame/CMakeLists.txt`.

## Using Anvil in Your Own Game
1. Add Anvil as a submodule or external project. Example layout:
   ```powershell
   git submodule add <anvil-repo> external/anvil
   git submodule update --init --recursive
   ```
2. In your CMakeLists.txt, add the engine and link against the `anvil` target:
   ```cmake
   add_subdirectory(external/anvil)

   add_executable(myGame src/main.cpp)
   target_link_libraries(myGame PRIVATE anvil)
   ```
   The public headers are exposed via the target, so no manual include paths are necessary.
3. In game code, include the umbrella header and bootstrap the engine:
   ```cpp
   #include "anvil.h"

   int main() {
       anvil::AppSettings settings;
       settings.loadFromIni("settings.ini");

       auto* app = anvil::Application::Instance();
       app->init(settings);
       app->run();
       return 0;
   }
   ```
4. Ship the runtime dependencies:
   - Copy `anvil.dll` plus SDL/fmt DLLs alongside your executable (see the sample game’s post-build command).
   - Deploy your game assets and `settings.ini`. Reuse `copy_assets()` from `cmake/Deploy.cmake` or craft your own step.

## Coding Guidelines
- Indentation: tabs
- Line endings: Windows CRLF
- Prefer ASCII. Only introduce Unicode where the file already does so.
