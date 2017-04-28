/** @file simpleobject.js
 *  @class SimpleObject
 *  @brief 
 */
function SimpleObject(instanceId) {
    this.instanceId = instanceId;
}

SimpleObject.prototype.release = function() {
    this.releaseInstance();
};
SimpleObject.prototype.invoke = function(methodBinding) {
    return interop.invoke(this.instanceId, methodBinding);
};
SimpleObject.prototype.releaseInstance = function() {
    interop.releaseInstance(this.instanceId);
};

/**
 * Gets an integer value
 */
SimpleObject.prototype.getIntProperty = function() {
    return this.invoke({
        "method": "getIntProperty"
    });
};
/**
 * Sets an integer value
 */
SimpleObject.prototype.setIntProperty = function(value) {
    return this.invoke({
        "method": "setIntProperty",
        "value": value
    });
};

/**
 * Gets a double value
 */
SimpleObject.prototype.getDblProperty = function() {
    return this.invoke({
        "method": "getDblProperty"
    });
};

/**
 * Sets a double value
 */
SimpleObject.prototype.setDblProperty = function(value) {
    return this.invoke({
        "method": "setDblProperty",
        "value": value
    });
};

/**
 * Gets a boolean value
 */
SimpleObject.prototype.getBoolProperty = function() {
    return this.invoke({
        "method": "getBoolProperty"
    });
};

/**
 * Sets a boolean value
 */
SimpleObject.prototype.setBoolProperty = function(value) {
    return this.invoke({
        "method": "setBoolProperty",
        "value": value
    });
};

/**
 * Gets a string value
 */
SimpleObject.prototype.getStringProperty = function() {
    return this.invoke({
        "method": "getStringProperty"
    });
};

/**
 * Sets a integer value
 */
SimpleObject.prototype.setStringProperty = function(value) {
    return this.invoke({
        "method": "setStringProperty",
        "value": value
    });
};

/**
 * Raises a notification
 */
SimpleObject.prototype.raiseTrigger = function(value) {
    return this.invoke({
        "method": "raiseTrigger",
        "value": value
    });
};

function createSimpleObject(instanceId) {
    return interop.createInstance("Simple.SimpleObject", SimpleObject, instanceId);
}

