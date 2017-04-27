Simple Interop Example

1. Open simpleinterop.sln in Visual Studio 2013
2. Select "Debug" and rebuild solution
3. Copy simpleinterop from "Debug" subdirectory to host exe location
4. Copy simpleobject.js and simpleobjecttest.js from the js directory to the sample skin directory with mainwindow.html (mainwindow)
5. Edit the workflow.json and load the interop during the load chain (also unloading during the unload chain)
6. Open sample skin file - mainwindow.html
7. Following "<script src="mainwindow.js" type="text/javascript"></script>" add 
        <script src="simpleobject.js" type="text/javascript"></script>
        <script src="simpleobjecttest.js" type="text/javascript"></script>
8. Save mainwindow.html
9. Rebuild the skin using the builder
10. Make sure the simpleinterop.dll is still in the directory with the host executable
11. Run the exe with --disablesecurity as the first argument (During production if you sign the dll you won't need this).

