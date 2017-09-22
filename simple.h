#pragma once

#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 Simple_SetInt64Property(void *SimpleContext, int64 Property);
int32 Simple_GetInt64Property(void *SimpleContext, int64 *Property);

int32 Simple_SetFloat64Property(void *SimpleContext, float64 Property);
int32 Simple_GetFloat64Property(void *SimpleContext, float64 *Property);

int32 Simple_SetBooleanProperty(void *SimpleContext, int32 Property);
int32 Simple_GetBooleanProperty(void *SimpleContext, int32 *Property);

int32 Simple_SetStringProperty(void *SimpleContext, char *Property);
int32 Simple_GetStringProperty(void *SimpleContext, char *Property, int32 MaxPropertyLength);
int32 Simple_GetStringPropertyPtr(void *SimpleContext, char **PropertyPtr);

// Interop Callbacks
int32 Simple_GetInstanceId(void *SimpleContext, char *String, int32 MaxString);
int32 Simple_Process(void *SimpleContext);
int32 Simple_Invoke(void *SimpleContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

int32 Simple_Create(void **SimpleContext);
void* Simple_AddRef(void *SimpleContext);
int32 Simple_Release(void **SimpleContext);

/*********************************************************************/

#endif