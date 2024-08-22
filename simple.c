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
    int64_t int_property;
    float64_t float_property;
    int32_t boolean_property;
    char string_property[320];
} SimpleStruct;

/********************************************************************/
// Notification functions

static bool Notification_OnUpdate(void *user_ptr, const char *type, const char *notification, const char *sender,
                                  echandle dictionary_handle) {
    SimpleStruct *simple = (SimpleStruct *)user_ptr;
    float64_t value_float = 0;
    int64_t value_int64 = 0;
    bool value_bool = false;
    const char *value_ptr = NULL;

    if (IDictionary_GetStringPtrByKey(dictionary_handle, "String", &value_ptr))
        Simple_SetStringProperty(simple, value_ptr);
    if (IDictionary_GetFloatByKey(dictionary_handle, "Float", &value_float))
        Simple_SetFloatProperty(simple, value_float);
    if (IDictionary_GetInt64ByKey(dictionary_handle, "Int", &value_int64))
        Simple_SetIntProperty(simple, value_int64);
    if (IDictionary_GetBooleanByKey(dictionary_handle, "Boolean", &value_bool))
        Simple_SetBooleanProperty(simple, value_bool);
    return true;
}

static bool Notification_OnValueResponse(void *user_ptr, const char *type, const char *notification, const char *sender,
                                         echandle dictionary_handle) {
    SimpleStruct *simple = (SimpleStruct *)user_ptr;
    const char *value_ptr = NULL;

    if (IDictionary_GetStringPtrByKey(dictionary_handle, "String", &value_ptr)) {
        // Do something with value
        printf("%s", value_ptr);
    }

    return true;
}

/********************************************************************/
// Concrete functions

bool Simple_SetIntProperty(void *handle, int64_t property) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    const int64_t old_property = simple->int_property;

    simple->int_property = property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(simple), 0,
                                              "{ \"newValue\": %lld, \"oldValue\": %lld }", simple->int_property,
                                              old_property);
    return true;
}

int64_t Simple_GetIntProperty(void *handle) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    return simple->int_property;
}

bool Simple_SetFloatProperty(void *handle, float64_t Property) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    const float64_t old_property = simple->float_property;

    simple->float_property = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(simple), 0,
                                              "{ \"newValue\": %g, \"oldValue\": %g }", simple->float_property,
                                              old_property);
    return true;
}

float64_t Simple_GetFloatProperty(void *handle) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    return simple->float_property;
}

bool Simple_SetBooleanProperty(void *handle, bool property) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    const bool old_property = simple->boolean_property;

    simple->boolean_property = property;

    NotificationCenter_FireAfterDelayWithJSON(
        "Simple", "Changed", Class_InstanceId(simple), 0, "{ \"newValue\": %s, \"oldValue\": %s }",
        simple->boolean_property ? "true" : "false", old_property ? "true" : "false");
    return true;
}

bool Simple_GetBooleanProperty(void *handle) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    return simple->boolean_property;
}

bool Simple_SetStringProperty(void *handle, const char *property) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    char old_property[320] = {0};

    strncpy(old_property, simple->string_property, sizeof(old_property));
    old_property[sizeof(old_property) - 1] = 0;
    strncpy(simple->string_property, property, sizeof(simple->string_property));
    simple->string_property[sizeof(simple->string_property) - 1] = 0;

    // FireWithJSON format: use %js for javascript string - automatically escapes string
    // Use after delay because I don't need it to wait for the return

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Class_InstanceId(simple), 0,
                                              "{ \"newValue\": \"%js\", \"oldValue\": \"%js\" }",
                                              simple->string_property, old_property);
    return true;
}

const char *Simple_GetStringProperty(void *handle) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    return simple->string_property;
}

bool Simple_StartValueRequest(void *handle) {
    SimpleStruct *simple = (SimpleStruct *)handle;

    // Add an observer to wait for a response to our request
    NotificationCenter_AddInstanceObserver("Simple", "ValueResponse", Class_InstanceId(simple), simple,
                                           Notification_OnValueResponse);

    // Send a request for a value
    NotificationCenter_FireAfterDelayWithJSON("Simple", "ValueRequest", Class_InstanceId(simple), 0, "{}");
    return true;
}

