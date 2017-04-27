function SimpleObject(instanceId) {
    this.instanceId = instanceId;
}

SimpleObject.prototype.release = function() {
    interop.releaseInstance(this.instanceId);
};

SimpleObject.prototype.getIntProperty = function() {
    var methodBinding = {
        "method": "getIntProperty"
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.setIntProperty = function(value) {
    var methodBinding = {
        "method": "setIntProperty",
        "value": value
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.getDblProperty = function() {
    var methodBinding = {
        "method": "getDblProperty"
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.setDblProperty = function(value) {
    var methodBinding = {
        "method": "setDblProperty",
        "value": value
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.getBoolProperty = function() {
    var methodBinding = {
        "method": "getBoolProperty"
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.setBoolProperty = function(value) {
    var methodBinding = {
        "method": "setBoolProperty",
        "value": value
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.getStringProperty = function() {
    var methodBinding = {
        "method": "getStringProperty"
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.setStringProperty = function(value) {
    var methodBinding = {
        "method": "setStringProperty",
        "value": value
    };
    return interop.invoke(this.instanceId, methodBinding);
};

SimpleObject.prototype.raiseTrigger = function(value) {
    var methodBinding = {
        "method": "raiseTrigger",
        "value": value
    };
    return interop.invoke(this.instanceId, methodBinding);
};

function createSimpleObject(instanceId) {
    return interop.createInstance("Simple.SimpleObject", SimpleObject, instanceId);
}

