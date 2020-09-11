# Raspberry Pi Sample in C for IoT Plug and Play 

## Requirements

- [Raspberry Pi 4](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/) or Raspberry Pi 3
- [SenseHat](https://www.raspberrypi.org/products/sense-hat/)
- [Raspbian Buster](https://www.raspberrypi.org/downloads/raspberry-pi-os/)

## Compile

Run command below to compile the sample code

```bash
sudo apt-get install -y git cmake build-essential curl libcurl4-openssl-dev libssl-dev uuid-dev i2c-tools libi2c-dev librtimulib-dev && \
cd ~/RP4-SenseHat-PnP/C && \
git clone --depth 1 --recurse-submodules https://github.com/Azure/azure-iot-sdk-c.git 
mkdir cmake
cd cmake
cmake .. -Dskip_samples=ON -Dhsm_type_symm_key=ON -Dbuild_service_client=OFF -Dbuild_provisioning_service_client=OFF -Duse_prov_client=ON -Dhsm_type_symm_key=ON
cmake --build .
```

## Running the app

Specify DPS parameters or IoT Hub Connection string through environmental variable.  DPS provisioning will be prioritized over IOT Hub Connection String.

Example :

```bash
export DPS_SCOPEID='<DPS Scope ID>'
export DPS_REGISTRATIONID='<DPS Registration ID>'
export DPS_DEVICEKEY='<DPS Symmetric Key>'
export IOTHUB_CS='<IoT Hub Connection String>'
export PNP_MODEL_ID='<PnP Model ID>'
```

