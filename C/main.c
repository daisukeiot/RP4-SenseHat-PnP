#include <stdio.h>
#include "iothub/iothub_op.h"
#include "senseHat/sensehat.h"
#include "telemetry.h"
#include "iothub/callback.h"

extern bool isConnected;

static const char ch_led_color[] = "led_color";

void deviceTwin_CB(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char* payload, size_t size, void* userContextCallback)
{
    JSON_Value* root_Value = NULL;
    JSON_Object* root_Object = NULL;
    JSON_Object* desired_Object = NULL;
    JSON_Value* version_Value = NULL;
    int version;
    size_t i;

    if ((root_Value = json_parse_string(payload)) == NULL)
    {
        LogError("Unable to parse Device Twin payload");
    }
    else if ((root_Object = json_value_get_object(root_Value)) == NULL)
    {
        LogError("Unable to get root object of JSON");
    }
    else
    {
        if (updateState == DEVICE_TWIN_UPDATE_COMPLETE)
        {
            desired_Object = json_object_get_object(root_Object, "desired");
        }
        else
        {
            desired_Object = root_Object;
        }
    }

    if (desired_Object == NULL)
    {
       LogError("Could not find Desired Property");
    }
    else if ((version_Value = json_object_get_value(desired_Object, "$version")) == NULL)
    {
        LogError("Could not find $version");
    }
    else if (json_value_get_type(version_Value) != JSONNumber)
    {
        LogError("$version is not a number");
    }
    else
    {
        int node_count = json_object_get_count(desired_Object);
        version = json_value_get_number(version_Value);

        for (i = 0; i < node_count; i++)
        {
            const char* node_Name = json_object_get_name(desired_Object, i);
            JSON_Value* desired_Value = json_object_get_value_at(desired_Object, i);

            if (strcmp(node_Name, ch_led_color) == 0)
            {
                if (json_type(desired_Value) == JSONNumber)
                {
                    int led_Color_Value = json_value_get_number(desired_Value);             

                    switch (led_Color_Value)
                    {
                        case 0:
                            LogInfo("LED Off");
                            break;
                        case 1:
                            LogInfo("LED Red");
                            break;
                        case 2:
                            LogInfo("LED Green");
                            break;
                        case 3:
                            LogInfo("LED Blue");
                            break;
                        case 4:
                            LogInfo("LED White");
                            break;
                        case 5:
                            LogInfo("LED Yellow");
                            break;
                        case 6:
                            LogInfo("LED Purple");
                            break;
                    }
                }
            }
        }
    }
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