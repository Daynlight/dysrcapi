# Dysrcapi 



## About



## TOC
- [About](#about)
- [TOC](#toc)
- [Installation](#installation)
- [Usage \[Will be added\]](#usage-will-be-added)
- [Architecture](#architecture)
  - [General](#general)
  - [ScriptController](#scriptcontroller)
  - [Limitation](#limitation)
- [Requirements](#requirements)
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

#### 2. Compilation via CMake
```bash
cmake -S . -B build -G Ninja && cmake --build build --target all
```
Set your toolchain -DCMAKE_TOOLCHAIN_FILE="$PWD/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"
Might required providing compiler by `-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++`.  
Build specific target `--target examples`.  
Provide path to toolchain `-DCMAKE_TOOLCHAIN_FILE="$PWD/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"`.

#### 3.1 Link in your project via cmake
```bash
cmake_minimum_required(VERSION 3.15)
project(Example LANGUAGES CXX)

find_package(dysrcapi CONFIG REQUIRED)
  
set(src 
  main.cpp
)

add_executable(Example ${src})
target_link_libraries(Example PRIVATE 
  dysrcapi::dysrcapi
)
```

#### 3.2 Run examples
```bash
./build/examples/examples
```

#### 3.3 Compile and run examples
```bash
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ && cmake --build build --target examples && ./build/examples/examples
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

### Limitation
- Development stack on ```VSCode + cmake + gcc``` on windows installed via ```mingw64``` will be updated to support any dev stack.
- **[Script Controller]** with no Dependence Graph.
- **[Script Controller]** don't have all functionalities yet like ```recompileAll, etc```.
- **[Script Controller]** In future will provide build in scripts.



## Requirements
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
- **fmt** -> for examples.
- **gtest** -> for tests.



## TODO
<details>
<summary>(Prototype) Iteration 1. Base Setup and Device Connection</summary>

- [x] Script Controller for Dlls management and hot reloading, scripting.
- [x] vcpkg port + cmake config for include in other projects.
- [x] Basic Script for testing.
- [x] Windows Support.
- [x] Vcpkg setup.
- [x] Base Docs.
</details>

<details open>
<summary>(Prototype) Iteration 2. Script Controller clean up and refactor</summary>

- [x] Script Controller file split.
- [x] Update Naming.
- [x] Update Namespace.
- [ ] Script Controller build in scripts provided by factory.
- [ ] Script Controller Tests.
- [ ] Script Controller Getter/Setters.
- [ ] Usage Docs.
- [ ] About Docs.
- [ ] Unit Tests.
- [ ] Github workflow.

</details>
