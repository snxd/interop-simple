#include <string.h>
#include "interop/interoptypes.h"
#include "interop/interoplib.h"

#include "simpleobject.h"

/*********************************************************************/

NotificationCenter_FireWithJSONCallback NotificationCenter_FireWithJSON = NULL;
NotificationCenter_FireAfterDelayWithJSONCallback NotificationCenter_FireAfterDelayWithJSON = NULL;
Interop_GenerateInstanceIdCallback Interop_GenerateInstanceId = NULL;

/*********************************************************************/

int32 Interop_CreateInstance(char *TypeName, char *InstanceId, int32 InstanceIdLength,
                             void *ExecuteUserPtr, Interop_ExecuteCallback Execute,
                             Interop_InvokeInstanceCallback *InvokeInstance,
                             Interop_ReleaseInstanceCallback *ReleaseInstance,
                             Interop_ProcessInstanceCallback *ProcessInstance,
                             void **UserPtr)
{
    void *Context;

    if (strcmp(TypeName, "Simple.SimpleObject") == 0)
    {
        *InvokeInstance = SimpleObject_InvokeInstance;
        *ReleaseInstance = SimpleObject_ReleaseInstance;
        *ProcessInstance = NULL;

        // Comment this line in if you want the process call
        // *ProcessInstance = SimpleObject_ProcessInstance;

        SimpleObject_Create(&Context);
        SimpleObject_GetInstanceId(Context, InstanceId, InstanceIdLength);
        *UserPtr = Context;
        return TRUE;
    }
    return FALSE;
}


int32 Interop_SetOverride(char *Key, void *Value)
{
    if (strcmp(Key, "NotificationCenter_FireWithJSON") == 0)
        NotificationCenter_FireWithJSON = Value;
    else if (strcmp(Key, "NotificationCenter_FireAfterDelayWithJSON") == 0)
        NotificationCenter_FireAfterDelayWithJSON = Value;
    else if (strcmp(Key, "Interop_GenerateInstanceId") == 0)
        Interop_GenerateInstanceId = Value;

    return TRUE;
}

int32 Interop_SetOption(char *Key, char *Value)
{
    return TRUE;
}

int32 Interop_Load()
{
    return TRUE;
}

int32 Interop_Unload()
{
    return TRUE;
}

/*********************************************************************/
