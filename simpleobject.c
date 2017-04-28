#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interoplib.h"
#include "dictionaryi.h"

#include "jansson.h"
#include "jansson_private.h"

#include "simpleobject.h"

/*********************************************************************/

typedef struct SimpleObjectStruct
{
    // Interop Storage Data
    ClassStruct                     Class;

    // Object Data
    int64                           Int64Property;
    float64                         Float64Property;
    int32                           BooleanProperty;
    char                            StringProperty[320];
} SimpleObjectStruct;

/********************************************************************/
// Notification Functions

int32 SimpleObject_Notification_OnUpdate(void *UserPtr, char *Type, char *Notification, void *Sender, echandle DictionaryHandle)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)UserPtr;
    float64 ValueFloat = 0;
    int64 ValueInt64 = 0;
    int32 ValueInt32 = 0;
    char *ValuePtr = NULL;

    if (IDictionary_GetStringPtrByKey(DictionaryHandle, "String", &ValuePtr) == TRUE)
        SimpleObject_SetStringProperty(SimpleObject, ValuePtr);
    if (IDictionary_GetFloat64ByKey(DictionaryHandle, "Float64", &ValueFloat) == TRUE)
        SimpleObject_SetFloat64Property(SimpleObject, ValueFloat);
    if (IDictionary_GetInt64ByKey(DictionaryHandle, "Int64", &ValueInt64) == TRUE)
        SimpleObject_SetInt64Property(SimpleObject, ValueInt64);
    if (IDictionary_GetBooleanByKey(DictionaryHandle, "Boolean", &ValueInt32) == TRUE)
        SimpleObject_SetBooleanProperty(SimpleObject, ValueInt32);
    return TRUE;
}

/********************************************************************/
// Concrete Functions

int32 SimpleObject_SetInt64Property(void *SimpleObjectContext, int64 Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    int64 OldProperty = SimpleObject->Int64Property;

    SimpleObject->Int64Property = Property;

    NotificationCenter_FireAfterDelayWithJSON("SimpleObject", "Changed", SimpleObject, 0, 
        "{ \"newValue\": %lld, \"oldValue\": %lld }", SimpleObject->Int64Property, OldProperty);

    return TRUE;
}

int32 SimpleObject_GetInt64Property(void *SimpleObjectContext, int64 *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    *Property = SimpleObject->Int64Property;
    return TRUE;
}

int32 SimpleObject_SetFloat64Property(void *SimpleObjectContext, float64 Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    float64 OldProperty = SimpleObject->Float64Property;

    SimpleObject->Float64Property = Property;

    NotificationCenter_FireAfterDelayWithJSON("SimpleObject", "Changed", SimpleObject, 0, 
        "{ \"newValue\": %g, \"oldValue\": %g }", SimpleObject->Float64Property, OldProperty);

    return TRUE;
}

int32 SimpleObject_GetFloat64Property(void *SimpleObjectContext, float64 *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    *Property = SimpleObject->Float64Property;
    return TRUE;
}

int32 SimpleObject_SetBooleanProperty(void *SimpleObjectContext, int32 Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    int32 OldProperty = SimpleObject->BooleanProperty;

    SimpleObject->BooleanProperty = Property;

    NotificationCenter_FireAfterDelayWithJSON("SimpleObject", "Changed", SimpleObject, 0, 
        "{ \"newValue\": %s, \"oldValue\": %s }", SimpleObject->BooleanProperty ? "true" : "false", 
        OldProperty ? "true" : "false");

    return TRUE;
}

int32 SimpleObject_GetBooleanProperty(void *SimpleObjectContext, int32 *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    *Property = SimpleObject->BooleanProperty;
    return TRUE;
}

int32 SimpleObject_SetStringProperty(void *SimpleObjectContext, char *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    char OldProperty[320] = { 0 };

    String_CopyLength(OldProperty, SimpleObject->StringProperty, 320);
    String_CopyLength(SimpleObject->StringProperty, Property, 320);
    
    // FireWithJSON format: use %js for javascript string - automatically escapes string
    // Use after delay because I don't need it to wait for the return

    NotificationCenter_FireAfterDelayWithJSON("SimpleObject", "Changed", SimpleObject, 0, 
        "{ \"newValue\": \"%js\", \"oldValue\": \"%js\" }", SimpleObject->StringProperty, OldProperty);

    return TRUE;
}

int32 SimpleObject_GetStringProperty(void *SimpleObjectContext, char *Property, int32 MaxPropertyLength)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    String_CopyLength(Property, SimpleObject->StringProperty, MaxPropertyLength);
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SimpleObject_GetInstanceId(void *SimpleObjectContext, char *String, int32 MaxString)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    String_CopyLength(String, Class_InstanceId(SimpleObject), MaxString);
    return TRUE;
}

int32 SimpleObject_Process(void *SimpleObjectContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return TRUE;
}

