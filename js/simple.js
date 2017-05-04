/** @file simple.js
 *  @class Simple Object
 *  @brief 
 */
 
(function (root) {
    function Simple(instanceId) {
        this.instanceId = instanceId;
    }
    
    Simple.prototype = new EventEmitter();
    
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
     * @type float
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

    root.createSimple = function(instanceId) {
        return interop.createInstance("SSN.Simple", Simple, instanceId);
    };
    
}(_global()));