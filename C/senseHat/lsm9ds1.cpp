#ifdef HAVE_RTIMULIB_H
#include "RTIMULib.h"
#include "lsm9ds1.h"

static RTIMUSettings *settings;
// static RTIMU *imu;

/*
** Initialize RTIMUlib
** Reference : https://github.com/RPi-Distro/RTIMULib
*/
extern "C" RTIMU_CONFIG* rtimu_initialize(void)
{
    RTIMU_CONFIG* config = NULL;
    RTIMUSettings *settings;
    RTIMU *imu = NULL;

    printf("RTIMULib init start\r\n");

    if ((config = (RTIMU_CONFIG*)malloc(sizeof(RTIMU_CONFIG))) == NULL)
    {
        printf("ERR : Failed to allocate memory for RTIMU_CONFIG\r\n");
    }
    else if ((settings = new RTIMUSettings("RTIMULib")) == NULL)
    {
        printf("ERR : Failed to allocate RTIMUSettings\r\n");
    }
    else
    {
        imu = RTIMU::createIMU(settings);

        if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL))
        {
            printf("No IMU found\n");
            return NULL;
        }

        imu->IMUInit();
        imu->setSlerpPower(0.02);
        imu->setGyroEnable(true);
        imu->setAccelEnable(true);
        imu->setCompassEnable(true);
        imu->setDebugEnable(false);

        config->settings = (void*)settings;
        config->imu = (void*)imu;
        config->isInitialized = true;
    }

    printf("RTIMULib init finish\r\n");

    return config;
}

extern "C" void rtimu_uninitialize(RTIMU_CONFIG* config)
{
    if (config->imu)
    {
        delete (RTIMU*) config->imu;
    }

    if (config->settings)
    {
        delete (RTIMUSettings*) config->settings;
    }

    free(config);
}

extern "C" int lsm9ds1_read(RTIMU_CONFIG* config, ORIANTATION_DATA* accel, ORIANTATION_DATA* gyro, ORIANTATION_DATA* compass, FUSION_DATA* fusion )
{
    RTIMU_DATA imuData;
    RTIMU* imu = (RTIMU *)config->imu;

    accel->isValid = false;
    gyro->isValid = false;
    compass->isValid = false;
    fusion->isValid = false;

    if (imu == NULL)
    {
        printf("ERR : IMU is Empty\r\n");
    }

    if (!imu->IMURead()) {
        printf("IMURead Fail\r\n");
        return -1;
    }

    imuData = imu->getIMUData();

    if (imuData.accelValid)
    {
        accel->x = imuData.accel.x();
        accel->y = imuData.accel.y();
        accel->z = imuData.accel.z();
        accel->isValid = true;
    }

    if (imuData.gyroValid)
    {
        gyro->x = imuData.gyro.x();
        gyro->y = imuData.gyro.y();
        gyro->z = imuData.gyro.z();
        gyro->isValid = true;
    }

    if (imuData.compassValid)
    {
        compass->x = imuData.compass.x();
        compass->y = imuData.compass.y();
        compass->z = imuData.compass.z();
        compass->isValid = true;
    }

    if (imuData.fusionPoseValid)
    {

        fusion->roll = (int) (RTMATH_RAD_TO_DEGREE * imuData.fusionPose.x());
        fusion->pitch = (int) (RTMATH_RAD_TO_DEGREE * imuData.fusionPose.y());
        fusion->yaw = (int) (RTMATH_RAD_TO_DEGREE * imuData.fusionPose.z());
        fusion->isValid = true;
    }

    return imuData.accelValid;
}

extern "C" int lsm9ds1_get_fusion(RTIMU_CONFIG* config, int *roll, int *pitch, int *yaw )
{
    RTIMU_DATA imuData;
    RTIMU* imu = (RTIMU *)config->imu;

    if (!imu->IMURead()) {
        printf("IMURead Fail\r\n");
        return -1;
    }

    imuData = imu->getIMUData();

    if (imuData.fusionPoseValid)
    {

        *roll = (int) (RTMATH_RAD_TO_DEGREE * imuData.fusionPose.x());
        *pitch = (int) (RTMATH_RAD_TO_DEGREE * imuData.fusionPose.y());
        *yaw = (int) (RTMATH_RAD_TO_DEGREE * imuData.fusionPose.z());
    }
    else
    {
        *roll = *pitch = *yaw = 0;
    }

    return imuData.fusionPoseValid;
}


extern "C" int lsm9ds1_get_accel(RTIMU_CONFIG* config, ORIANTATION_DATA* data)
{
    RTIMU_DATA imuData;
    RTIMU* imu = (RTIMU *)config->imu;

    if (!imu->IMURead()) {
        printf("IMURead Fail\r\n");
        return -1;
    }

    imuData = imu->getIMUData();

    if (imuData.accelValid)
    {
        data->x = imuData.accel.x();
        data->y = imuData.accel.y();
        data->z = imuData.accel.z();
    }
    else
    {
        data->x = data->y = data->z  = 0.0;
    }

    return imuData.accelValid;
}

extern "C" int lsm9ds1_get_compass(RTIMU_CONFIG* config, ORIANTATION_DATA* data)
{
    RTIMU_DATA imuData;
    RTIMU* imu = (RTIMU *)config->imu;

    if (!imu->IMURead()) {
        printf("IMURead Fail\r\n");
        return -1;
    }

    imuData = imu->getIMUData();

    if (imuData.compassValid)
    {
        data->x = imuData.compass.x();
        data->y = imuData.compass.y();
        data->z = imuData.compass.z();
    }
    else
    {
        data->x = data->y = data->z  = 0.0;
    }

    return imuData.compassValid;
}

extern "C" int lsm9ds1_get_gyro(RTIMU_CONFIG* config, ORIANTATION_DATA* data)
{
    RTIMU_DATA imuData;
    RTIMU* imu = (RTIMU *)config->imu;

    if (!imu->IMURead()) {
        printf("IMURead Fail\r\n");
        return -1;
    }

    imuData = imu->getIMUData();

    if (imuData.gyroValid)
    {
        data->x = imuData.gyro.x();
        data->y = imuData.gyro.y();
        data->z = imuData.gyro.z();
    }
    else
    {
        data->x = data->y = data->z  = 0.0;
    }

    return imuData.gyroValid;
}
#endif