int32 SimpleObject_Invoke(void *SimpleObjectContext, char *String, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    char *MethodName = NULL;
    char MethodResultString[MAX_JSON_STRINGLENGTH];
    int64 MethodResultInt = 0;
    float64 MethodResultDbl = 0;
    int32 MethodResultBool = 0;
    char *JSONDumpString = NULL;
    json_t *JSON = NULL;
    json_t *JSONReturnRoot = NULL;
    json_t *JSONReturn = NULL;
    json_t *JSONMethod = NULL;
    json_t *Parameter[MAX_JSON_PARAMETERS];
    json_error_t JSONError;
    int32 RetVal = FALSE;

    memset(MethodResultString, 0, MAX_JSON_STRINGLENGTH);
    JSON = json_loads(String, 0, &JSONError);
    if (JSON == FALSE)
        return FALSE;

    RetVal = (JSONMethod = json_object_get(JSON, "method")) != NULL;

    if (RetVal == TRUE)
        RetVal = json_is_string(JSONMethod);

    if (RetVal == TRUE)
    {
        MethodName = (char *)json_string_value(JSONMethod);
        RetVal = MethodName != NULL;
    }

    if (RetVal == TRUE && String_Compare(MethodName, "setInt64Property") == TRUE)
    {
        if (RetVal == TRUE)
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_integer(Parameter[0]);
        if (RetVal == TRUE)
        {
            SimpleObject_SetInt64Property(SimpleObjectContext, json_integer_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getInt64Property") == TRUE)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetInt64Property(SimpleObject, &MethodResultInt);
            RetVal = (JSONReturn = json_integer(MethodResultInt)) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "setFloat64Property") == TRUE)
    {
        if (RetVal == TRUE)
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_real(Parameter[0]);
        if (RetVal == TRUE)
        {
            SimpleObject_SetFloat64Property(SimpleObjectContext, json_real_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getFloat64Property") == TRUE)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetFloat64Property(SimpleObject, &MethodResultDbl);
            RetVal = (JSONReturn = json_real(MethodResultDbl)) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "setBooleanProperty") == TRUE)
    {
        if (RetVal == TRUE)
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_boolean(Parameter[0]);
        if (RetVal == TRUE)
        {
            SimpleObject_SetBooleanProperty(SimpleObjectContext, json_is_true(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getBooleanProperty") == TRUE)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetBooleanProperty(SimpleObject, &MethodResultBool);
            RetVal = (JSONReturn = json_boolean(MethodResultBool)) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "setStringProperty") == TRUE)
    {
        if (RetVal == TRUE)
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_string(Parameter[0]);
        if (RetVal == TRUE)
        {
            SimpleObject_SetStringProperty(SimpleObject, (char*)json_string_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getStringProperty") == TRUE)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetStringProperty(SimpleObject, MethodResultString, MAX_JSON_STRINGLENGTH);
            RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
        }
    }

    // Set json return value
    if (RetVal == TRUE)
        RetVal = (JSONReturnRoot = json_object()) != NULL;
    if (RetVal == TRUE)
        RetVal = (json_object_set_new(JSONReturnRoot, "returnValue", JSONReturn) == 0);

    if (RetVal == TRUE)
        RetVal = (JSONDumpString = json_dumps(JSONReturnRoot, 0)) != NULL;

    if (RetVal == TRUE)
        RetVal = ((signed)String_Length(JSONDumpString) < ResultStringLength);
    if (RetVal == TRUE)
        String_CopyLength(ResultString, JSONDumpString, ResultStringLength);

    if (JSONDumpString != NULL)
        jsonp_free(JSONDumpString);
    if (JSONReturnRoot != NULL)
        json_decref(JSONReturnRoot);
    if (JSON != NULL)
        json_decref(JSON);

    return RetVal;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 SimpleObject_Create(void **SimpleObjectContext)
{
    SimpleObjectStruct *SimpleObject = NULL;

    SimpleObject = (SimpleObjectStruct *)malloc(sizeof(SimpleObjectStruct));
    Interop_GenerateInstanceId(SimpleObject->Class.InstanceId, 40);

    SimpleObject->Class.RefCount = 1;
    SimpleObject->Int64Property = 0;
    SimpleObject->Float64Property = 0.f;
    SimpleObject->BooleanProperty = FALSE;

    NotificationCenter_AddInstanceObserver("SimpleObject", "Update", SimpleObject, SimpleObject, SimpleObject_Notification_OnUpdate);

    memset(SimpleObject->StringProperty, 0, 320);

    *SimpleObjectContext = SimpleObject;
    return TRUE;
}

void *SimpleObject_AddRef(void *SimpleObjectContext)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    SimpleObject->Class.RefCount += 1;
    return SimpleObject;
}

int32 SimpleObject_Release(void **SimpleObjectContext)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)*SimpleObjectContext;

    if (--SimpleObject->Class.RefCount == 0)
    {
        NotificationCenter_RemoveInstanceObserver("SimpleObject", "Update", SimpleObject, SimpleObject, SimpleObject_Notification_OnUpdate);
        free(SimpleObject);
    }

    *SimpleObjectContext = NULL;
    return TRUE;
}

/*********************************************************************/
