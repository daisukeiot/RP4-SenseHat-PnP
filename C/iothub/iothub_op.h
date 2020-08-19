#ifndef _IOTHUB_OP
#define _IOTHUB_OP

#include <stdio.h>

#include "iothub.h"
#include "iothub_device_client_ll.h"
#include "iothub_client_options.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/xlogging.h"
#include "iothubtransportmqtt.h"

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
#include "azure_c_shared_utility/shared_util_options.h"
#include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES

static const char IOTHUB_CS[] = "IOTHUB_CS";

static bool g_hubClientTraceEnabled = false;

IOTHUB_DEVICE_CLIENT_LL_HANDLE IoTHubInitialize(void);

#endif // _IOTHUB_OP