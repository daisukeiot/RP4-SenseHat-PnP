#include "main.h"
#include "senseHat/sensehat.h"
#include "iothub/callback.h"
#include "iothub/iothub_op.h"

static const char ch_led_color[] = "led_color";

int main(int argc, char *argv[])
{
    IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceClientLL = NULL;
    char msgBuffer[1024];
    APP_CONTEXT* appContext;
    SENSEHAT_DATA* senseHat;

    printf("Starting Raspberry Pi4 + Sensehat IoT Hub Device App\r\n");

    appContext = malloc(sizeof(APP_CONTEXT));

    if (appContext == NULL)
    {
        printf("Failed to allocate app context\r\n");
        return -1;
    }

    senseHat = initialize_senseHat();
    appContext->isConnected = false;
    appContext->deviceData = senseHat;

    while (true)
    {
        if (appContext->isConnected != true)
        {
            if (deviceClientLL == NULL)
            {
                if ((appContext->deviceClient = IoTHubInitialize(appContext)) == NULL)
                {
                    LogError("Failed to initialize IoT Hub connection.  Retrying in 5 seconds");
                    ThreadAPI_Sleep(5000);
                    continue;
                }
                else
                {
                    set_callback(CB_DEVICE_TWIN, deviceTwin_CB, appContext);
                    set_callback(CB_DEVICE_METHOD, deviceMethod_CB, appContext);
                    set_callback(CB_C2D_MESSAGE, c2d_CB, appContext);
                }
            }

            // wait for connection up to 10 sec
            int retry = 100;
            while (appContext->isConnected != true && retry > 0)
            {
                ThreadAPI_Sleep(100);
                IoTHubDeviceClient_LL_DoWork(appContext->deviceClient);
            }

            if (appContext->isConnected != true)
            {
                LogError("IoT Hub connection not established after 10 seconds");
            }
        }

        if (appContext->isConnected == true)
        {
            // process periodic tasks
            // e.g. sending telemetry
            processTelemetry(appContext);
            ThreadAPI_Sleep(3000);
        }
        else
        {
            LogInfo("Not Connected");
        }

        IoTHubDeviceClient_LL_DoWork(appContext->deviceClient);
    }

    if (appContext != NULL)
    {
        free(appContext);
    }

    return 0;
}

/*
** Callback on Device Twin changes
*/
void deviceTwin_CB(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char* payload, size_t size, void* userContextCallback)
{
    APP_CONTEXT* appContext = (APP_CONTEXT* )userContextCallback;
    SENSEHAT_DATA* senseHat = appContext->deviceData;
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
                    char buffer[128];
                    uint8_t r = 0;
                    uint8_t g = 0;
                    uint8_t b = 0;
                    int led_Color_Value = json_value_get_number(desired_Value);             

                    switch (led_Color_Value)
                    {
                        case 0:
                            LogInfo("LED Off");
                            break;
                        case 1:
                            LogInfo("LED Red");
                            r = 255;
                            break;
                        case 2:
                            LogInfo("LED Green");
                            g = 255;
                            break;
                        case 3:
                            LogInfo("LED Blue");
                            b = 255;
                            break;
                        case 4:
                            LogInfo("LED White");
                            r = 255;
                            g = 255;
                            b = 255;
                            break;
                        case 5:
                            LogInfo("LED Yellow");
                            r = 255;
                            g = 255;
                            break;
                        case 6:
                            LogInfo("LED Purple");
                            r = 255;
                            b = 255;
                            break;
                    }

                    led_fill(senseHat->led, r, g, b);

                    if (snprintf(buffer, sizeof(buffer), "{\"led_color\":{\"value\":%d,\"ac\":%d,\"av\":%d,\"ad\":\"LED color set to %d\"}}", led_Color_Value, 200, version, led_Color_Value) < 0)
                    {
                        LogError("snprintf building targetTemperature response failed");
                    }
                    else
                    {
                        updateReportedState(appContext->deviceClient, buffer);
                    }
                }
            }
        }
    }
}

/*
** Callback on C2D
*/
IOTHUBMESSAGE_DISPOSITION_RESULT c2d_CB(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    LogInfo("%s %s", __func__, message);
    APP_CONTEXT* appContext = (APP_CONTEXT* )userContextCallback;
    return IOTHUBMESSAGE_ACCEPTED;
}

