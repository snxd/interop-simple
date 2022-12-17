/** @file SimpleExample.js
 *  @brief example using the Simple class
 */

import {interop, notificationCenter} from "../direct/Host";
import {createSimple} from "./Simple";

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

  // Calls from JS to C are synchronous
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
  notificationCenter.fire("Simple", "Update", simple, {"Int64": -1});

  // Calls from C to JS are asynchronous so we have to listen for a request
  // and send a response through the notification center

  // Listen for a request for a value
  observer = notificationCenter.addInstanceObserver("Simple", "ValueRequest", simple, function (sender, info) {
    var response = {"String": "Item"};
    console.log("Simple Value Request/Response - {0} -> {1}"
      .format(JSON.stringify(info), JSON.stringify(response)));
    // Send out a response to our request with the values we want to send back to C
    notificationCenter.fire("Simple", "ValueResponse", simple, response);
    // We are done with the observer
    observer.release();
    observer = null;
  });

  // Trigger a request for a value, ideally would be called from C
  simple.startValueRequest();
}

function interopUnloaded() {
  // Release our notification center instance observer
  if (observer) {
    observer.release();
    observer = null;
  }
  // Release our object
  if (simple) {
    simple.release();
    simple = null;
  }
}

interop.on("load", function({name, successful}) {
  if (name === "simple") {
    if (successful) {
      console.log("Simple interop loaded successfully");
      interopLoaded();
    } else {
      console.log("Simple interop failed to load");
    }
  }
});

interop.on("unload", function({name}) {
  if (name === "simple") {
    interopUnloaded();
  }
});
