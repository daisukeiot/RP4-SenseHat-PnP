#include <stdio.h>
#include "iothub/iothub_op.h"
#include "senseHat/sensehat.h"
#include "telemetry.h"

extern bool isConnected;

int main(int argc, char *argv[])
{
    IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceClientLL = NULL;
    char msgBuffer[1024];
    SENSEHAT_DATA* senseHat;

    printf("Starting Raspberry Pi4 + Sensehat IoT Hub Device App\r\n");

    senseHat = initialize_senseHat();

    //led_set(senseHat->led, 0, 0, 255, 0, 0);

    while (true)
    {
        processTelemetry(deviceClientLL, senseHat);

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