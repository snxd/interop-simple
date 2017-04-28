// Remember to add simpleobject.js to your HTML scripts
(function () {
    function simpleInteropLoaded() {
        var testSimple = createSimpleObject();
        console.log("SimpleObject instanceId: " + testSimple.instanceId);

        testSimple.setIntProperty(406);
        console.log("SimpleObject getIntProperty: " + testSimple.getIntProperty());

        testSimple.setDblProperty(40.1);
        console.log("SimpleObject getDblProperty: " + testSimple.getDblProperty());

        testSimple.setBoolProperty(true);
        console.log("SimpleObject getBoolProperty: " + testSimple.getBoolProperty());

        testSimple.setStringProperty("String Test");
        console.log("SimpleObject getStringProperty: " + testSimple.getStringProperty());

        var observer = notificationCenter.addInstanceObserver("SimpleObject", "Trigger", testSimple, function (sender, info) {
            observer.release();

            console.log("SimpleObject Trigger: " + info.value);
            console.log("SimpleObject Trigger getStringProperty: " + sender.getStringProperty());

            // Make sure to release the object or else it will cause a memory leak
            testSimple.release();
        });

        testSimple.raiseTrigger(42);
    };
    
    interop.on("libraryLoad", function(info) {
        if (info.name.toLowerCase() == "simple") {
            simpleInteropLoaded();
        }
    });
}());
