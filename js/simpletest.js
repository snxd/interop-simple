// Remember to add simple.js to your HTML scripts
(function () {
    var simple = null;
    var observer = null;
    
    function interopLoaded() {

        //notificationCenter.verbose = true;
        //interop.verbose = true;
        
        simple = createSimple();

        // Watch for changes in object
        observer = notificationCenter.addInstanceObserver("Simple", "Changed", simple, function (sender, info) {
            console.log("Simple Changed - {0} -> {1}".format(info.oldValue, info.newValue));
        });
        
        console.log("Simple InstanceId - " + simple.instanceId);
        simple.setInt64Property(406);
        console.log("Simple Int64 - " + simple.getInt64Property());
        simple.setFloat64Property(40.1);
        console.log("Simple Float64 - " + simple.getFloat64Property());
        simple.setBooleanProperty(true);
        console.log("Simple Boolean - " + simple.getBooleanProperty());
        simple.setStringProperty("String Test");
        console.log("Simple String - " + simple.getStringProperty());
        
        // Send changes to object through notification center
        notificationCenter.fire("Simple", "Update", simple, { "Int64": -1});
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
