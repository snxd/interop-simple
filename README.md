## Simple Interop Example

### Requirements

* Visual Studio 2013
* DIRECT 4 or higher

### Build Instructions

1. Open simpleinterop.sln
2. Select Debug and rebuild solution
3. Copy simpleinterop.dll from Debug subdirectory to host executable location
4. Copy simpleobject.js and simpleobjecttest.js from the js directory to the skin directory
5. Edit the workflow.json and load the interop during the load chain (also unloading during the unload chain)
6. Add the following under the loadDownloader item ```
    "loadSimple": {
        "type": "loadInterop",
        "name": "simple",
        "filename": "{ModulePath}{LibraryPrefix}simpleinterop.{LibraryExtension}"
    },```
7. Add "loadSimple" to the actions item in the load queue
8. Add the following under the unloadDownloader item ```
    "unloadSimple": {
        "type": "unloadInterop",
        "name": "simple",
        "filename": "{ModulePath}{LibraryPrefix}simpleinterop.{LibraryExtension}"
    },```
9. Open mainwindow.html and insert the following scripts after mainwindow.js: ```
    <script src="simpleobject.js" type="text/javascript"></script>
    <script src="simpleobjecttest.js" type="text/javascript"></script>```
10. Save mainwindow.html
11. Rebuild the skin using the builder
12. Make sure the simpleinterop.dll is still in the directory with the host executable
13. Run the exe with --disablesecurity as the first argument (During production if you sign the dll you won't need this).

