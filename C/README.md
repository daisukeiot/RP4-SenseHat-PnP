# Raspberry Pi Sample in C for IoT Plug and Play 

## Compile

Run command below to compile the sample code

```bash
sudo apt-get install -y git cmake build-essential curl libcurl4-openssl-dev libssl-dev uuid-dev i2c-tools libi2c-dev librtimulib-dev && \
cd ~/RP4-SenseHat-PnP/C && \
git clone --depth 1 --recurse-submodules https://github.com/Azure/azure-iot-sdk-c.git 
mkdir cmake
cd cmake
cmake .. -Dskip_samples=ON -Dhsm_type_symm_key=ON -Dbuild_service_client=OFF -Dbuild_provisioning_service_client=OFF -Duse_http=OFF -Duse_amqp=OFF
```

## Running the app

Set IoT Hub Connection String to `IOTHUB_CS` environment variable :

Example :

```bash
export IOTHUB_CS='<Device Connection String>'
```

## RTIMULib

This sample uses RTIMULib from :

https://github.com/RPi-Distro/RTIMULib.git
