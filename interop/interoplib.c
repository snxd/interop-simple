#include "interoplib.h"

#include <stdarg.h>

/*********************************************************************/

#define NOTIFICATIONCENTER_GLOBALSENDER ("Global")

/*********************************************************************/

Class_ConvertFromInstanceIdCallback Class_ConvertFromInstanceIdPtr = NULL;
Class_ConvertToInstanceIdCallback   Class_ConvertToInstanceIdPtr = NULL;
Class_TrackInstanceCallback         Class_TrackInstancePtr = NULL;
Class_UntrackInstanceCallback       Class_UntrackInstancePtr = NULL;

NotificationCenter_AddInstanceObserverCallback      NotificationCenter_AddInstanceObserverPtr = NULL;
NotificationCenter_RemoveInstanceObserverCallback   NotificationCenter_RemoveInstanceObserverPtr = NULL;
NotificationCenter_FireWithJSONCallback             NotificationCenter_FireWithJSONPtr = NULL;
NotificationCenter_FireWithJSONVCallback            NotificationCenter_FireWithJSONVPtr = NULL;
NotificationCenter_FireAfterDelayWithJSONCallback   NotificationCenter_FireAfterDelayWithJSONPtr = NULL;
NotificationCenter_FireAfterDelayWithJSONVCallback  NotificationCenter_FireAfterDelayWithJSONVPtr = NULL;

Interop_GenerateInstanceIdCallback  Interop_GenerateInstanceIdPtr = NULL;

/*********************************************************************/

int32 InteropLib_SetOverride(char *Key, void *Value)
{
    if (String_Compare(Key, "Class_ConvertFromInstanceId") == TRUE)
        Class_ConvertFromInstanceIdPtr = (Class_ConvertFromInstanceIdCallback)Value;
    else if (String_Compare(Key, "Class_ConvertToInstanceId") == TRUE)
        Class_ConvertToInstanceIdPtr = (Class_ConvertToInstanceIdCallback)Value;
    else if (String_Compare(Key, "Class_TrackInstance") == TRUE)
        Class_TrackInstancePtr = (Class_TrackInstanceCallback)Value;
    else if (String_Compare(Key, "Class_UntrackInstance") == TRUE)
        Class_UntrackInstancePtr = (Class_UntrackInstanceCallback)Value;

    if (String_Compare(Key, "NotificationCenter_AddInstanceObserver") == TRUE)
        NotificationCenter_AddInstanceObserverPtr = (NotificationCenter_AddInstanceObserverCallback)Value;
    else if (String_Compare(Key, "NotificationCenter_RemoveInstanceObserver") == TRUE)
        NotificationCenter_RemoveInstanceObserverPtr = (NotificationCenter_RemoveInstanceObserverCallback)Value;
    else if (String_Compare(Key, "NotificationCenter_FireWithJSON") == TRUE)
        NotificationCenter_FireWithJSONPtr = (NotificationCenter_FireWithJSONCallback)Value;
    else if (String_Compare(Key, "NotificationCenter_FireWithJSONV") == TRUE)
        NotificationCenter_FireWithJSONVPtr = (NotificationCenter_FireWithJSONVCallback)Value;
    else if (String_Compare(Key, "NotificationCenter_FireAfterDelayWithJSON") == TRUE)
        NotificationCenter_FireAfterDelayWithJSONPtr = (NotificationCenter_FireAfterDelayWithJSONCallback)Value;
    else if (String_Compare(Key, "NotificationCenter_FireAfterDelayWithJSONV") == TRUE)
        NotificationCenter_FireAfterDelayWithJSONVPtr = (NotificationCenter_FireAfterDelayWithJSONVCallback)Value;

    if (String_Compare(Key, "Interop_GenerateInstanceId") == TRUE)
        Interop_GenerateInstanceIdPtr = (Interop_GenerateInstanceIdCallback)Value;

    return TRUE;
}

/*********************************************************************/

void *Class_ConvertFromInstanceId(char *InstanceId)
{
    return Class_ConvertFromInstanceIdPtr(InstanceId);
}

char *Class_ConvertToInstanceId(void *Pointer)
{
    return Class_ConvertToInstanceIdPtr(Pointer);
}

int32 Class_TrackInstance(void *Pointer, char *InstanceId)
{
    return Class_TrackInstancePtr(Pointer, InstanceId);
}

int32 Class_UntrackInstance(void *Pointer)
{
    return Class_UntrackInstancePtr(Pointer);
}

int32 NotificationCenter_AddObserver(char *Type, char *Notification, void *UserPtr, NotificationCenter_ObserverCallback Callback)
{
    return NotificationCenter_AddInstanceObserver(Type, Notification, NOTIFICATIONCENTER_GLOBALSENDER, UserPtr, Callback);
}

int32 NotificationCenter_AddInstanceObserver(char *Type, char *Notification, void *Sender, void *UserPtr, NotificationCenter_ObserverCallback Callback)
{
    return NotificationCenter_AddInstanceObserverPtr(Type, Notification, Sender, UserPtr, Callback);
}

int32 NotificationCenter_RemoveObserver(char *Type, char *Notification, void *UserPtr, NotificationCenter_ObserverCallback Callback)
{
    return NotificationCenter_RemoveInstanceObserver(Type, Notification, NOTIFICATIONCENTER_GLOBALSENDER, UserPtr, Callback);
}

int32 NotificationCenter_RemoveInstanceObserver(char *Type, char *Notification, void *Sender, void *UserPtr, NotificationCenter_ObserverCallback Callback)
{
    return NotificationCenter_RemoveInstanceObserverPtr(Type, Notification, Sender, UserPtr, Callback);
}

int32 NotificationCenter_FireWithJSON(char *Type, char *Notification, void *Sender, char *Format, ...)
{
    va_list ArgumentList;
    int32 RetVal = FALSE;
    va_start(ArgumentList, Format);
    RetVal = NotificationCenter_FireWithJSONVPtr(Type, Notification, Sender, Format, ArgumentList);
    va_end(ArgumentList);
    return RetVal;
}

int32 NotificationCenter_FireAfterDelayWithJSON(char *Type, char *Notification, void *Sender, int32 DelayMS, char *Format, ...)
{
    va_list ArgumentList;
    int32 RetVal = FALSE;
    va_start(ArgumentList, Format);
    RetVal = NotificationCenter_FireAfterDelayWithJSONVPtr(Type, Notification, Sender, DelayMS, Format, ArgumentList);
    va_end(ArgumentList);
    return RetVal;
}

int32 Interop_GenerateInstanceId(char *String, int32 MaxString)
{
    return Interop_GenerateInstanceIdPtr(String, MaxString);
}