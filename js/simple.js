/** @file simple.js
 *  @class Simple Object
 *  @brief 
 */
function Simple(instanceId) {
    this.instanceId = instanceId;
}

Simple.prototype.release = function() {
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
 */
Simple.prototype.getInt64Property = function() {
    return this.invoke({
        "method": "getInt64Property"
    });
};
/**
 * Sets an integer value
 */
Simple.prototype.setInt64Property = function(value) {
    return this.invoke({
        "method": "setInt64Property",
        "value": value
    });
};

/**
 * Gets a double value
 */
Simple.prototype.getFloat64Property = function() {
    return this.invoke({
        "method": "getFloat64Property"
    });
};

/**
 * Sets a double value
 */
Simple.prototype.setFloat64Property = function(value) {
    return this.invoke({
        "method": "setFloat64Property",
        "value": value
    });
};

/**
 * Gets a boolean value
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
 */
Simple.prototype.getStringProperty = function() {
    return this.invoke({
        "method": "getStringProperty"
    });
};

/**
 * Sets a integer value
 */
Simple.prototype.setStringProperty = function(value) {
    return this.invoke({
        "method": "setStringProperty",
        "value": value
    });
};

function createSimple(instanceId) {
    return interop.createInstance("SSN.Simple", Simple, instanceId);
}

