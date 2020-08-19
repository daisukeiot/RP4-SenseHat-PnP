#pragma once

#ifndef _TELEMETRY
#define _TELEMETRY

#include <parson.h>
#include "iothub.h"
#include "iothub_device_client_ll.h"
#include "iothub/d2c.h"
#include "senseHat/sensehat.h"

int processTelemetry(IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceClientLL, SENSEHAT_DATA* senseHat);

#endif