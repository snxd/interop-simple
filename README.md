## Simple Interop Example

### Contents 

* bin - Priority interop dlls
* js/simpleobject.js - Javascript class
* js/simpleobjecttest.js - Javascript example
* interop - DIRECT interop headers
* jansson - Jansson javascript parser
* mac - Xcode project files
* linux - QtCreator project files
* windows - Windows project files

### Requirements

* Visual Studio 2013
* Xcode 8
* QtCreator 5.8
* DIRECT 5

### Setup Instructions

1. Compile the solution
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
5. Copy simpleobject.js and simpleobjecttest.js from the js directory to the skin directory
6. Open main.html and insert the following scripts after main.js: ```
    <script src="simpleobject.js" type="text/javascript"></script>
    <script src="simpleobjecttest.js" type="text/javascript"></script>```
7. Run host.exe with --disablesecurity as the first argument (during production if you sign the dll you won't need this).

