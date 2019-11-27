#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interoplib.h"
#include "dictionaryi.h"

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

int32 Simple_Notification_OnValueResponse(void *UserPtr, char *Type, char *Notification, void *Sender, echandle DictionaryHandle)
{
    SimpleStruct *Simple = (SimpleStruct *)UserPtr;
    char *ValuePtr = NULL;

    if (IDictionary_GetStringPtrByKey(DictionaryHandle, "String", &ValuePtr) == TRUE)
    {
        // Do something with value
        printf("%s", ValuePtr);
    }

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

int32 Simple_GetStringPropertyPtr(void *SimpleContext, char **PropertyPtr)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    *PropertyPtr = Simple->StringProperty;
    return TRUE;
}

int32 Simple_StartValueRequest(void *SimpleContext)
{
    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;

    // Add an observer to wait for a response to our request
    NotificationCenter_AddInstanceObserver("Simple", "ValueResponse", Simple, Simple, Simple_Notification_OnValueResponse);

    // Send a request for a value
    NotificationCenter_FireAfterDelayWithJSON("Simple", "ValueRequest", Simple, 0, "{}");

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

int32 Simple_Invoke(void *SimpleContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SimpleStruct *Simple = (SimpleStruct *)SimpleContext;
    echandle ItemHandle = NULL;
    float64 ValueFloat64 = 0;
    int64 Value64 = 0;
    int32 ValueBool = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    char *Method = NULL;
    char *ValueString = NULL;

    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "setInt64Property") == TRUE)
    {
        RetVal = IDictionary_GetInt64ByKey(MethodDictionaryHandle, "value", &Value64);
        if (RetVal == TRUE)
            ReturnValue = Simple_SetInt64Property(SimpleContext, Value64);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getInt64Property") == TRUE)
    {
        RetVal = Simple_GetInt64Property(Simple, &Value64);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    }
    else if (String_Compare(Method, "setFloat64Property") == TRUE)
    {
        RetVal = IDictionary_GetFloat64ByKey(MethodDictionaryHandle, "value", &ValueFloat64);
        if (RetVal == TRUE)
            ReturnValue = Simple_SetFloat64Property(SimpleContext, ValueFloat64);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getFloat64Property") == TRUE)
    {
        RetVal = Simple_GetFloat64Property(Simple, &ValueFloat64);
        IDictionary_AddFloat64(ReturnDictionaryHandle, "returnValue", ValueFloat64, &ItemHandle);
    }
    else if (String_Compare(Method, "setBooleanProperty") == TRUE)
    {
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "value", &ValueBool);
        if (RetVal == TRUE)
            ReturnValue = Simple_SetBooleanProperty(SimpleContext, ValueBool);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getBooleanProperty") == TRUE)
    {
        RetVal = Simple_GetBooleanProperty(Simple, &ValueBool);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ValueBool, &ItemHandle);
    }
    else if (String_Compare(Method, "setStringProperty") == TRUE)
    {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "value", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = Simple_SetStringProperty(SimpleContext, ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getStringProperty") == TRUE)
    {
        RetVal = Simple_GetStringPropertyPtr(Simple, &ValueString);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", ValueString, &ItemHandle);
    }
    else if (String_Compare(Method, "startValueRequest") == TRUE)
    {
        RetVal = Simple_StartValueRequest(Simple);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", RetVal, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 Simple_Create(void **SimpleContext)
{
    SimpleStruct *Simple = NULL;

    Simple = (SimpleStruct *)malloc(sizeof(SimpleStruct));
    memset(Simple, 0, sizeof(SimpleStruct));
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
        NotificationCenter_RemoveInstanceObserver("Simple", "ValueResponse", Simple, Simple, Simple_Notification_OnValueResponse);
        NotificationCenter_RemoveInstanceObserver("Simple", "Update", Simple, Simple, Simple_Notification_OnUpdate);
        free(Simple);
    }

    *SimpleContext = NULL;
    return TRUE;
}

/*********************************************************************/
