## Simple Interop Example

### Requirements

* Visual Studio 2013
* DIRECT 3.6 or higher

### Build Instructions

1. Open simpleinterop.sln
2. Select Debug and rebuild solution
3. Copy simpleinterop.dll from Debug subdirectory to host executable location
4. Copy simpleobject.js and simpleobjecttest.js from the js directory to the skin directory
5. Edit the workflow.json and load the interop during the load chain (also unloading during the unload chain)
6. Open html file for the main window (mainwindow.html) and insert the following scripts after mainwindow.js: ```
    <script src="simpleobject.js" type="text/javascript"></script>
    <script src="simpleobjecttest.js" type="text/javascript"></script>```
8. Save mainwindow.html
9. Rebuild the skin using the builder
10. Make sure the simpleinterop.dll is still in the directory with the host executable
11. Run the exe with --disablesecurity as the first argument (During production if you sign the dll you won't need this).

