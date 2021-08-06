#pragma once
#include <stdio.h>
#include "Runtime/Plugins/al/al.h"
void CheckLastOpenALError(const char *prefix, const char* file, long line, const char*operation);
//----------------------------alice extension
#define ALCheckError(x) 	{ CheckLastOpenALError (NULL,__FILE__, __LINE__,#x); }
#define OAL_CALL(x) do { x; ALCheckError(x); } while(0)
//----------------------------alice extension end