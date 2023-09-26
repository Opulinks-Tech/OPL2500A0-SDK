# Function
This example shows how to enable XIP OTA bundle mode and assign functions or read only data to XIP region

# Work Flow

- Add define `OPL2500_XIP` to project
- A counter is inceasing  in Thread1 and its value is send to message queue. Thread2 received message , get counter's value and print it out.  

# Notes
1. Thread and message working mechanism is based on CMSIS RTOS.  

2. Enable XIP OTA bundle mode
   In example project
     - Add Add define `OPL2500_XIP` to project
     - Add following to __Patch_EntryPoint, must called before SysInit_EntryPoint
        ```c
        Main_PinMuxUpdate(); /* Init flash pin-mux first */
        Sys_XipSetup(XIP_MODE_OTA_BUNDLE, SPI_SLAVE_0, 0);
        ```
   Download
     - Using PatchData_XIP.txt to pack APS and MSQ binary files. XIP binary file is included.
     - Download application binary file
