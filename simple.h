#pragma once

#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool Simple_SetIntProperty(void *handle, int64_t property);
int64_t Simple_GetIntProperty(void *handle);

bool Simple_SetFloatProperty(void *handle, float64_t property);
float64_t Simple_GetFloatProperty(void *handle);

bool Simple_SetBooleanProperty(void *handle, bool property);
bool Simple_GetBooleanProperty(void *handle);

bool Simple_SetStringProperty(void *handle, const char *property);
const char *Simple_GetStringProperty(void *handle);

bool Simple_StartValueRequest(void *handle);

// Interop callbacks
bool Simple_GetInstanceId(void *handle, char *string, int32_t max_string);
bool Simple_Process(void *handle);
bool Simple_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle);

/*********************************************************************/

void *Simple_Create(void);
void *Simple_AddRef(void *handle);
int32_t Simple_Release(void **handle);

/*********************************************************************/

#endif