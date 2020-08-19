sudo apt-get install -y cmake build-essential curl libcurl4-openssl-dev libssl-dev uuid-dev i2c-tools libi2c-dev librtimulib-dev && \
cd ~/RP4-SenseHat-PnP/C && \
git clone --depth 1 --recurse-submodules https://github.com/Azure/azure-iot-sdk-c.git && \
mkdir cmake && \
cd cmake && \
cmake .. -Dskip_samples=ON -Dhsm_type_symm_key=ON -Dbuild_service_client=OFF -Dbuild_provisioning_service_client=OFF -Duse_http=OFF -Duse_amqp=OFF && \
cmake --build .