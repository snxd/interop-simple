#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interoplib.h"
#include "dictionaryi.h"

#include "simple.h"

/*********************************************************************/

#define SIMPLE_MAXPARAMETERS (10)

/*********************************************************************/

typedef struct SimpleStruct {
    // Interop storage data
    ClassStruct Class;

    // Object data
    int64_t Int64Property;
    float64_t Float64Property;
    int32_t BooleanProperty;
    char StringProperty[320];
} SimpleStruct;

/********************************************************************/
// Notification functions

static bool Simple_Notification_OnUpdate(void *UserPtr, const char *Type, const char *Notification, const char *Sender,
                                         echandle DictionaryHandle) {
    SimpleStruct *Simple = (SimpleStruct *)UserPtr;
    float64_t ValueFloat = 0;
    int64_t ValueInt64 = 0;
    bool ValueBool = false;
    const char *ValuePtr = NULL;

    if (IDictionary_GetStringPtrByKey(DictionaryHandle, "String", &ValuePtr) == true)
        Simple_SetStringProperty(Simple, ValuePtr);
    if (IDictionary_GetFloat64ByKey(DictionaryHandle, "Float64", &ValueFloat) == true)
        Simple_SetFloat64Property(Simple, ValueFloat);
    if (IDictionary_GetInt64ByKey(DictionaryHandle, "Int64", &ValueInt64) == true)
        Simple_SetInt64Property(Simple, ValueInt64);
    if (IDictionary_GetBooleanByKey(DictionaryHandle, "Boolean", &ValueBool) == true)
        Simple_SetBooleanProperty(Simple, ValueBool);
    return true;
}

static bool Simple_Notification_OnValueResponse(void *UserPtr, const char *Type, const char *Notification,
                                                const char *Sender, echandle DictionaryHandle) {
    SimpleStruct *Simple = (SimpleStruct *)UserPtr;
    const char *ValuePtr = NULL;

    if (IDictionary_GetStringPtrByKey(DictionaryHandle, "String", &ValuePtr) == true) {
        // Do something with value
        printf("%s", ValuePtr);
    }

    return true;
}

/********************************************************************/
// Concrete functions

bool Simple_SetInt64Property(void *SimpleContext, int64_t Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    int64_t OldProperty = Simple->Int64Property;

    Simple->Int64Property = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(Simple), 0,
                                              "{ \"newValue\": %lld, \"oldValue\": %lld }", Simple->Int64Property,
                                              OldProperty);
    return true;
}

bool Simple_GetInt64Property(void *SimpleContext, int64_t *Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *Property = Simple->Int64Property;
    return true;
}

bool Simple_SetFloat64Property(void *SimpleContext, float64_t Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    float64_t OldProperty = Simple->Float64Property;

    Simple->Float64Property = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(Simple), 0,
                                              "{ \"newValue\": %g, \"oldValue\": %g }", Simple->Float64Property,
                                              OldProperty);
    return true;
}

bool Simple_GetFloat64Property(void *SimpleContext, float64_t *Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *Property = Simple->Float64Property;
    return true;
}

bool Simple_SetBooleanProperty(void *SimpleContext, bool Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    bool OldProperty = Simple->BooleanProperty;

    Simple->BooleanProperty = Property;

    NotificationCenter_FireAfterDelayWithJSON(
        "Simple", "Changed", Class_InstanceId(Simple), 0, "{ \"newValue\": %s, \"oldValue\": %s }",
        Simple->BooleanProperty ? "true" : "false", OldProperty ? "true" : "false");
    return true;
}

bool Simple_GetBooleanProperty(void *SimpleContext, bool *Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *Property = Simple->BooleanProperty;
    return true;
}

bool Simple_SetStringProperty(void *SimpleContext, const char *Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    char OldProperty[320] = {0};

    strncpy(OldProperty, Simple->StringProperty, sizeof(OldProperty));
    OldProperty[sizeof(OldProperty) - 1] = 0;
    strncpy(Simple->StringProperty, Property, sizeof(Simple->StringProperty));
    Simple->StringProperty[sizeof(Simple->StringProperty) - 1] = 0;

    // FireWithJSON format: use %js for javascript string - automatically escapes string
    // Use after delay because I don't need it to wait for the return

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(Simple), 0,
                                              "{ \"newValue\": \"%js\", \"oldValue\": \"%js\" }",
                                              Simple->StringProperty, OldProperty);
    return true;
}

bool Simple_GetStringProperty(void *SimpleContext, char *Property, int32_t MaxPropertyLength) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    strncpy(Property, Simple->StringProperty, MaxPropertyLength);
    Property[MaxPropertyLength - 1] = 0;
    return true;
}

bool Simple_GetStringPropertyPtr(void *SimpleContext, const char **PropertyPtr) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *PropertyPtr = Simple->StringProperty;
    return true;
}