/*
** Callback on Device Method
*/
int deviceMethod_CB(const char* methodName, const unsigned char* payload, size_t size, unsigned char** response, size_t* responseSize, void* userContextCallback)
{
    LogInfo("%s %s", __func__, methodName);
    APP_CONTEXT* appContext = (APP_CONTEXT* )userContextCallback;
    return 0;
}


int processTelemetry(APP_CONTEXT* appContext)
{
    HTS221_DATA hts221_data;
    LPS25H_DATA lps25h_data;
    ORIANTATION_DATA gyro;
    ORIANTATION_DATA accel;
    ORIANTATION_DATA compass;
    FUSION_DATA fusion;
    char str[40];
    float float2decimal;
    IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceClient;
    SENSEHAT_DATA* senseHat = appContext->deviceData;

    printf("processTelemetry\r\n");

    deviceClient = appContext->deviceClient;

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;

    if (senseHat)
    {
        if (hts221_read(senseHat->hts221, &hts221_data) >= 0)
        {
            if (hts221_data.isTemperatureValid)
            {
//#ifdef DEBUG_TELEMETRY
                printf("Temp   : %f\r\n", hts221_data.temperature);
//#endif
                json_object_set_number(root_object, "temperature_hts221", hts221_data.temperature);
            }

            if (hts221_data.isHumidityValid)
            {
#ifdef DEBUG_TELEMETRY
                printf("Humid  : %f\r\n", hts221_data.humidity);
#endif
                json_object_set_number(root_object, "humidity", hts221_data.humidity);
            }
        }

        if (lps25h_read(senseHat->lps25h, &lps25h_data) >= 0)
        {
            if (lps25h_data.isTemperatureValid)
            {
#ifdef DEBUG_TELEMETRY
                printf("Temp   : %f\r\n", lps25h_data.temperature);
#endif
                json_object_set_number(root_object, "temperature_lps25h", (((lps25h_data.temperature / 5) * 9) + 32));
            }

            if (lps25h_data.isPressureValid)
            {
#ifdef DEBUG_TELEMETRY
                printf("Humid  : %f\r\n", lps25h_data.pressure);
#endif
                json_object_set_number(root_object, "pressure", lps25h_data.pressure);
            }
        }

        if (lsm9ds1_read(senseHat->rtimu, &accel, &gyro, &compass, &fusion) >= 0)
        {
            if (fusion.isValid)
            {
                json_object_dotset_number(root_object, "imu.yaw", fusion.yaw);
                json_object_dotset_number(root_object, "imu.roll", fusion.roll);
                json_object_dotset_number(root_object, "imu.pitch", fusion.pitch);
#ifdef DEBUG_TELEMETRY
                printf("Fusion : R %d P %d Y %d\r\n", fusion.roll, fusion.pitch, fusion.yaw);
#endif
            }

            if (accel.isValid)
            {
                json_object_dotset_number(root_object, "lsm9ds1_accelerometer.x", accel.x);
                json_object_dotset_number(root_object, "lsm9ds1_accelerometer.y", accel.y);
                json_object_dotset_number(root_object, "lsm9ds1_accelerometer.z", accel.z);
            }

            if (gyro.isValid)
            {
                json_object_dotset_number(root_object, "lsm9ds1_gyroscope.x", gyro.x);
                json_object_dotset_number(root_object, "lsm9ds1_gyroscope.y", gyro.y);
                json_object_dotset_number(root_object, "lsm9ds1_gyroscope.z", gyro.z);
            }

            if (compass.isValid)
            {
                json_object_dotset_number(root_object, "lsm9ds1_compass.x", compass.x);
                json_object_dotset_number(root_object, "lsm9ds1_compass.y", compass.y);
                json_object_dotset_number(root_object, "lsm9ds1_compass.z", compass.z);
            }

        }

        if (json_object_get_count(root_object) > 0)
        {
#ifdef DEBUG_TELEMETRY
            printf("%s\r\n", json_serialize_to_string_pretty(root_value));
//            puts(serialized_string);
#endif
            serialized_string = json_serialize_to_string(root_value);

            if (appContext->isConnected)
            {
                sendMessage(deviceClient, serialized_string);
            }
            json_free_serialized_string(serialized_string);
        }

        json_value_free(root_value);
    }
}