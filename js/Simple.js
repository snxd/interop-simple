/** @file Simple.js
 *  @class Simple Object
 *  @brief
 */

import {interop} from "../direct/Host";

// Node JS support
var EventEmitter;
if (typeof (require) !== "undefined") {
  if (typeof (EventEmitter) == "undefined") {
    EventEmitter = require("events");
  }
}

function Simple(instanceId) {
  this.instanceId = instanceId;
}

Simple.prototype = Object.create(EventEmitter.prototype, {
  constructor: {value: Simple, enumerable: false, writable: true, configurable: true}
});

Simple.prototype.release = function() {
  this.emit("finalize");
  this.releaseInstance();
};
Simple.prototype.invoke = function(methodBinding) {
  return interop.invoke(this.instanceId, methodBinding);
};
Simple.prototype.releaseInstance = function() {
  interop.releaseInstance(this.instanceId);
};

/**
 * Gets an integer value
 * @type int
 */
Simple.prototype.getIntProperty = function() {
  return this.invoke({
    "method": "getIntProperty"
  });
};
/**
 * Sets an integer value
 */
Simple.prototype.setIntProperty = function(value) {
  return this.invoke({
    "method": "setIntProperty",
    "value": value
  });
};
/**
 * Gets a double value
 * @type float
 */
Simple.prototype.getFloatProperty = function() {
  return this.invoke({
    "method": "getFloatProperty"
  });
};
/**
 * Sets a double value
 */
Simple.prototype.setFloatProperty = function(value) {
  return this.invoke({
    "method": "setFloatProperty",
    "value": value
  });
};
/**
 * Gets a boolean value
 * @type bool
 */
Simple.prototype.getBooleanProperty = function() {
  return this.invoke({
    "method": "getBooleanProperty"
  });
};
/**
 * Sets a boolean value
 */
Simple.prototype.setBooleanProperty = function(value) {
  return this.invoke({
    "method": "setBooleanProperty",
    "value": value
  });
};
/**
 * Gets a string value
 * @type string
 */
Simple.prototype.getStringProperty = function() {
  return this.invoke({
    "method": "getStringProperty"
  });
};
/**
 * Sets a string value
 */
Simple.prototype.setStringProperty = function(value) {
  return this.invoke({
    "method": "setStringProperty",
    "value": value
  });
};
/**
 * Starts a request for a value
 */
Simple.prototype.startValueRequest = function(value) {
  return this.invoke({
    "method": "startValueRequest"
  });
};

var createSimple = function(instanceId) {
  return interop.createInstance("SSN.Simple", Simple, instanceId);
};

export {createSimple};
