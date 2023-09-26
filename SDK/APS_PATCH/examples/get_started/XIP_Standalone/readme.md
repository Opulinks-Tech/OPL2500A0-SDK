# Function
This example shows how to enable XIP standalone and assign functions or read only data to XIP region
This XIP content is not within OTA image, user must handle XIP binary and make sure the XIP content is matching to OTA image.

# Work Flow

- Add define `OPL2500_XIP` to project
- Update `XIP_STANDALONE_FLASH_ADDRESS` to XIP content flash address
- A counter is inceasing  in Thread1 and its value is send to message queue. Thread2 received message , get counter's value and print it out.  

# Notes
1. Thread and message working mechanism is based on CMSIS RTOS.  

2. Enable XIP standalone mode
   Select flash address for XIP context use: XIP_FLASH_ADDRESS
   
   In example project
     - Add Add define `OPL2500_XIP` to project
     - Add following to __Patch_EntryPoint, must called before SysInit_EntryPoint
        ```c
        Main_PinMuxUpdate(); /* Init flash pin-mux first */
        Sys_XipSetup(XIP_MODE_STAND_ALONE, SPI_SLAVE_0, XIP_STANDALONE_FLASH_ADDRESS);
        ```
   Download
     - Using PatchData_XIP_SA.txt to pack APS and MSQ binary files except XIP binary file.
     - Download application binary file
     - Erase XIP_FLASH_ADDRESS and required sector size
     - Donwload XIP binary file to flash, the flash address is equal to XIP_FLASH_ADDRESS
     

