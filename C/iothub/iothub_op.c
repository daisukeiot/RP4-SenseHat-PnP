#include "iothub_op.h"
#include "deviceTwin.h"
#include "deviceMethod.h"
#include "c2d.h"

#define PNP_ENABLE

static const char g_pnp_model_id[] = "dtmi:com:example:sensehat;1";

bool isConnected = false;

/*
** Receives callback when IoT Hub connection status change
**
** To Do : Add DPS support
*/
static void connection_status_callback(IOTHUB_CLIENT_CONNECTION_STATUS isConfigured, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* user_context)
{
    (void)reason;
    (void)user_context;

    if (isConfigured == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED)
    {
        LogInfo("The device client is connected to iothub");
        isConnected = true;
    }
    else
    {
        LogInfo("The device client has been disconnected");
        isConnected = false;
    }
}

/*
** Opens handle using Connection String
**
** To Do : Add DPS support
*/
static IOTHUB_DEVICE_CLIENT_LL_HANDLE CreateDeviceClientLLHandle(void)
{
    const char* cs;

    if ((cs = getenv(IOTHUB_CS)) == NULL)
    {
        LogError("Cannot read environment variable=%s", IOTHUB_CS);
        return NULL;
    }

    return IoTHubDeviceClient_LL_CreateFromConnectionString(cs, MQTT_Protocol);
}

/*
** Initialize IoT Hub connection
**
** To Do : Add DPS support
*/
IOTHUB_DEVICE_CLIENT_LL_HANDLE IoTHubInitialize(void)
{
    int iothubInitResult;
    bool isConfigured;
    IOTHUB_CLIENT_RESULT iothubResult;
    IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceHandle = NULL;
    bool urlAutoEncodeDecode = true;

    LogInfo("%s()", __func__);

    if ((iothubInitResult = IoTHub_Init()) != 0)
    {
        LogError("Failure to initialize client.  Error=%d", iothubInitResult);
        isConfigured = false;
    }
    else if ((deviceHandle = CreateDeviceClientLLHandle()) == NULL)
    {
        LogError("Failure creating IotHub client.  Hint: Check your connection string or DPS configuration");
        isConfigured = false;
    }
    else if ((iothubResult = IoTHubDeviceClient_LL_SetOption(deviceHandle, OPTION_LOG_TRACE, &g_hubClientTraceEnabled)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set logging option, error=%d", iothubResult);
        isConfigured = false;
    }
#ifdef PNP_ENABLE
    else if ((iothubResult = IoTHubDeviceClient_LL_SetOption(deviceHandle, OPTION_MODEL_ID, g_pnp_model_id)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set auto Url encode option, error=%d", iothubResult);
        isConfigured = false;
    }
#endif
    else if ((iothubResult = IoTHubDeviceClient_LL_SetOption(deviceHandle, OPTION_AUTO_URL_ENCODE_DECODE, &urlAutoEncodeDecode)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set auto Url encode option, error=%d", iothubResult);
        isConfigured = false;
    }
    else if ((iothubResult = IoTHubDeviceClient_LL_SetConnectionStatusCallback(deviceHandle, connection_status_callback, NULL)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set Connection Status callback, error=%d", iothubResult);
        isConfigured = false;
    }
    else if ((iothubResult = IoTHubDeviceClient_LL_SetDeviceMethodCallback(deviceHandle, deviceMethodCallback, (void*)deviceHandle)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set device method callback, error=%d", iothubResult);
        isConfigured = false;
    }
    else if ((iothubResult = IoTHubDeviceClient_LL_SetDeviceTwinCallback(deviceHandle, deviceTwinCallback, (void*)deviceHandle)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set Device Twin callback, error=%d", iothubResult);
        isConfigured = false;
    }
    else if ((iothubResult = IoTHubDeviceClient_LL_SetMessageCallback(deviceHandle, cloudMessageCallback, (void*)deviceHandle)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set Message callback, error=%d", iothubResult);
        isConfigured = false;
    }
#ifdef SET_TRUSTED_CERT_IN_SAMPLES
    // Setting the Trusted Certificate.  This is only necessary on systems without built in certificate stores.
    else if ((iothubResult = IoTHubDeviceClient_LL_SetOption(deviceHandle, OPTION_TRUSTED_CERT, certificates)) != IOTHUB_CLIENT_OK)
    {
        LogError("Unable to set the trusted cert, error=%d", iothubResult);
        isConfigured = false;
    }
#endif // SET_TRUSTED_CERT_IN_SAMPLES
    else
    {
        isConfigured = true;
    }

    if ((isConfigured == false) && (deviceHandle != NULL))
    {
        IoTHubDeviceClient_LL_Destroy(deviceHandle);
        deviceHandle = NULL;
    }

    if ((isConfigured == false) &&  (iothubInitResult == 0))
    {
        IoTHub_Deinit();
    }

    return deviceHandle;
}