## Simple Interop Example for DIRECT 5.2

This is an example interop that shows how to create a simple javascript object.

### Contents 

* js/simple.js - Javascript class
* js/simpletest.js - Javascript example
* interop - DIRECT interop headers
* mac - Macintosh specific files
* linux - Linux specific files
* windows - Windows specific files

### Requirements

* CMake 2.8

### Build Instructions

CMake is a makefile generator that produces solutions and project files for various compiler toolkits. 

#### Visual Studio

```
cmake .
cmake --build . --config Debug
```

#### Xcode

```
cmake . -G Xcode
cmake --build . --config Debug
```

#### Unix Makefiles

```
cmake . -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Setup Instructions

1. Compile the solution or project for your platform
2. Copy the dynamic library from the target directory to the host.exe directory
3. Edit workflow.json and add the following task to be run in the load entry point: ```
    "loadSimple": {
        "type": "interopLoad",
        "name": "simple",
        "path": "{ModuleDirectory}{LibraryPrefix}simple.{LibraryExtension}"
    },```
4. Edit workflow.json and add the following task to be run in the unload entry point: ```
    "unloadSimple": {
        "type": "interopUnload",
        "name": "simple",
        "path": "{ModuleDirectory}{LibraryPrefix}simple.{LibraryExtension}"
    },```
5. Copy simple.js and simpletest.js from the js directory to the skin directory
6. Open main.html and insert the following scripts after main.js: ```
    <script src="simple.js" type="text/javascript"></script>
    <script src="simpletest.js" type="text/javascript"></script>```
7. Run host.exe with --disablesecurity as the first argument (during production if you sign the dll you won't need this).

