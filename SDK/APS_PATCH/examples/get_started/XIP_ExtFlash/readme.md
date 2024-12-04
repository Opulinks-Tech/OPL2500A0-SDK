# Function
This example shows how to enable XIP on external flash
On external flash, there are two XIP OTA image and user data section.
The layout of XIP content needs to pre-defined in Boot Agent and SDK.

# Work Flow
- Add define `OPL2500_XIP` and `OPL2500_EXT_FLASH` to project
- Boot Agent also needs to build specific image.


# Notes
1. Thread and message working mechanism is based on CMSIS RTOS.  

2. Enable XIP on external Flash mode
   
   In example project
     - Add define `OPL2500_XIP` and `OPL2500_EXT_FLASH` to project
     - Add following to __Patch_EntryPoint, must called before SysInit_EntryPoint
        ```c
        if (!Boot_CheckWarmBoot())
        {
            Main_PinMuxUpdate_flash(); /* Init flash pin-mux first */
        }
        Sys_XipSetup(XIP_MODE_EXT_FLASH, SPI_SLAVE_1, 0);
        ```
     -  Update link.ld to XIP version
   Download
     - Using PatchData_XIP.txt to pack APS and MSQ binary files except XIP binary file.
     - Download application binary file
3. Boot Agent project
   - Add define `OPL2500_EXT_FLASH` to project
   - Update `MW_OTA_EXT_IMAGE_ADDR_PATCH_START` and `MW_OTA_EXT_IMAGE_SIZE_PATCH` if external flash start and size changed.
        at *mw_ota_def_patch.h*
        ```c
        #define MW_OTA_EXT_IMAGE_ADDR_PATCH_START   0x00000000
        #define MW_OTA_EXT_IMAGE_SIZE_PATCH         0x00080000
        ```
        MW_OTA_EXT_IMAGE_SIZE_PATCH shall be equal to `APS_XIP_SIZE`
   - Update pinmux configuration
        at *main_patch.c* in Boot Agent project
        ```c
        #ifdef OPL2500_EXT_FLASH
        void BA_PinmuxUpdate(void)
        {
            /* Update Pinmux for external flash here */
            Hal_Pin_Config(PIN_TYPE_SPI0_CS2_IO9  | PIN_DRVCRNT_IO9_4mA | PIN_INMODE_IO9_FLOATING);
            Hal_Pin_Config(PIN_TYPE_SPI0_IO2_IO10 | PIN_DRVCRNT_IO10_8mA | PIN_INMODE_IO10_PULL_UP);
            Hal_Pin_Config(PIN_TYPE_SPI0_IO3_IO11 | PIN_DRVCRNT_IO11_8mA | PIN_INMODE_IO11_PULL_UP);
            Hal_Pin_Config(PIN_TYPE_SPI0_CLK_IO13 | PIN_DRVCRNT_IO13_8mA | PIN_INMODE_IO13_FLOATING);
            Hal_Pin_Config(PIN_TYPE_SPI0_IO0_IO14 | PIN_DRVCRNT_IO14_8mA | PIN_INMODE_IO14_FLOATING);
            Hal_Pin_Config(PIN_TYPE_SPI0_IO1_IO15 | PIN_DRVCRNT_IO15_8mA | PIN_INMODE_IO15_FLOATING);
        }
        #endif /* OPL2500_EXT_FLASH */
        ```
    - Build Boot Agent image for external flash application

4. Internal flash and external flash use the same SPI hardware.
    When flash is erasing and polling status, CPU will be blocked when access XIP content.
    And recover after polling status finished.
    
    
5. External flash size definition in opl2500_app_aps.sct or Makefile
    | Defination        | Description                                            |
    | ----------------- | ------------------------------------------------------ |
    | `APS_XIP_SIZE`    | The OTA image size for XIP on external flash (bytes)   |

    - External flash layout
        Total size: `APS_XIP_SIZE`*2 + remaining size

    | External Flash | Length            |
    | -------------- | ----------------- |
    | OTA Img0 XIP   | `APS_XIP_SIZE`    |
    | OTA Img1 XIP   | `APS_XIP_SIZE`    |
    | User data      | Remaining part    |

6. Example for external flash layout
   External flash size = 32Mbits (4MB)
   Case 1: Set OTA image XIP size as 2MB.
           (`APS_XIP_SIZE` = 0x200000)
           On external flash, there are only two OTA image blocks.
             | External Flash | Length |
             | -------------- | ------ |
             | OTA Img0 XIP   |  2MB   |
             | OTA Img1 XIP   |  2MB   |
   Case 2: Set OTA image XIP size as 1.5MB
           (`APS_XIP_SIZE` = 0x180000)
           On external flash, there are two OTA image blocks and user data.
             | External Flash | Length |
             | -------------- | ------ |
             | OTA Img0 XIP   |  1.5MB |
             | OTA Img1 XIP   |  1.5MB |
             | User data      |  1 MB  |

## FIM Zones must be on the same flash.
   i.e. It is not valid for ZONE0 in internal flash, and ZONE1 in external flash.

