#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interoplib.h"
#include "dictionaryi.h"

#include "jansson.h"
#include "jansson_private.h"

#include "simple.h"

/*********************************************************************/

#define SIMPLE_MAXPARAMETERS        (10)

/*********************************************************************/

typedef struct SimpleStruct
{
    // Interop Storage Data
    ClassStruct                     Class;

    // Object Data
    int64                           Int64Property;
    float64                         Float64Property;
    int32                           BooleanProperty;
    char                            StringProperty[320];
} SimpleStruct;

/********************************************************************/
// Notification Functions

int32 Simple_Notification_OnUpdate(void *UserPtr, char *Type, char *Notification, void *Sender, echandle DictionaryHandle)
{
    SimpleStruct *Simple = (SimpleStruct *)UserPtr;
    float64 ValueFloat = 0;
    int64 ValueInt64 = 0;
    int32 ValueInt32 = 0;
    char *ValuePtr = NULL;

    if (IDictionary_GetStringPtrByKey(DictionaryHandle, "String", &ValuePtr) == TRUE)
        Simple_SetStringProperty(Simple, ValuePtr);
    if (IDictionary_GetFloat64ByKey(DictionaryHandle, "Float64", &ValueFloat) == TRUE)
        Simple_SetFloat64Property(Simple, ValueFloat);
    if (IDictionary_GetInt64ByKey(DictionaryHandle, "Int64", &ValueInt64) == TRUE)
        Simple_SetInt64Property(Simple, ValueInt64);
    if (IDictionary_GetBooleanByKey(DictionaryHandle, "Boolean", &ValueInt32) == TRUE)
        Simple_SetBooleanProperty(Simple, ValueInt32);
    return TRUE;
}

/********************************************************************/
// Concrete Functions

int32 Simple_SetInt64Property(void *SimpleContext, int64 Property)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    int64 OldProperty = Simple->Int64Property;

    Simple->Int64Property = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Simple, 0, 
        "{ \"newValue\": %lld, \"oldValue\": %lld }", Simple->Int64Property, OldProperty);

    return TRUE;
}

int32 Simple_GetInt64Property(void *SimpleContext, int64 *Property)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *Property = Simple->Int64Property;
    return TRUE;
}

int32 Simple_SetFloat64Property(void *SimpleContext, float64 Property)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    float64 OldProperty = Simple->Float64Property;

    Simple->Float64Property = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Simple, 0, 
        "{ \"newValue\": %g, \"oldValue\": %g }", Simple->Float64Property, OldProperty);

    return TRUE;
}

int32 Simple_GetFloat64Property(void *SimpleContext, float64 *Property)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *Property = Simple->Float64Property;
    return TRUE;
}

int32 Simple_SetBooleanProperty(void *SimpleContext, int32 Property)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    int32 OldProperty = Simple->BooleanProperty;

    Simple->BooleanProperty = Property;

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Simple, 0, 
        "{ \"newValue\": %s, \"oldValue\": %s }", Simple->BooleanProperty ? "true" : "false", 
        OldProperty ? "true" : "false");

    return TRUE;
}

int32 Simple_GetBooleanProperty(void *SimpleContext, int32 *Property)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *Property = Simple->BooleanProperty;
    return TRUE;
}

int32 Simple_SetStringProperty(void *SimpleContext, char *Property)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    char OldProperty[320] = { 0 };

    String_CopyLength(OldProperty, Simple->StringProperty, 320);
    String_CopyLength(Simple->StringProperty, Property, 320);
    
    // FireWithJSON format: use %js for javascript string - automatically escapes string
    // Use after delay because I don't need it to wait for the return

    NotificationCenter_FireAfterDelayWithJSON("Simple", "Changed", Simple, 0, 
        "{ \"newValue\": \"%js\", \"oldValue\": \"%js\" }", Simple->StringProperty, OldProperty);

    return TRUE;
}

int32 Simple_GetStringProperty(void *SimpleContext, char *Property, int32 MaxPropertyLength)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    String_CopyLength(Property, Simple->StringProperty, MaxPropertyLength);
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 Simple_GetInstanceId(void *SimpleContext, char *String, int32 MaxString)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    String_CopyLength(String, Class_InstanceId(Simple), MaxString);
    return TRUE;
}

int32 Simple_Process(void *SimpleContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return TRUE;
}

int32 Simple_Invoke(void *SimpleContext, char *String, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    char *MethodName = NULL;
    char MethodResultString[INTEROP_MAXSTRING];
    int64 MethodResultInt = 0;
    float64 MethodResultDbl = 0;
    int32 MethodResultBool = 0;
    char *JSONDumpString = NULL;
    json_t *JSON = NULL;
    json_t *JSONReturnRoot = NULL;
    json_t *JSONReturn = NULL;
    json_t *JSONMethod = NULL;
    json_t *Parameter[SIMPLE_MAXPARAMETERS];
    json_error_t JSONError;
    int32 RetVal = FALSE;

    memset(MethodResultString, 0, INTEROP_MAXSTRING);
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
            Simple_SetInt64Property(SimpleContext, json_integer_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getInt64Property") == TRUE)
    {
        if (RetVal == TRUE)
        {
            Simple_GetInt64Property(Simple, &MethodResultInt);
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
            Simple_SetFloat64Property(SimpleContext, json_real_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getFloat64Property") == TRUE)
    {
        if (RetVal == TRUE)
        {
            Simple_GetFloat64Property(Simple, &MethodResultDbl);
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
            Simple_SetBooleanProperty(SimpleContext, json_is_true(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getBooleanProperty") == TRUE)
    {
        if (RetVal == TRUE)
        {
            Simple_GetBooleanProperty(Simple, &MethodResultBool);
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
            Simple_SetStringProperty(Simple, (char*)json_string_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getStringProperty") == TRUE)
    {
        if (RetVal == TRUE)
        {
            Simple_GetStringProperty(Simple, MethodResultString, INTEROP_MAXSTRING);
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

int32 Simple_Create(void **SimpleContext)
{
    SimpleStruct *Simple = NULL;

    Simple = (SimpleStruct *)malloc(sizeof(SimpleStruct));
    Interop_GenerateInstanceId(Simple->Class.InstanceId, 40);

    Simple->Class.RefCount = 1;
    Simple->Int64Property = 0;
    Simple->Float64Property = 0.f;
    Simple->BooleanProperty = FALSE;

    NotificationCenter_AddInstanceObserver("Simple", "Update", Simple, Simple, Simple_Notification_OnUpdate);

    memset(Simple->StringProperty, 0, Element_Count(Simple->StringProperty));

    *SimpleContext = Simple;
    return TRUE;
}

void *Simple_AddRef(void *SimpleContext)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    Simple->Class.RefCount += 1;
    return Simple;
}

int32 Simple_Release(void **SimpleContext)
{
    SimpleStruct *Simple = (SimpleStruct *)*SimpleContext;

    if (--Simple->Class.RefCount == 0)
    {
        NotificationCenter_RemoveInstanceObserver("Simple", "Update", Simple, Simple, Simple_Notification_OnUpdate);
        free(Simple);
    }

    *SimpleContext = NULL;
    return TRUE;
}

/*********************************************************************/
