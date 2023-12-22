#pragma once

#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool Simple_SetIntProperty(void *SimpleContext, int64_t Property);
int64_t Simple_GetIntProperty(void *SimpleContext);

bool Simple_SetFloatProperty(void *SimpleContext, float64_t Property);
float64_t Simple_GetFloatProperty(void *SimpleContext);

bool Simple_SetBooleanProperty(void *SimpleContext, bool Property);
bool Simple_GetBooleanProperty(void *SimpleContext);

bool Simple_SetStringProperty(void *SimpleContext, const char *Property);
const char *Simple_GetStringProperty(void *SimpleContext);

bool Simple_StartValueRequest(void *SimpleContext);

// Interop callbacks
bool Simple_GetInstanceId(void *SimpleContext, char *String, int32_t MaxString);
bool Simple_Process(void *SimpleContext);
bool Simple_Invoke(void *SimpleContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

void *Simple_Create(void);
void *Simple_AddRef(void *SimpleContext);
int32_t Simple_Release(void **SimpleContext);

/*********************************************************************/

#endif