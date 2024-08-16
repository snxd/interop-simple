/** @file Simple.js
 *  @class Simple Object
 *  @brief
 */
import {EventEmitter} from "events";

import {interop} from "../direct/Host";

class Simple extends EventEmitter {
  constructor(instanceId) {
    super();
    this.instanceId = instanceId;
    this.refCount = 1;
  }

  addRef() {
    this.refCount++;
  }
  release() {
    if (--this.refCount === 0) {
      this.emit("release");

      return interop.releaseInstance(this.instanceId);
    }
  }
  releaseAsync() {
    if (--this.refCount === 0) {
      this.emit("release");

      return interop.releaseInstanceAsync(this.instanceId);
    }
    return Promise.resolve();
  }
  invoke(method, methodArgs) {
    return interop.invoke(this.instanceId, method, methodArgs);
  }
  invokeAsync(method, methodArgs) {
    if (this.refCount === 0) throw Error("Unable to call 'invokeAsync' of null reference");
    const self = this, releaseRef = () => self.release();
    this.addRef();
    return interop.invokeAsync(this.instanceId, method, methodArgs).then(releaseRef, releaseRef);
  }
  invokeMemoize(method, methodArgs) {
    return interop.invokeMemoize(this.instanceId, method, methodArgs);
  }
  invokeMemoizeAsync(method, methodArgs) {
    if (this.refCount === 0) throw Error("Unable to call 'invokeMemoizeAsync' of null reference");
    const self = this, releaseRef = () => self.release();
    this.addRef();
    return interop.invokeMemoizeAsync(this.instanceId, method, methodArgs).then(releaseRef, releaseRef);
  }
  /**
   * Gets an integer value
   * @type int
   */
  getIntProperty() {
    return this.invoke("getIntProperty");
  }
  /**
   * Sets an integer value
   */
  setIntProperty(value) {
    return this.invoke("setIntProperty", {value});
  }
  /**
   * Gets a double value
   * @type float
   */
  getFloatProperty() {
    return this.invoke("getFloatProperty");
  }
  /**
   * Sets a double value
   */
  setFloatProperty(value) {
    return this.invoke("setFloatProperty", {value});
  }
  /**
   * Gets a boolean value
   * @type bool
   */
  getBooleanProperty() {
    return this.invoke("getBooleanProperty");
  }
  /**
   * Sets a boolean value
   */
  setBooleanProperty(value) {
    return this.invoke("setBooleanProperty", {value});
  }
  /**
   * Gets a string value
   * @type string
   */
  getStringProperty() {
    return this.invoke("getStringProperty");
  }
  /**
   * Sets a string value
   */
  setStringProperty(value) {
    return this.invoke("setStringProperty", {value});
  }
  /**
   * Starts a request for a value
   */
  startValueRequest() {
    return this.invoke("startValueRequest");
  }
}

export const createSimple = (instanceId) =>
  interop.createInstance("SSN.Simple", Simple, instanceId);
export const createAnalyticsAsync = (instanceId) =>
  interop.createInstanceAsync("SSN.Simple", Simple, instanceId);
export const createSimpleCopy = (obj) =>
  new Simple(interop.getInstanceId(obj));
