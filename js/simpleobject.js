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
SimpleObject.prototype.getInt64Property = function() {
    return this.invoke({
        "method": "getInt64Property"
    });
};
/**
 * Sets an integer value
 */
SimpleObject.prototype.setInt64Property = function(value) {
    return this.invoke({
        "method": "setInt64Property",
        "value": value
    });
};

/**
 * Gets a double value
 */
SimpleObject.prototype.getFloat64Property = function() {
    return this.invoke({
        "method": "getFloat64Property"
    });
};

/**
 * Sets a double value
 */
SimpleObject.prototype.setFloat64Property = function(value) {
    return this.invoke({
        "method": "setFloat64Property",
        "value": value
    });
};

/**
 * Gets a boolean value
 */
SimpleObject.prototype.getBooleanProperty = function() {
    return this.invoke({
        "method": "getBooleanProperty"
    });
};

/**
 * Sets a boolean value
 */
SimpleObject.prototype.setBooleanProperty = function(value) {
    return this.invoke({
        "method": "setBooleanProperty",
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

function createSimpleObject(instanceId) {
    return interop.createInstance("Simple.SimpleObject", SimpleObject, instanceId);
}

