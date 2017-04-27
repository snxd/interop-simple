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

SimpleObject.prototype.getIntProperty = function() {
    return this.invoke({
        "method": "getIntProperty"
    });
};

SimpleObject.prototype.setIntProperty = function(value) {
    return this.invoke({
        "method": "setIntProperty",
        "value": value
    });
};

SimpleObject.prototype.getDblProperty = function() {
    return this.invoke({
        "method": "getDblProperty"
    });
};

SimpleObject.prototype.setDblProperty = function(value) {
    return this.invoke({
        "method": "setDblProperty",
        "value": value
    });
};

SimpleObject.prototype.getBoolProperty = function() {
    return this.invoke({
        "method": "getBoolProperty"
    });
};

SimpleObject.prototype.setBoolProperty = function(value) {
    return this.invoke({
        "method": "setBoolProperty",
        "value": value
    });
};

SimpleObject.prototype.getStringProperty = function() {
    return this.invoke({
        "method": "getStringProperty"
    });
};

SimpleObject.prototype.setStringProperty = function(value) {
    return this.invoke({
        "method": "setStringProperty",
        "value": value
    });
};

SimpleObject.prototype.raiseTrigger = function(value) {
    return this.invoke({
        "method": "raiseTrigger",
        "value": value
    });
};

function createSimpleObject(instanceId) {
    return interop.createInstance("Simple.SimpleObject", SimpleObject, instanceId);
}

