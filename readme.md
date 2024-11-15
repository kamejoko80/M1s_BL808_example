# M1s_BL808_example

[Detailed document](https://wiki.sipeed.com/hardware/en/maix/m1s/other/start.html#SDK-Compile)

## Setup

1. Download sdk

    ```shell
    mkdir m1s & cd m1s
    git clone git@github.com:kamejoko80/M1s_BL808_SDK.git
    cd M1s_BL808_SDK
    git checkout henry_dev
    ```

2. Download example

    ```shell
    cd m1s
    git clone git@github.com:kamejoko80/M1s_BL808_example.git
    cd M1s_BL808_example
    git checkout henry_dev
    ```

3. Download toolchain (version 2.2.4) [here](https://dl.sipeed.com/shareURL/others/toolchain) or [here](https://occ.t-head.cn/community/download?id=3996672928124047360). Extract it to `M1s_BL808_SDK/toolchain/` and rename to  `Linux_x86_64`

    ```shell
    cd m1s
    mkdir -p M1s_BL808_SDK/toolchain
    cd M1s_BL808_SDK/toolchain
    git clone git@github.com:kamejoko80/m1s_toolchain.git
    mv m1s_toolchain Linux_x86_64
    ```

4. Export SDK path environment variables


    ```shell
    cd M1s_BL808_SDK
    export BL_SDK_PATH=$PWD
    ```

5. Build the hello_world example

    ```shell
    cd M1s_BL808_example/c906_app/

    # Compile the hello_world project
    ./build.sh hello_world
    ```

5. The compilation is complete, and the target file is saved in the `build_out` path

##  Flash Guide

1. Download flash tool and startup

    Go to [bflb_dev_website](https://dev.bouffalolab.com/download) and download `Bouffalo Lab Dev Cube` released on `2022-09-01` and with version `v1.8.0`

    ```shell
    # unzip
    unzip BouffaloLabDevCube-v1.8.0.zip

    # startup
    ./BLDevCube-v1.8.0/BLDevCube-ubuntu &> /dev/null &
    ```

2. Download bl808 firmware

    - Use the type-c usb cable to connect from the PC side to the UART port of the board

    - Keep the `BOOT` button pressed and click `RST` button, then release the `BOOT` button

    - Startup `BLDevCube Tools` and choose `BL808` chip

    - Configure the tool according to the label below, finally click `Create & Download` will start to download firmware

        ![image-20221018233328391](assets/image-20221018233328391.png)

        **Tips:**

        > If you have downloaded the e907 firmware and have an extra USB port on your PC, you can update the c906 firmware via USB. You just need connect the USB to the OTG port, press and hold the left and right buttons before the board is reset, then drag your firmware to the ejected disk and just wait for the new firmware to boot.

3. Download bl702 firmware

    - Press the `BOOT` button, and connect the type-c cable to the UART port, then release the `BOOT` button

    - Startup `BLDevCube Tools` and choose `BL702/704/706` chip

    - Configure the tool according to the label below, finally click `Create & Download` will start to download firmware

        ![image-20221018233819623](assets/image-20221018233819623.png)

4. Download e907 firmware

    - Use the type-c usb cable to connect from the PC side to the UART port of the board

    - Keep the `BOOT` button pressed and click `RST` button, then release the `BOOT` button

    - Startup `BLDevCube Tools` and choose `BL808` chip

    - Configure the tool according to the label below, finally click `Create & Download` will start to download firmware

        ![image-20221109200656](assets/image-20221109200656.jpg)

5. About partition table files

    The partition table of bl808 is written in toml file format. You can specify the flash size used by the firmware by modifying the partition table.

    The default partition table is provided in the M1s_BL808_example/partition directory, and you can also create your own partition table.
