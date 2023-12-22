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
    int64_t IntProperty;
    float64_t FloatProperty;
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
    if (IDictionary_GetFloatByKey(DictionaryHandle, "Float", &ValueFloat) == true)
        Simple_SetFloatProperty(Simple, ValueFloat);
    if (IDictionary_GetInt64ByKey(DictionaryHandle, "Int", &ValueInt64) == true)
        Simple_SetIntProperty(Simple, ValueInt64);
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

bool Simple_SetIntProperty(void *SimpleContext, int64_t Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    int64_t OldProperty = Simple->IntProperty;

    Simple->IntProperty = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(Simple), 0,
                                              "{ \"newValue\": %lld, \"oldValue\": %lld }", Simple->IntProperty,
                                              OldProperty);
    return true;
}

int64_t Simple_GetIntProperty(void *SimpleContext) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    return Simple->IntProperty;
}

bool Simple_SetFloatProperty(void *SimpleContext, float64_t Property) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    float64_t OldProperty = Simple->FloatProperty;

    Simple->FloatProperty = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(Simple), 0,
                                              "{ \"newValue\": %g, \"oldValue\": %g }", Simple->FloatProperty,
                                              OldProperty);
    return true;
}

float64_t Simple_GetFloatProperty(void *SimpleContext) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    return Simple->FloatProperty;
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

bool Simple_GetBooleanProperty(void *SimpleContext) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    return Simple->BooleanProperty;
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

const char *Simple_GetStringProperty(void *SimpleContext) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    return Simple->StringProperty;
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
    int32_t RetVal = false;
    int32_t ReturnValue = false;
    const char *Method = NULL;

    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == false)
        return false;

    if (strcmp(Method, "setInt64Property") == 0) {
        int64_t Value64 = 0;
        RetVal = IDictionary_GetInt64ByKey(MethodDictionaryHandle, "value", &Value64);
        if (RetVal == true)
            ReturnValue = Simple_SetIntProperty(SimpleContext, Value64);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getIntProperty") == 0) {
        int64_t Value64 = Simple_GetIntProperty(Simple);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    } else if (strcmp(Method, "setFloatProperty") == 0) {
        float64_t ValueFloat = 0;
        RetVal = IDictionary_GetFloatByKey(MethodDictionaryHandle, "value", &ValueFloat);
        if (RetVal == true)
            ReturnValue = Simple_SetFloatProperty(SimpleContext, ValueFloat);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getFloatProperty") == 0) {
        float64_t ValueFloat = Simple_GetFloatProperty(Simple);
        IDictionary_AddFloat(ReturnDictionaryHandle, "returnValue", ValueFloat, &ItemHandle);
    } else if (strcmp(Method, "setBooleanProperty") == 0) {
        bool ValueBool = 0;
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "value", &ValueBool);
        if (RetVal == true)
            ReturnValue = Simple_SetBooleanProperty(SimpleContext, ValueBool);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getBooleanProperty") == 0) {
        bool ValueBool = Simple_GetBooleanProperty(Simple);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ValueBool, &ItemHandle);
    } else if (strcmp(Method, "setStringProperty") == 0) {
        const char *ValueString = NULL;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "value", &ValueString);
        if (RetVal == true)
            ReturnValue = Simple_SetStringProperty(SimpleContext, ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getStringProperty") == 0) {
        const char *ValueString = Simple_GetStringProperty(Simple);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", ValueString, &ItemHandle);
    } else if (strcmp(Method, "startValueRequest") == 0) {
        RetVal = Simple_StartValueRequest(Simple);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", RetVal, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/deletion functions

void *Simple_Create(void) {
    SimpleStruct *Simple = (SimpleStruct *)calloc(1, sizeof(SimpleStruct));
    if (!Simple)
        return NULL;

    Interop_GenerateInstanceId(Simple->Class.InstanceId, sizeof(Simple->Class.InstanceId));

    Simple->Class.RefCount = 1;
    NotificationCenter_AddInstanceObserver("Simple", "Update", Class_InstanceId(Simple), Simple,
                                           Simple_Notification_OnUpdate);
    return Simple;
}

void *Simple_AddRef(void *SimpleContext) {
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    Simple->Class.RefCount += 1;
    return Simple;
}

int32_t Simple_Release(void **SimpleContext) {
    if (!SimpleContext || !*SimpleContext)
        return 0;
    SimpleStruct *Simple = (SimpleStruct *)*SimpleContext;
    if (!Simple)
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
