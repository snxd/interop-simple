#pragma once

#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool Simple_SetInt64Property(void *SimpleContext, int64_t Property);
bool Simple_GetInt64Property(void *SimpleContext, int64_t *Property);

bool Simple_SetFloat64Property(void *SimpleContext, float64_t Property);
bool Simple_GetFloat64Property(void *SimpleContext, float64_t *Property);

bool Simple_SetBooleanProperty(void *SimpleContext, bool Property);
bool Simple_GetBooleanProperty(void *SimpleContext, bool *Property);

bool Simple_SetStringProperty(void *SimpleContext, const char *Property);
bool Simple_GetStringProperty(void *SimpleContext, char *Property, int32_t MaxPropertyLength);
bool Simple_GetStringPropertyPtr(void *SimpleContext, const char **PropertyPtr);

bool Simple_StartValueRequest(void *SimpleContext);

// Interop callbacks
bool Simple_GetInstanceId(void *SimpleContext, char *String, int32_t MaxString);
bool Simple_Process(void *SimpleContext);
bool Simple_Invoke(void *SimpleContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

bool Simple_Create(void **SimpleContext);
void *Simple_AddRef(void *SimpleContext);
int32_t Simple_Release(void **SimpleContext);

/*********************************************************************/

#endif