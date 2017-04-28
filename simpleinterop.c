#include <string.h>

#include "interoplib.h"

#include "simpleobject.h"

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
    InteropLib_SetOverride(Key, Value);
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
