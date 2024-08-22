#include <string.h>

#include "interoplib.h"
#include "interopstub.h"

#include "simple.h"

/*********************************************************************/

bool Interop_CreateInstance(const char *type_name, char *instance_id, int32_t max_instance_id, void *execute_user_ptr,
                            Interop_ExecuteCallback execute, Interop_InvokeInstanceCallback *invoke_instance,
                            Interop_ReleaseInstanceCallback *release_instance,
                            Interop_ProcessInstanceCallback *process_instance, void **user_ptr) {
    if (strcmp(type_name, "SSN.Simple") == 0) {
        void *context = Simple_Create();
        Simple_GetInstanceId(context, instance_id, max_instance_id);

        *invoke_instance = Simple_Invoke;
        *release_instance = Simple_Release;
        *process_instance = NULL;

        // Uncomment this line in if you want the process call
        // *process_instance = Simple_Process;

        *user_ptr = context;
        return true;
    }
    return false;
}

bool Interop_SetOption(const char *key, void *value) {
    return true;
}

bool Interop_Load(void) {
    return true;
}

bool Interop_Unload(void) {
    return true;
}

/*********************************************************************/
