#pragma once

#ifndef _SIMPLEOBJECT_H_
#define _SIMPLEOBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SimpleObject_GetInstanceId(void *SimpleObjectContext, char *String, int32 MaxString);

// Interop Callbacks
int32 SimpleObject_ProcessInstance(void *SimpleObjectContext);
int32 SimpleObject_InvokeInstance(void *SimpleObjectContext, char *String, char *ResultString, int32 ResultStringLength);
int32 SimpleObject_ReleaseInstance(void **SimpleObjectContext);

/*********************************************************************/

int32 SimpleObject_SetIntProperty(void *SimpleObjectContext, int64 Property);
int32 SimpleObject_GetIntProperty(void *SimpleObjectContext, int64 *Property);

int32 SimpleObject_SetDblProperty(void *SimpleObjectContext, float64 Property);
int32 SimpleObject_GetDblProperty(void *SimpleObjectContext, float64 *Property);

int32 SimpleObject_SetBoolProperty(void *SimpleObjectContext, int32 Property);
int32 SimpleObject_GetBoolProperty(void *SimpleObjectContext, int32 *Property);

int32 SimpleObject_SetStringProperty(void *SimpleObjectContext, char *Property);
int32 SimpleObject_GetStringProperty(void *SimpleObjectContext, char *Property, int32 MaxPropertyLength);

int32 SimpleObject_RaiseTrigger(void *SimpleObjectContext, int64 Value, char *ResultString, int32 MaxResultStringLength);

/*********************************************************************/

int32 SimpleObject_Create(void **SimpleObjectContext);
int32 SimpleObject_Delete(void **SimpleObjectContext);

/*********************************************************************/

#endif