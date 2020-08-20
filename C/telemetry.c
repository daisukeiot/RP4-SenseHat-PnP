#include "telemetry.h"

extern bool isConnected;

int processTelemetry(IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceClientLL, SENSEHAT_DATA* senseHat)
{
    HTS221_DATA hts221_data;
    LPS25H_DATA lps25h_data;
    ORIANTATION_DATA gyro;
    ORIANTATION_DATA accel;
    ORIANTATION_DATA compass;
    FUSION_DATA fusion;
    char str[40];
    float float2decimal;

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;

    if (senseHat)
    {
        if (hts221_read(senseHat->hts221, &hts221_data) >= 0)
        {
            if (hts221_data.isTemperatureValid)
            {
#ifdef DEBUG_TELEMETRY
                printf("Temp   : %f\r\n", hts221_data.temperature);
#endif
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
//#ifdef DEBUG_TELEMETRY
            printf("%s\r\n", json_serialize_to_string_pretty(root_value));
//            puts(serialized_string);
//#endif
            serialized_string = json_serialize_to_string(root_value);

            if (isConnected)
            {
                sendMessage(deviceClientLL, serialized_string);
            }
            json_free_serialized_string(serialized_string);
        }

        json_value_free(root_value);
    }
}