bool Simple_StartValueRequest(void *SimpleContext) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;

    // Add an observer to wait for a response to our request
    NotificationCenter_AddInstanceObserver("Simple", "ValueResponse", Class_InstanceId(Simple), Simple,
                                           Simple_Notification_OnValueResponse);

    // Send a request for a value
    NotificationCenter_FireAfterDelayWithJSON("Simple", "ValueRequest", Class_InstanceId(Simple), 0, "{}");
    return true;
}

/*********************************************************************/
// Interop functions

bool Simple_GetInstanceId(void *SimpleContext, char *String, int32_t MaxString) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    strncpy(String, Class_InstanceId(Simple), MaxString);
    String[MaxString - 1] = 0;
    return true;
}

bool Simple_Process(void *SimpleContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return true;
}

bool Simple_Invoke(void *SimpleContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    echandle ItemHandle = NULL;
    float64_t ValueFloat64 = 0;
    int64_t Value64 = 0;
    int32_t RetVal = false;
    int32_t ReturnValue = false;
    bool ValueBool = 0;
    const char *Method = NULL;
    const char *ValueString = NULL;

    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == false)
        return false;

    if (strcmp(Method, "setInt64Property") == 0) {
        RetVal = IDictionary_GetInt64ByKey(MethodDictionaryHandle, "value", &Value64);
        if (RetVal == true)
            ReturnValue = Simple_SetInt64Property(SimpleContext, Value64);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getInt64Property") == 0) {
        RetVal = Simple_GetInt64Property(Simple, &Value64);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    } else if (strcmp(Method, "setFloat64Property") == 0) {
        RetVal = IDictionary_GetFloat64ByKey(MethodDictionaryHandle, "value", &ValueFloat64);
        if (RetVal == true)
            ReturnValue = Simple_SetFloat64Property(SimpleContext, ValueFloat64);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getFloat64Property") == 0) {
        RetVal = Simple_GetFloat64Property(Simple, &ValueFloat64);
        IDictionary_AddFloat64(ReturnDictionaryHandle, "returnValue", ValueFloat64, &ItemHandle);
    } else if (strcmp(Method, "setBooleanProperty") == 0) {
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "value", &ValueBool);
        if (RetVal == true)
            ReturnValue = Simple_SetBooleanProperty(SimpleContext, ValueBool);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getBooleanProperty") == 0) {
        RetVal = Simple_GetBooleanProperty(Simple, &ValueBool);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ValueBool, &ItemHandle);
    } else if (strcmp(Method, "setStringProperty") == 0) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "value", &ValueString);
        if (RetVal == true)
            ReturnValue = Simple_SetStringProperty(SimpleContext, ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getStringProperty") == 0) {
        RetVal = Simple_GetStringPropertyPtr(Simple, &ValueString);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", ValueString, &ItemHandle);
    } else if (strcmp(Method, "startValueRequest") == 0) {
        RetVal = Simple_StartValueRequest(Simple);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", RetVal, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/deletion functions

bool Simple_Create(void **SimpleContext) {
    SimpleStruct *Simple = NULL;

    Simple = (SimpleStruct *)malloc(sizeof(SimpleStruct));
    if (Simple == NULL)
        return false;
    memset(Simple, 0, sizeof(SimpleStruct));
    Interop_GenerateInstanceId(Simple->Class.InstanceId, sizeof(Simple->Class.InstanceId));

    Simple->Class.RefCount = 1;
    Simple->Int64Property = 0;
    Simple->Float64Property = 0.f;
    Simple->BooleanProperty = false;

    NotificationCenter_AddInstanceObserver("Simple", "Update", Class_InstanceId(Simple), Simple,
                                           Simple_Notification_OnUpdate);

    memset(Simple->StringProperty, 0, sizeof(Simple->StringProperty));

    *SimpleContext = Simple;
    return true;
}

void *Simple_AddRef(void *SimpleContext) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    Simple->Class.RefCount += 1;
    return Simple;
}

int32_t Simple_Release(void **SimpleContext) {
    SimpleStruct *Simple;

    if (SimpleContext == NULL || *SimpleContext == NULL)
        return 0;
    Simple = (SimpleStruct *)*SimpleContext;
    if (Simple == NULL)
        return 0;

    *SimpleContext = NULL;
    if (--Simple->Class.RefCount == 0) {
        NotificationCenter_RemoveInstanceObserver("Simple", "ValueResponse", Class_InstanceId(Simple), Simple,
                                                  Simple_Notification_OnValueResponse);
        NotificationCenter_RemoveInstanceObserver("Simple", "Update", Class_InstanceId(Simple), Simple,
                                                  Simple_Notification_OnUpdate);
        free(Simple);
        return 0;
    }
    return Simple->Class.RefCount;
}

/*********************************************************************/
