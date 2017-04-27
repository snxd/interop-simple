#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interoplib.h"

#include "jansson.h"

#include "simpleobject.h"

/*********************************************************************/

typedef struct SimpleObjectStruct
{
    // Interop Storage Data : Guids are 32 bytes long - I need the null (so I rounded up to the next 64-bit value which is 40)
    char                            InstanceId[40];

    // Object Data
    int32                           IntProperty;
    float64                         DblProperty;
    int32                           BoolProperty;
    char                            StringProperty[320];
} SimpleObjectStruct;

/********************************************************************/

extern NotificationCenter_FireWithJSONCallback NotificationCenter_FireWithJSON;
extern NotificationCenter_FireAfterDelayWithJSONCallback NotificationCenter_FireAfterDelayWithJSON;
extern Interop_GenerateInstanceIdCallback Interop_GenerateInstanceId;

/********************************************************************/
// Interop Functions

int32 SimpleObject_GetInstanceId(void *SimpleObjectContext, char *String, int32 MaxString)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    strncpy(String, SimpleObject->InstanceId, MaxString);
    return TRUE;
}

int32 SimpleObject_ProcessInstance(void *SimpleObjectContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return TRUE;
}

int32 SimpleObject_InvokeInstance(void *SimpleObjectContext, char *String, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    char *MethodName = NULL;
    char MethodResultString[MAX_JSON_STRINGLENGTH];
    int32 MethodResultInt = 0;
    double MethodResultDbl = 0;
    char *JSONDumpString = NULL;
    json_t *JSON = NULL;
    json_t *JSONReturnRoot = NULL;
    json_t *JSONReturn = NULL;
    json_t *JSONMethod = NULL;
    json_t *Parameter[MAX_JSON_PARAMETERS];
    json_error_t JSONError;
    int32 RetVal = FALSE;

    memset(MethodResultString, 0, MAX_JSON_STRINGLENGTH);
    JSON = json_loads(String, &JSONError);
    if (JSON == FALSE)
    {
        return FALSE;
    }

    RetVal = (JSONMethod = json_object_get(JSON, "method")) != NULL;

    if (RetVal == TRUE)
    {
        RetVal = json_is_string(JSONMethod);
    }

    if (RetVal == TRUE)
    {
        MethodName = (char *)json_string_value(JSONMethod);
        RetVal = MethodName != NULL;
    }

    if (RetVal == TRUE && strcmp(MethodName, "setIntProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        }

        if (RetVal == TRUE)
        {
            RetVal = json_is_integer(Parameter[0]);
        }

        if (RetVal == TRUE)
        {
            SimpleObject_SetIntProperty(SimpleObjectContext, json_integer_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "getIntProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetIntProperty(SimpleObject, &MethodResultInt);
            RetVal = (JSONReturn = json_integer(MethodResultInt)) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "setDblProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        }

        if (RetVal == TRUE)
        {
            RetVal = json_is_real(Parameter[0]);
        }

        if (RetVal == TRUE)
        {
            SimpleObject_SetDblProperty(SimpleObjectContext, json_real_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "getDblProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetDblProperty(SimpleObject, &MethodResultDbl);
            RetVal = (JSONReturn = json_real(MethodResultDbl)) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "setBoolProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        }

        if (RetVal == TRUE)
        {
            RetVal = json_is_boolean(Parameter[0]);
        }

        if (RetVal == TRUE)
        {
            SimpleObject_SetBoolProperty(SimpleObjectContext, json_is_true(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "getBoolProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetBoolProperty(SimpleObject, &MethodResultInt);
            RetVal = (JSONReturn = (MethodResultInt == TRUE) ? json_true() : json_false()) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "setStringProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        }
        if (RetVal == TRUE)
        {
            RetVal = json_is_string(Parameter[0]);
        }
        if (RetVal == TRUE)
        {
            SimpleObject_SetStringProperty(SimpleObject, (char*)json_string_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "getStringProperty") == 0)
    {
        if (RetVal == TRUE)
        {
            SimpleObject_GetStringProperty(SimpleObject, MethodResultString, MAX_JSON_STRINGLENGTH);
            RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "raiseTrigger") == 0)
    {
        if (RetVal == TRUE)
        {
            RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        }

        if (RetVal == TRUE)
        {
            RetVal = json_is_integer(Parameter[0]);
        }

        if (RetVal == TRUE)
        {
            SimpleObject_RaiseTrigger(SimpleObject, json_integer_value(Parameter[0]), MethodResultString, MAX_JSON_STRINGLENGTH);
            RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
        }
    }

    // Set json return value
    if (RetVal == TRUE)
    {
        RetVal = (JSONReturnRoot = json_object()) != NULL;
    }

    if (RetVal == TRUE)
    {
        RetVal = (json_object_set_new(JSONReturnRoot, "returnValue", JSONReturn) == 0);
    }

    if (RetVal == TRUE)
    {
        RetVal = (JSONDumpString = json_dumps(JSONReturnRoot, 0)) != NULL;
    }

    if (RetVal == TRUE)
    {
        RetVal = ((signed)strlen(JSONDumpString) < ResultStringLength);
    }

    if (RetVal == TRUE)
    {
        strncpy(ResultString, JSONDumpString, ResultStringLength);
    }

    if (JSONDumpString != NULL)
    {
        free(JSONDumpString);
    }
    if (JSONReturnRoot != NULL)
    {
        json_decref(JSONReturnRoot);
    }
    if (JSON != NULL)
    {
        json_decref(JSON);
    }

    return RetVal;
}

int32 SimpleObject_ReleaseInstance(void **SimpleObjectContext)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)*SimpleObjectContext;
    SimpleObject_Delete((void *)&SimpleObject);
    *SimpleObjectContext = NULL;
    return TRUE;
}

/*********************************************************************/
// Concrete Functions

int32 SimpleObject_SetIntProperty(void *SimpleObjectContext, int32 Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    SimpleObject->IntProperty = Property;
    return TRUE;
}

int32 SimpleObject_GetIntProperty(void *SimpleObjectContext, int32 *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    *Property = SimpleObject->IntProperty;
    return TRUE;
}

int32 SimpleObject_SetDblProperty(void *SimpleObjectContext, float64 Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    SimpleObject->DblProperty = Property;
    return TRUE;
}

int32 SimpleObject_GetDblProperty(void *SimpleObjectContext, float64 *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    *Property = SimpleObject->DblProperty;
    return TRUE;
}

int32 SimpleObject_SetBoolProperty(void *SimpleObjectContext, int32 Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    SimpleObject->BoolProperty = Property;
    return TRUE;
}

int32 SimpleObject_GetBoolProperty(void *SimpleObjectContext, int32 *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    *Property = SimpleObject->BoolProperty;
    return TRUE;
}

int32 SimpleObject_SetStringProperty(void *SimpleObjectContext, char *Property)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    strncpy(SimpleObject->StringProperty, Property, 320);
    return TRUE;
}

int32 SimpleObject_GetStringProperty(void *SimpleObjectContext, char *Property, int32 MaxPropertyLength)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;
    strncpy(Property, SimpleObject->StringProperty, MaxPropertyLength);
    return TRUE;
}

int32 SimpleObject_RaiseTrigger(void *SimpleObjectContext, int32 Value, char *ResultString, int32 MaxResultStringLength)
{
    // This function calls onTrigger in the simpleobject.js file
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)SimpleObjectContext;

    // FireWithJSON format: use %js for javascript string - automatically escapes string
    // Use after delay because I don't need it to wait for the return
    NotificationCenter_FireAfterDelayWithJSON("SimpleObject", "Trigger", SimpleObject->InstanceId, 0,
        "{ \"value\": %d }", Value);

    return TRUE;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 SimpleObject_Create(void **SimpleObjectContext)
{
    SimpleObjectStruct *SimpleObject = NULL;

    SimpleObject = (SimpleObjectStruct *)malloc(sizeof(SimpleObjectStruct));
    Interop_GenerateInstanceId(SimpleObject->InstanceId, 40);

    SimpleObject->IntProperty = 0;
    SimpleObject->DblProperty = 0.f;
    SimpleObject->BoolProperty = FALSE;

    memset(SimpleObject->StringProperty, 0, 320);

    *SimpleObjectContext = SimpleObject;
    return TRUE;
}

int32 SimpleObject_Delete(void **SimpleObjectContext)
{
    SimpleObjectStruct *SimpleObject = (SimpleObjectStruct *)*SimpleObjectContext;

    free(SimpleObject);

    *SimpleObjectContext = NULL;
    return TRUE;
}

/*********************************************************************/
