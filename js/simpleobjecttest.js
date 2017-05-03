// Remember to add simpleobject.js to your HTML scripts
(function () {
    var simple = null;
    var observer = null;
    
    function interopLoaded() {

        //notificationCenter.verbose = true;
        //interop.verbose = true;
        
        simple = createSimpleObject();

        // Watch for changes in object
        observer = notificationCenter.addInstanceObserver("SimpleObject", "Changed", simple, function (sender, info) {
            console.log("SimpleObject Changed - {0} -> {1}".format(info.oldValue, info.newValue));
        });
        
        console.log("SimpleObject InstanceId - " + simple.instanceId);
        simple.setInt64Property(406);
        console.log("SimpleObject Int64 - " + simple.getInt64Property());
        simple.setFloat64Property(40.1);
        console.log("SimpleObject Float64 - " + simple.getFloat64Property());
        simple.setBooleanProperty(true);
        console.log("SimpleObject Boolean - " + simple.getBooleanProperty());
        simple.setStringProperty("String Test");
        console.log("SimpleObject String - " + simple.getStringProperty());
        
        // Send changes to object through notification center
        notificationCenter.fire("SimpleObject", "Update", simple, { "Int64": -1});
    };
    
    function interopUnloaded() {
        // Release our notification center instance observer
        if (!isNull(observer)) {
            observer.release();
        }
        // Release our object
        if (!isNull(simple)) {
            simple.release();
        }
    };
    
    interop.on("libraryLoad", function(info) {
        if (info.name.toLowerCase() == "simple") {
            interopLoaded();
        }
    });
    
    interop.on("libraryUnload", function(info) {
        if (info.name.toLowerCase() == "simple") {
            interopUnloaded();
        }
    });
}());
