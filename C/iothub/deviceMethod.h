#ifndef _IOTHUB_DIRECT_METHOD
#define _IOTHUB_DIRECT_METHOD

#include "iothub_op.h"

int deviceMethodCallback(const char* methodName, const unsigned char* payload, size_t size, unsigned char** response, size_t* responseSize, void* userContextCallback);

#endif // _IOTHUB_DIRECT_METHOD