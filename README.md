# MicroController



## About



## TOC
- [About](#about)
- [TOC](#toc)
- [Installation](#installation)
  - [2.2 Via Cmake VSCode Extension](#22-via-cmake-vscode-extension)
- [Usage \[Will be added\]](#usage-will-be-added)
- [Architecture](#architecture)
  - [General](#general)
  - [ScriptController](#scriptcontroller)
  - [MicroController](#microcontroller-1)
  - [Utils](#utils)
  - [App](#app)
  - [Limitation](#limitation)
- [Requirements](#requirements)
  - [General](#general-1)
- [Prerequisites](#prerequisites)
- [TODO](#todo)



## Installation
#### 0. Install Tools
You have to install `cmake`, `gcc`, `gcc-toolchain`, `ninja-build`, `vcpkg`. Look [Prerequisites](#prerequisites).
##### Linux
```bash
sudo apt install build-essential cmake ninja-build
```
##### Windows
For Windows use [`Mingw64`](https://sourceforge.net/mingw-x64) 
```bash
# In MINGW64
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
gcc --version && g++ --version && gdb --version
```

#### 1. Install Dependencies
##### Linux gcc
```bash
vcpkg install --x-install-root=./vendor
```
##### Windows with mingw64 triplet vcpkg
```bash
vcpkg install -triplet x64-mingw-static -x-install-root=./vendor
```
In project root.

### 2. Compilation via CMake
```bash
cmake -S . -B build -G Ninja && cmake --build build --target App
```
Set your toolchain -DCMAKE_TOOLCHAIN_FILE="$PWD/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"
Might required providing compiler by `-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++`.  
Build specific target `--target App`.  
Provide path to toolchain `-DCMAKE_TOOLCHAIN_FILE="$PWD/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"`.

#### 3. Run Program
```bash
./build/MicroController/App/App
```

#### 4. Compile and run
```bash
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ && cmake --build build --target App && ./build/MicroController/App/App
```



## Usage [Will be added]



## Architecture
### General
Project is split into sub libraries connected to ```App```.

### ScriptController
- Fully templated library.
- Main Responsibility is to provide dll Scripts provided from templated interface and supports hot-reloading and at runtime editing. 
- Uses cpp last time write to check if new updates appear. 
- For compilation uses ```gcc``` or other provided compiler with predefined compilation command **for now** will be changed to ```cmake```.
- DLL's are loaded once **Should be used like old ```C``` class with reference to struct**.
- Missing dependency Graph soo You have to recompile it manually.
- Don't have all functionalities yet like ```recompileAll, etc```.
- In future will provide build in scripts.
- DLL communication are ```extern "C"``` for ```GetScript()``` and ```DeleteScript()``` also **functions with reference to struct** like in old ```C```.

### MicroController
- Main Responsibility is controlling devices and information about them setting ```[names, identifiers, drivers, binds, getting IDevice*, etc]```. It's communication layer between **controller** and **pc**.
- ```IDriver``` is interface that provides base operation for specific Drivers.
- ```IDevice``` is interface that provides end user operations on Device.
- ```Device``` is class for Controller that allows more operations then IDevice.
- For now Drivers are compiled into main program but in future will use ScriptController that will allow hot updating Drivers. 

### Utils
- Main Responsibility is providing cross libraries helpers.
- ```Config``` provides static values for program.
- ```Config``` should provide complete paths to folders.
- ```Logger``` is logging singleton class that allows printing with color via fmt and saving n lines to file to debugging and finding failure points.

### App
- Main Entry point connecting other libraries into one executable and providing UI via ```RmlUI```.

### Limitation
- Current Version supports only **linux** because of not all windows specific implementation will be updated.
- Development stack on ```VSCode + cmake + gcc``` on windows installed via ```mingw64``` will be updated to support any dev stack.
- **[Script Controller]** with no Dependence Graph.
- **[Script Controller]** don't have all functionalities yet like ```recompileAll, etc```.
- **[Script Controller]** In future will provide build in scripts.
- **[MicroController]** For now Drivers are compiled into main program but in future will use ScriptController that will allow hot updating Drivers.
- Why this limitation exists? Because I'm tired of my life and don't have time to finish this bitch :v.



## Requirements
### General
- [**vcpkg**](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)
- [**cmake**](https://cmake.org/cmake/help/latest/command/install.html)
- [**gcc**](https://gcc.gnu.org/install/) (only for now in future support for other compilers)
- [**Ninja**](https://ninja-build.org/) (only for now in future support for other builders)
- [**Mingw64**](https://sourceforge.net/mingw-x64) 


## Prerequisites
- **cmake**
- **gcc**
- **ninja-build**
- **vcpkg**
- **RmlUI**
- **GLFW3**
- **fmt**
- **gtest**



## TODO
<details>
<summary>(Prototype) Iteration 1. Base Setup and Device Connection</summary>

- [x] Script Controller for Dlls management and hot reloading, scripting.
- [x] Basic Script for testing.
- [x] Device class and IDevice interface for end user access.
- [x] MicroController class for Device Management.
- [x] IDriver Interface providing required functions for Device. 
- [x] ArduinoDriver for testing.
- [x] Logger and coverage of failure points. 
- [x] RmlUI and GLFW3 window Initialization.
- [x] PC Controller contention design via Serial (tty) for arduino without hid support. 
- [x] Vcpkg setup.
- [x] Unit Tests.
- [x] Base Docs.
- [x] Tests Workflow.
</details>

<details open>
<summary>(Prototype) Iteration 2. Windows Support and UI</summary>

- [ ] Drivers by ScriptController and DLLS.
- [ ] Script Controller build in scripts provided by factory.
- [ ] Script Controller file split.
- [ ] Script Controller Tests.
- [ ] Script Controller Getter/Setters.
- [ ] Driver and Scripts os independent.
- [ ] MicroController more control.
- [ ] Windows Support.
- [ ] RmlUI.
- [ ] Usage Docs.
</details>
