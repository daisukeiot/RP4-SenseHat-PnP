#pragma once

#ifndef _IOTHUB_DEVICE_TWIN
#define _IOTHUB_DEVICE_TWIN

#include "iothub_op.h"

void deviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char* payload, size_t size, void* userContextCallback);

#endif // _IOTHUB_DEVICE_TWIN