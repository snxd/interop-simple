#include <string.h>

#include "interoplib.h"
#include "interopstub.h"

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

    if (String_Compare(TypeName, "Simple.SimpleObject") == TRUE)
    {
        SimpleObject_Create(&Context);
        SimpleObject_GetInstanceId(Context, InstanceId, InstanceIdLength);

        *InvokeInstance = SimpleObject_Invoke;
        *ReleaseInstance = SimpleObject_Release;
        *ProcessInstance = NULL;

        // Comment this line in if you want the process call
        // *ProcessInstance = SimpleObject_Process;

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
