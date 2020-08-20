# IoT Plug and Play Sample Code in C

This sample is for Raspberry Pi and SenseHat written in in C

## Requirements

- Raspbery Pi 3 or 4
- SenseHat
- Raspbian  
    Raspbian Buster Lite May 2020 version
    <https://downloads.raspberrypi.org/raspios_lite_armhf_latest>

## Setting up Raspberry Pi 4

1. Install Raspbian OS  
    [Installation Guide](https://www.raspberrypi.org/documentation/installation/installing-images/README.md)

1. Add an empty file `ssh` in the root of `boot` partition  

    This will enable SSH automatically, so you do not need keyboard/mouse etc.

1. Configure Raspberry Pi 4  

    Run following commands to configure the Raspberry Pi 4

    ```bash
    sudo apt-get update && \
    sudo apt-get upgrade -y && \
    sudo apt-get install -y git && \
    sudo raspi-config nonint do_memory_split 16 && \
    sudo raspi-config nonint do_spi 0 && \
    sudo raspi-config nonint do_i2c 0 && \
    sudo raspi-config nonint do_wifi_country US && \
    sudo raspi-config nonint do_change_locale en_US.UTF-8 && \
    sudo raspi-config nonint do_configure_keyboard us && \
    sudo reboot now
    ```

    This script configures Raspberry Pi 4 with following configurations

    - Sets memory allocated for GPU to be 16GB
    - Enables I2C and SPI
    - Sets WiFi country to US  
        Change country code if you are not in the U.S.  
        To see other country codes, run :

        ```bash
        cat /usr/share/zoneinfo/iso3166.tab
        ```

    - Sets system locale to US

        To see other country codes, run :

        ```bash
        cat /usr/share/i18n/SUPPORTED
        ```

    - Sets keyboard layout to US

1. Upon reboot, clone this repo with :

    ```bash
    cd ~
    git clone https://github.com/daisukeiot/RP4-SenseHat-PnP.git
    ```
