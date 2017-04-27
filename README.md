## Simple Interop Example

### Requirements

* Visual Studio 2013
* Xcode 8
* QtCreator 5.8
* DIRECT 5

### Build Instructions

1. Compile the dynamic library
2. Copy the dynamic library from the target directory to the host.exe directory
3. Edit workflow.json and add the following task to be run in the load entry point: ```
    "loadSimple": {
        "type": "interopLoad",
        "name": "simple",
        "path": "{ModulePath}{LibraryPrefix}simpleinterop.{LibraryExtension}"
    },```
4. Edit workflow.json and add the following task to be run in the unload entry point: ```
    "unloadSimple": {
        "type": "interopUnload",
        "name": "simple",
        "path": "{ModulePath}{LibraryPrefix}simpleinterop.{LibraryExtension}"
    },```
5. Copy simpleobject.js and simpleobjecttest.js from the js directory to the skin directory
6. Open main.html and insert the following scripts after main.js: ```
    <script src="simpleobject.js" type="text/javascript"></script>
    <script src="simpleobjecttest.js" type="text/javascript"></script>```
7. Run host.exe with --disablesecurity as the first argument (during production if you sign the dll you won't need this).

