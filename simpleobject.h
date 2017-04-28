#pragma once

#ifndef _SIMPLEOBJECT_H_
#define _SIMPLEOBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SimpleObject_SetInt64Property(void *SimpleObjectContext, int64 Property);
int32 SimpleObject_GetInt64Property(void *SimpleObjectContext, int64 *Property);

int32 SimpleObject_SetFloat64Property(void *SimpleObjectContext, float64 Property);
int32 SimpleObject_GetFloat64Property(void *SimpleObjectContext, float64 *Property);

int32 SimpleObject_SetBooleanProperty(void *SimpleObjectContext, int32 Property);
int32 SimpleObject_GetBooleanProperty(void *SimpleObjectContext, int32 *Property);

int32 SimpleObject_SetStringProperty(void *SimpleObjectContext, char *Property);
int32 SimpleObject_GetStringProperty(void *SimpleObjectContext, char *Property, int32 MaxPropertyLength);

// Interop Callbacks
int32 SimpleObject_GetInstanceId(void *SimpleObjectContext, char *String, int32 MaxString);
int32 SimpleObject_Process(void *SimpleObjectContext);
int32 SimpleObject_Invoke(void *SimpleObjectContext, char *String, char *ResultString, int32 ResultStringLength);

/*********************************************************************/

int32 SimpleObject_Create(void **SimpleObjectContext);
void* SimpleObject_AddRef(void *SimpleObjectContext);
int32 SimpleObject_Release(void **SimpleObjectContext);

/*********************************************************************/

#endif