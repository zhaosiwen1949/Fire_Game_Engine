#include "AudioDebugger.h"
#include <stdio.h>

void CheckLastOpenALError(const char *prefix, const char* file, long line, const char*operation)
{
	int err = alGetError();

	if (err != AL_NO_ERROR)
	{
		switch (err)
		{
		case AL_INVALID_NAME:
			printf("%d : AL_INVALID_NAME in %s\n",line, operation);
			break;

		case AL_INVALID_ENUM:
			printf("%d :AL_INVALID_ENUM in %s\n", line, operation);
			break;

		case AL_INVALID_VALUE:
			printf( "%d :AL_INVALID_VALUE in %s\n", line, operation);
			break;

		case AL_INVALID_OPERATION:
			printf("%d :AL_INVALID_OPERATION in %s\n", line, operation);
			break;

		case AL_OUT_OF_MEMORY:
			printf("%d :AL_OUT_OF_MEMORY in %s\n", line, operation);
			break;
		}
	}
}