#pragma once

#ifndef _SIMPLEOBJECT_H_
#define _SIMPLEOBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

/*********************************************************************/

void SimpleObject_GetInstanceId(void *SimpleObjectContext, char *String, int32 MaxString);

// Interop Callbacks
int32 SimpleObject_ProcessInstance(void *SimpleObjectContext);
int32 SimpleObject_InvokeInstance(void *SimpleObjectContext, char *String, char *ResultString, int32 ResultStringLength);
int32 SimpleObject_ReleaseInstance(void **SimpleObjectContext);

/*********************************************************************/

void SimpleObject_SetIntProperty(void *SimpleObjectContext, int Property);
void SimpleObject_GetIntProperty(void *SimpleObjectContext, int *Property);

void SimpleObject_SetDblProperty(void *SimpleObjectContext, double Property);
void SimpleObject_GetDblProperty(void *SimpleObjectContext, double *Property);

void SimpleObject_SetBoolProperty(void *SimpleObjectContext, int Property);
void SimpleObject_GetBoolProperty(void *SimpleObjectContext, int *Property);

void SimpleObject_SetStringProperty(void *SimpleObjectContext, char *Property);
void SimpleObject_GetStringProperty(void *SimpleObjectContext, char *Property, int32 MaxPropertyLength);

int32 SimpleObject_RaiseTrigger(void *SimpleObjectContext, int32 Value, char *ResultString, int32 MaxResultStringLength);

/*********************************************************************/

void SimpleObject_Create(void **SimpleObjectContext);
void SimpleObject_Delete(void **SimpleObjectContext);

/*********************************************************************/

#endif