/*********************************************************************/
// Interop functions

bool Simple_GetInstanceId(void *handle, char *string, int32_t max_string) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    strncpy(string, Class_InstanceId(simple), max_string);
    string[max_string - 1] = 0;
    return true;
}

bool Simple_Process(void *handle) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return true;
}

bool Simple_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SimpleStruct *simple = (SimpleStruct *)handle;
    bool ret = true;
    const char *method = NULL;

    if (!IDictionary_GetStringPtrByKey(method_dictionary_handle, "method", &method))
        return false;

    if (strcmp(method, "setIntProperty") == 0) {
        bool return_value = false;
        int64_t value = 0;
        ret = IDictionary_GetInt64ByKey(method_dictionary_handle, "value", &value);
        if (ret)
            return_value = Simple_SetIntProperty(handle, value);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, NULL);
    } else if (strcmp(method, "getIntProperty") == 0) {
        const int64_t value = Simple_GetIntProperty(simple);
        IDictionary_AddInt(return_dictionary_handle, "returnValue", value, NULL);
    } else if (strcmp(method, "setFloatProperty") == 0) {
        bool return_value = false;
        float64_t value_float = 0;
        ret = IDictionary_GetFloatByKey(method_dictionary_handle, "value", &value_float);
        if (ret)
            return_value = Simple_SetFloatProperty(handle, value_float);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, NULL);
    } else if (strcmp(method, "getFloatProperty") == 0) {
        const float64_t value_float = Simple_GetFloatProperty(simple);
        IDictionary_AddFloat(return_dictionary_handle, "returnValue", value_float, NULL);
    } else if (strcmp(method, "setBooleanProperty") == 0) {
        bool return_value = false;
        bool value = 0;
        ret = IDictionary_GetBooleanByKey(method_dictionary_handle, "value", &value);
        if (ret)
            return_value = Simple_SetBooleanProperty(handle, value);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, NULL);
    } else if (strcmp(method, "getBooleanProperty") == 0) {
        const bool value = Simple_GetBooleanProperty(simple);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", value, NULL);
    } else if (strcmp(method, "setStringProperty") == 0) {
        bool return_value = false;
        const char *value_string = NULL;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "value", &value_string);
        if (ret)
            return_value = Simple_SetStringProperty(handle, value_string);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, NULL);
    } else if (strcmp(method, "getStringProperty") == 0) {
        const char *value_string = Simple_GetStringProperty(simple);
        IDictionary_AddString(return_dictionary_handle, "returnValue", value_string, NULL);
    } else if (strcmp(method, "startValueRequest") == 0) {
        ret = Simple_StartValueRequest(simple);
        IDictionary_AddInt(return_dictionary_handle, "returnValue", ret, NULL);
    } else {
        ret = false;
    }

    return ret;
}

/*********************************************************************/
// Creation/deletion functions

void *Simple_Create(void) {
    SimpleStruct *simple = (SimpleStruct *)calloc(1, sizeof(SimpleStruct));
    if (!simple)
        return NULL;

    Interop_GenerateInstanceId(simple->Class.InstanceId, sizeof(simple->Class.InstanceId));

    simple->Class.RefCount = 1;
    NotificationCenter_AddInstanceObserver("Simple", "Update", Class_InstanceId(simple), simple, Notification_OnUpdate);
    return simple;
}

void *Simple_AddRef(void *handle) {
    SimpleStruct *simple = (SimpleStruct *)handle;
    simple->Class.RefCount += 1;
    return simple;
}

int32_t Simple_Release(void **handle) {
    if (!handle || !*handle)
        return 0;
    SimpleStruct *simple = (SimpleStruct *)*handle;
    if (!simple)
        return 0;

    *handle = NULL;
    if (--simple->Class.RefCount == 0) {
        NotificationCenter_RemoveInstanceObserver("Simple", "ValueResponse", Class_InstanceId(simple), simple,
                                                  Notification_OnValueResponse);
        NotificationCenter_RemoveInstanceObserver("Simple", "Update", Class_InstanceId(simple), simple,
                                                  Notification_OnUpdate);
        free(simple);
        return 0;
    }
    return simple->Class.RefCount;
}

/*********************************************************************/
