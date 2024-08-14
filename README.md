## Simple Interop Example for DIRECT 6.2

This is an example interop that shows how to create a simple javascript object.

### Contents

* js/simple.js - Javascript class
* js/simpletest.js - Javascript example
* interop - DIRECT interop headers
* mac - macOS specific files
* linux - Linux specific files
* windows - Windows specific files

### Requirements

* CMake 3.15

### Build Instructions

CMake is a makefile generator that produces solutions and project files for various compiler toolkits.

#### Visual Studio

```
cmake -S . -B build -A Win32 -D CMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
cmake --build build --config Debug
```

#### Xcode

```
cmake -S . -B build -G Xcode
cmake --build build --config Debug
```

#### Unix Makefiles

```
cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Setup Instructions

1. Compile the project or solution. The target architecture must match the architecture of the DIRECT client.
2. Copy the dynamic library from the target directory to the host directory
3. Edit *workflow.json* and add the following task to be run in the load entry point:
```json
"loadSimple": {
    "type": "interopLoad",
    "name": "simple",
    "path": "{moduleDirectory}{libraryPrefix}simple.{libraryExtension}"
},
```

4. Edit *workflow.json* and add the following task to be run in the unload entry point:
```json
"unloadSimple": {
    "type": "interopUnload",
    "name": "simple",
    "path": "{moduleDirectory}{libraryPrefix}simple.{libraryExtension}"
},
```

5. Copy *simple.js* and *simpletest.js* from the js directory to the skin directory
6. Open *main.html* and insert the following scripts after the reference to *main.js*. Example:
```html
<script type="text/javascript" src="main.js"></script>
<script type="text/javascript" src="simple.js"></script>
<script type="text/javascript" src="simpletest.js"></script>
```

7. Run host.exe with `--disablesecurity` as the first argument (during production if you sign the dll you will not need this).
