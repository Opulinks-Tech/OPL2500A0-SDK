# Function
This example shows how to enable XIP standalone and assign functions or read only data to XIP region
This XIP content is not within OTA image, user must handle XIP binary and make sure the XIP content is matching to OTA image.

# Work Flow

- Add define `OPL2500_XIP` to project
- Update `XIP_STANDALONE_FLASH_ADDRESS` to XIP content flash address

# Notes
1. Enable XIP standalone mode
   Select flash address for XIP context use: XIP_FLASH_ADDRESS
   
   In example project
     - Add Add define `OPL2500_XIP` to project
     - Add following to __Patch_EntryPoint, must called before SysInit_EntryPoint
        ```c
        if (!Boot_CheckWarmBoot())
        {
            Main_PinMuxUpdate_flash(); /* Init flash pin-mux first */
        }
        Sys_XipSetup(XIP_MODE_STAND_ALONE, SPI_SLAVE_0, XIP_STANDALONE_FLASH_ADDRESS);
        ```
     - Update link.ld to XIP version
   Download
     - Using PatchData_XIP_SA.txt to pack APS and MSQ binary files except XIP binary file.
     - Download application binary file
     - Erase XIP_FLASH_ADDRESS and required sector size
     - Donwload XIP binary file to flash, the flash address is equal to XIP_FLASH_ADDRESS
     

