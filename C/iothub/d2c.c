#include "d2c.h"

/* 
** Callback function for Device to Cloud message send confirmation
*/
static void sendMessageCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *context)
{
	LogInfo("Message received by IoT Hub. Result is: %s", MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
}

/* 
** Send Device to Cloud Message
**
** To Do : Add Message Property
*/
bool sendMessage(IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceHandle, char* message)
{
    IOTHUB_CLIENT_RESULT iothubResult;
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromString(message);

	if (messageHandle == 0) {
		LogError("Unable to create a new IoTHubMessage");
		return false;
	}

    if ((iothubResult = IoTHubDeviceClient_LL_SendEventAsync(deviceHandle, messageHandle, sendMessageCallback, message)) != IOTHUB_CLIENT_OK)
    {
        LogError("IoTHubClient_LL_SendEventAsync : %s", MU_ENUM_TO_STRING(IOTHUB_CLIENT_RESULT, iothubResult));
    }
    else
    {
        LogInfo("IoTHubClient_LL_SendEventAsync accepted message for transmission to IoT Hub");
    }

    return true;
}