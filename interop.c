#include <string.h>

#include "interoplib.h"
#include "interopstub.h"

#include "simple.h"

/*********************************************************************/

bool Interop_CreateInstance(const char *TypeName, char *InstanceId, int32_t InstanceIdLength, void *ExecuteUserPtr,
                            Interop_ExecuteCallback Execute, Interop_InvokeInstanceCallback *InvokeInstance,
                            Interop_ReleaseInstanceCallback *ReleaseInstance,
                            Interop_ProcessInstanceCallback *ProcessInstance, void **UserPtr) {
    void *Context;

    if (strcmp(TypeName, "SSN.Simple") == 0) {
        Simple_Create(&Context);
        Simple_GetInstanceId(Context, InstanceId, InstanceIdLength);

        *InvokeInstance = Simple_Invoke;
        *ReleaseInstance = Simple_Release;
        *ProcessInstance = NULL;

        // Uncomment this line in if you want the process call
        // *ProcessInstance = Simple_Process;

        *UserPtr = Context;
        return true;
    }
    return false;
}

bool Interop_SetOverride(const char *Key, void *Value) {
    InteropLib_SetOverride(Key, Value);
    return true;
}

bool Interop_SetOption(const char *Key, void *Value) {
    return true;
}

bool Interop_Load(void) {
    return true;
}

bool Interop_Unload(void) {
    return true;
}

/*********************************************************************/
