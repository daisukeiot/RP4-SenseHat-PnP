#include <stdio.h>
#include "iothub/iothub_op.h"
#include "senseHat/sensehat.h"
#include "telemetry.h"
#include "iothub/callback.h"

extern bool isConnected;

void deviceTwin_CB(const unsigned char* payload, size_t size, void* userContextCallback)
{
    LogInfo("%s %s", __func__, payload);
}

IOTHUBMESSAGE_DISPOSITION_RESULT cd2_CB(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    LogInfo("%s %s", __func__, message);
    return IOTHUBMESSAGE_ACCEPTED;
}

int deviceMethod_CB(const char* methodName, const unsigned char* payload, size_t size, unsigned char** response, size_t* responseSize, void* userContextCallback)
{
    LogInfo("%s %s", __func__, methodName);
    return 0;
}

int main(int argc, char *argv[])
{
    IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceClientLL = NULL;
    char msgBuffer[1024];
    SENSEHAT_DATA* senseHat;

    printf("Starting Raspberry Pi4 + Sensehat IoT Hub Device App\r\n");

    senseHat = initialize_senseHat();

    while (true)
    {
        if (isConnected != true)
        {
            if (deviceClientLL == NULL)
            {
                if ((deviceClientLL = IoTHubInitialize()) == NULL)
                {
                    LogError("Failed to initialize IoT Hub connection.  Retrying in 5 seconds");
                    ThreadAPI_Sleep(5000);
                    continue;
                }
                else
                {
                    set_callback(CB_DEVICE_TWIN, deviceTwin_CB);
                    set_callback(CB_DEVICE_METHOD, deviceMethod_CB);
                    set_callback(CB_C2D_MESSAGE, cd2_CB);
                }
            }

            // wait for connection up to 10 sec
            int retry = 100;
            while (isConnected != true && retry > 0)
            {
                ThreadAPI_Sleep(100);
                IoTHubDeviceClient_LL_DoWork(deviceClientLL);
            }

            if (isConnected != true)
            {
                LogError("IoT Hub connection not established after 10 seconds");
            }
        }

        if (isConnected == true)
        {
            // process periodic tasks
            // e.g. sending telemetry
            processTelemetry(deviceClientLL, senseHat);
            ThreadAPI_Sleep(3000);
        }
        else
        {
            LogInfo("Not Connected");
        }

        IoTHubDeviceClient_LL_DoWork(deviceClientLL);
    }

    return 0;
}