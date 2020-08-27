#include "deviceTwin.h"

/* 
** Callback function to receive Device Twin update notification
*/
void deviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char* payload, size_t size, void* userContextCallback)
{
    IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceClient = (IOTHUB_DEVICE_CLIENT_LL_HANDLE)userContextCallback;
    LogInfo("%s() invoked : Update Status %s Paload %s", __func__, MU_ENUM_TO_STRING(DEVICE_TWIN_UPDATE_STATE, updateState), payload);

    if (_deviceTwinCallback_fn != NULL)
    {
        _deviceTwinCallback_fn(updateState, payload, size, userContextCallback);
    }
}
