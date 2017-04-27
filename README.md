## Simple Interop Example

### Requirements

* Visual Studio 2013
* DIRECT 5

### Build Instructions

1. Open simpleinterop.sln
2. Select Debug and rebuild solution
3. Copy simpleinterop.dll from the Debug subdirectory to host.exe directory
4. Copy simpleobject.js and simpleobjecttest.js from the js directory to the skin directory
5. Edit workflow.json and add the following task to be run in the load entry point: ```
    "loadSimple": {
        "type": "interopLoad",
        "name": "simple",
        "path": "{ModulePath}{LibraryPrefix}simpleinterop.{LibraryExtension}"
    },```
6. Edit workflow.json and add the following task to be run in the unload entry point: ```
    "unloadSimple": {
        "type": "interopUnload",
        "name": "simple",
        "path": "{ModulePath}{LibraryPrefix}simpleinterop.{LibraryExtension}"
    },```
7. Open main.html and insert the following scripts after main.js: ```
    <script src="simpleobject.js" type="text/javascript"></script>
    <script src="simpleobjecttest.js" type="text/javascript"></script>```
8. Save main.html
9. Rebuild the skin using the builder
10. Make sure the simpleinterop.dll is still in the directory with host.exe
11. Run host.exe with --disablesecurity as the first argument (during production if you sign the dll you won't need this).

