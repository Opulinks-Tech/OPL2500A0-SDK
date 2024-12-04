# Function
This example shows how to enable XIP on external PSRAM
On Psram, there are XIP content, data section, and PSRAM heap partition.
This XIP original content is within OTA image, and it will be copied to PSRAM when initialization.

# Work Flow
- Add define `OPL2500_XIP` and `OPL2500_PSRAM` to project
- A counter is inceasing  in Thread1 and use this value to get an 16-bits value to send to message queue. 
  When thread2 received message , use the 16-bits value as buffer length to allocate memory in PSRAM heap (PHEAP).
  There are DEMO_PHEAP_ALLOC_WINDOW_SIZE pointers array to store the allocated buffer.
  Before allocate PHEAP, if the next buffer is not NULL, free the buffer and allocate new buffer then store to the pointer array.

# Notes
1. Thread and message working mechanism is based on CMSIS RTOS.  

2. Enable XIP on external PSRAM mode
   
   In example project
     - Add Add define `OPL2500_XIP` to project
     - Add PHEAP init function to __Patch_EntryPoint.
        osMemoryPHeapMallocMixModeEn is to enable `malloc` function automatically use PHEAP when HEAP not available.
        ```c
        /* PSRAM initialization */
        osPHeapPatchInit();
        osMemoryPHeapMallocMixModeEn(EN_MALLOC_AUTO_PHEAP);       /* Select to PHEAP mix mode or not */
        /* Assign PHEAP */
        vPortPHeapRegionInit((uint8_t* )APS_PHEAP_START, APS_PHEAP_LENGTH);
        
        if (!Boot_CheckWarmBoot())
        {
            Main_PinMuxUpdate_flash(); /* Init flash pin-mux first */
        }
        Sys_XipSetup(XIP_MODE_EXT_PSRAM, SPI_SLAVE_2, 0);
        memset(ZI_REGION_PSRAM_START, 0, ZI_REGION_PSRAM_LENGTH);   /* Init PSRAM ZI region */
        ```
      - Update link.ld to XIP version
   Download
     - Using PatchData_XIP.txt to pack APS and MSQ binary files except XIP binary file.
     - Download application binary file

3. Internal flash and external PSRAM use the same SPI hardware.
    When flash is erasing and polling status, CPU will be blocked when access XIP content.
    And recover after polling status finished.
    
    
4. PSRAM size definition in opl2500_app_aps.sct or Makefile
    | Defination        | Description                       |
    | ----------------- | --------------------------------- |
    | `APS_XIP_SIZE`    | The external PSRAM size (bytes)   |

    - PSRAM layout
        Total size: `APS_XIP_SIZE`

    | External PSRAM | Length            |
    | -------------- | ----------------- |
    | XIP (RO)       | RO region         |
    | Data (RW, ZI)  | RW and ZI region  |
    | HEAP           | Remaining part    |
