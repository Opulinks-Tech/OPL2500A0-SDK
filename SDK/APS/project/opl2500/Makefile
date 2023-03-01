##### ARM GCC Toolchain Development #####
ROOT = ../../..
OUTPUT_FOLDER = $(ROOT)/APS/targets/opl2500/Output-GCC
LD_SCRIPT = link.ld

# Output file names
OUTPUT = opl2500_app_aps

# Compiler & Linker
CROSS_COMPILE = arm-none-eabi-
CC      = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
AR      = $(CROSS_COMPILE)ar


############# Ram mapping ############
APS_PATCH_START = 0x412454
APS_HEAP_START = 0x443000
APS_RAM_END = 0x454000

############# include SRC ############
ifneq ("$(wildcard ./patch_src.mk)", "")
    SRC_EXIST = 1
    include ./patch_src.mk
else
    SRC_EXIST = 0
endif
include ./sdk_src.mk

PATCH_LIB = $(OUTPUT_FOLDER)/opl2500_patch_aps.a
WPAS_LIB = $(ROOT)/APS/middleware/third_party/wpa_supplicant/project/lib/wpa_supplicant.a
SDK_LIB   = $(OUTPUT_FOLDER)/opl2500_sdk_aps.a
MAIN_LIB  = $(OUTPUT_FOLDER)/$(OUTPUT).a
CHKRGN_LIB = $(OUTPUT_FOLDER)/$(OUTPUT)_chkrgn.a


#############  CFLAGS   ##############

# Options for specific architecture
DEFS  = -DGCC
DEFS += -DARMCM4_FP
DEFS += -D__VFP_FP__
DEFS += -DOPL2500_APS
DEFS += -DOPL2500_PATCH
DEFS += -D__noSIMULATOR__
DEFS += -D__BLE__
DEFS += -D__LE_HOST_USE_CMD__
DEFS += -D__LWIP_TASK__
DEFS += -D__WIFI_MAC_TASK__
DEFS += -D__PMP_ENABLE__
DEFS += -D__PMP_REGION__
DEFS += -D__HEAP_REGION__
DEFS += -DnoLWIP_DYNAMIC_DEBUG_ENABLE
DEFS += -D__AT_CMD_TASK__
DEFS += -D__noRTL_SIMULATION__
DEFS += -DMBEDTLS_CONFIG_FILE="<config-opl-default.h>"


CFLAGS  = $(DEFS)
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += -mfloat-abi=hard
CFLAGS += -mlittle-endian
CFLAGS += -std=c99
CFLAGS += -Os
CFLAGS += -x none
CFLAGS += -include sys_common.h
CFLAGS += -nostartfiles
CFLAGS += -no-pie
CFLAGS += -fdata-sections
CFLAGS += -ffunction-sections
CFLAGS += -specs=nosys.specs
CFLAGS += -Xlinker "--just-symbols=$(ROOT)/APS/targets/opl2500/object/symdefs_gcc.o"
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-int-conversion

LFLAGS  = -T $(LD_SCRIPT)
LFLAGS += -Wl,--defsym=LD_APS_PATCH_START=$(APS_PATCH_START)
LFLAGS += -Wl,--defsym=LD_APS_HEAP_START=$(APS_HEAP_START)
LFLAGS += -Wl,--defsym=LD_APS_RAM_END=$(APS_RAM_END)
LFLAGS += -Wl,--gc-sections
#LFLAGS += -Wl,--print-memory-usage
LFLAGS += -Wl,--entry=__Patch_EntryPoint
LFLAGS += -Wl,--start-group $(PATCH_LIB) $(SDK_LIB) $(MAIN_LIB)
LFLAGS += -Wl,--end-group
LFLAGS += -Wl,-Map=$(OUTPUT_FOLDER)/$(OUTPUT).map


LFLAGS_CHKRGN  = -T $(LD_SCRIPT)
LFLAGS_CHKRGN += -Wl,--defsym=LD_APS_PATCH_START=$(APS_PATCH_START)
LFLAGS_CHKRGN += -Wl,--defsym=LD_APS_HEAP_START=$(APS_HEAP_START)
LFLAGS_CHKRGN += -Wl,--defsym=LD_APS_RAM_END=$(APS_RAM_END)
LFLAGS_CHKRGN += -Wl,--print-memory-usage
LFLAGS_CHKRGN += -Wl,--entry=__Patch_EntryPoint
LFLAGS_CHKRGN += -Wl,--start-group $(PATCH_LIB) $(SDK_LIB) $(CHKRGN_LIB)
LFLAGS_CHKRGN += -Wl,--end-group
LFLAGS_CHKRGN += -Wl,-Map=$(OUTPUT_FOLDER)/$(OUTPUT)_chkrgn.map


#############   include    ##############
INCDIR  = -I $(ROOT)/APS/project/opl2500/boot_sequence
INCDIR += -I $(ROOT)/APS/project/opl2500/include
INCDIR += -I $(ROOT)/APS/project/opl2500/lib
INCDIR += -I $(ROOT)/APS/project/opl2500/startup
INCDIR += -I $(ROOT)/APS/project/common
INCDIR += -I $(ROOT)/APS/FreeRtos
INCDIR += -I $(ROOT)/APS/FreeRtos/Source/include
INCDIR += -I $(ROOT)/APS/FreeRtos/Source/portable/GCC/ARM_CM4F
INCDIR += -I $(ROOT)/APS/driver/CMSIS/Device/Opulinks/opl2500/Include
INCDIR += -I $(ROOT)/APS/driver/CMSIS/Include
INCDIR += -I $(ROOT)/APS/driver/chip
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/camera
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_auxadc
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_dbg_uart
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_dma
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_gpio
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_i2c
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_patch
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_pdm
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_pwm
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_spi
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_system
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_tick
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_tmr
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_uart
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_vic
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/hal_wdt
INCDIR += -I $(ROOT)/APS/driver/chip/opl2500/securityipdriver
INCDIR += -I $(ROOT)/APS/middleware/netlink
INCDIR += -I $(ROOT)/APS/middleware/netlink/at
INCDIR += -I $(ROOT)/APS/middleware/netlink/ble_controller_layer/inc
INCDIR += -I $(ROOT)/APS/middleware/netlink/cli
INCDIR += -I $(ROOT)/APS/middleware/netlink/common/sys_api
INCDIR += -I $(ROOT)/APS/middleware/netlink/common/sys_api/rom_if
INCDIR += -I $(ROOT)/APS/middleware/netlink/common/sys_ctrl
INCDIR += -I $(ROOT)/APS/middleware/netlink/controller_task
INCDIR += -I $(ROOT)/APS/middleware/netlink/data_flow
INCDIR += -I $(ROOT)/APS/middleware/netlink/diag_task
INCDIR += -I $(ROOT)/APS/middleware/netlink/le_stack
INCDIR += -I $(ROOT)/APS/middleware/netlink/msg
INCDIR += -I $(ROOT)/APS/middleware/netlink/mw_fim
INCDIR += -I $(ROOT)/APS/middleware/netlink/mw_ota
INCDIR += -I $(ROOT)/APS/middleware/netlink/ps_task
INCDIR += -I $(ROOT)/APS/middleware/netlink/wifi_controller_layer
INCDIR += -I $(ROOT)/APS/middleware/netlink/wifi_mac
INCDIR += -I $(ROOT)/APS/middleware/netlink/wifi_mac/common
INCDIR += -I $(ROOT)/APS/middleware/netlink/wifi_mac/utils
INCDIR += -I $(ROOT)/APS/middleware/third_party/lwip-2.0.3
INCDIR += -I $(ROOT)/APS/middleware/third_party/lwip-2.0.3/lwip/src/include
INCDIR += -I $(ROOT)/APS/middleware/third_party/lwip-2.0.3/ports/freertos/include
INCDIR += -I $(ROOT)/APS/middleware/third_party/lwip-2.0.3/ports/freertos/netif
INCDIR += -I $(ROOT)/APS/middleware/third_party/lwip-2.0.3/ports/rom_if
INCDIR += -I $(ROOT)/APS/middleware/third_party/lwip-2.0.3/contrib/apps
INCDIR += -I $(ROOT)/APS/middleware/third_party/mbedtls/configs
INCDIR += -I $(ROOT)/APS/middleware/third_party/mbedtls/include
INCDIR += -I $(ROOT)/APS/middleware/third_party/mbedtls/include/mbedtls
INCDIR += -I $(ROOT)/APS/middleware/third_party/mbedtls/port/include
INCDIR += -I $(ROOT)/APS/middleware/third_party/wpa_supplicant
INCDIR += -I $(ROOT)/APS/middleware/third_party/wpa_supplicant/src
INCDIR += -I $(ROOT)/APS/middleware/third_party/wpa_supplicant/include
INCDIR += -I $(ROOT)/APS/middleware/third_party/wpa_supplicant/port/include
INCDIR += -I $(ROOT)/APS/middleware/third_party/tinycrypt/include
INCDIR += -I $(ROOT)/APS/middleware/third_party/tinycrypt/include/tinycrypt
INCDIR += -I $(ROOT)/APS/apps/le_app/cmd_app
INCDIR += -I $(ROOT)/APS/apps/le_app/pts_app
INCDIR += -I $(ROOT)/APS_PATCH/project/opl2500/boot_sequence
INCDIR += -I $(ROOT)/APS_PATCH/project/opl2500/include
INCDIR += -I $(ROOT)/APS_PATCH/project/opl2500/lib
INCDIR += -I $(ROOT)/APS_PATCH/project/opl2500/startup
INCDIR += -I $(ROOT)/APS_PATCH/project/common
INCDIR += -I $(ROOT)/APS_PATCH/FreeRtos
INCDIR += -I $(ROOT)/APS_PATCH/FreeRtos/Source/include
INCDIR += -I $(ROOT)/APS_PATCH/FreeRtos/Source/portable/GCC/ARM_CM4F
INCDIR += -I $(ROOT)/APS_PATCH/driver/CMSIS/Device/Opulinks/opl2500/Include
INCDIR += -I $(ROOT)/APS_PATCH/driver/CMSIS/Include
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/camera
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_auxadc
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_dbg_uart
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_dma
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_gpio
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_i2c
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_patch
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_pdm
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_pwm
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_spi
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_system
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_tick
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_tmr
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_uart
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_vic
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_wdt
INCDIR += -I $(ROOT)/APS_PATCH/driver/chip/opl2500/securityipdriver
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/at
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/ble_controller_layer/inc
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/cli
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/common/sys_api
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/common/sys_api/rom_if
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/common/sys_ctrl
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/controller_task
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/data_flow
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/diag_task
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/le_stack
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/le_stack/patch
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/msg
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/mw_fim
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/mw_ota
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/ps_task
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/wifi_controller_layer
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/wifi_mac
INCDIR += -I $(ROOT)/APS_PATCH/middleware/netlink/wifi_mac/common
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/lwip/src/include
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/ports/freertos/include
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/ports/freertos/netif
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/ports/rom_if
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/contrib/apps
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/mbedtls/configs
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/mbedtls/include
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/mbedtls/include/mbedtls
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/mbedtls/port/include
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/tinycrypt/include
INCDIR += -I $(ROOT)/APS_PATCH/middleware/third_party/tinycrypt/include/tinycrypt
INCDIR += -I $(ROOT)/APS_PATCH/apps/le_app/cmd_app
INCDIR += -I $(ROOT)/APS_PATCH/apps/le_app/include
INCDIR += -I $(ROOT)/APS_PATCH/apps/le_app/pts_app


#############   source    ##############
M_SRC = $(ROOT)/APS_PATCH/project/opl2500/startup/main_patch.o


#############   build   ##############
all: patch sdk $(M_SRC) check_region
	@rm -f $(MAIN_LIB)
	@$(AR) -r $(MAIN_LIB) $(M_SRC)
	@$(CC) -o $(OUTPUT_FOLDER)/$(OUTPUT).elf $(LFLAGS) $(CFLAGS) $(INCDIR)
	@$(OBJCOPY) -O binary --only-section=.aps_* $(OUTPUT_FOLDER)/$(OUTPUT).elf $(OUTPUT_FOLDER)/opl2500_ram1_EntryPoint.bin
	@echo Generate opl2500_ram1_EntryPoint.bin
	@$(OBJCOPY) -O binary --only-section=.shm_* $(OUTPUT_FOLDER)/$(OUTPUT).elf $(OUTPUT_FOLDER)/opl2500_ram2_ShmRegion.bin
	@echo Generate opl2500_ram2_ShmRegion.bin
	@$(OBJCOPY) -O binary --only-section=.part1_* $(OUTPUT_FOLDER)/$(OUTPUT).elf $(OUTPUT_FOLDER)/opl2500_ram3_PartMem.bin
	@echo Generate opl2500_ram3_PartMem.bin
	@$(OBJCOPY) -O binary --only-section=.cache_* $(OUTPUT_FOLDER)/$(OUTPUT).elf $(OUTPUT_FOLDER)/opl2500_ram4_CacheRam.bin
	@echo Generate opl2500_ram4_CacheRam.bin


%.o: %.c
	@$(CC) -c $(CFLAGS) $(INCDIR) $^ -o $@
	@echo Build $@

sdk: $(SDK_SRC)
	@rm -f $(SDK_LIB)
	@$(AR) -rT $(SDK_LIB) $(SDK_SRC) $(WPAS_LIB)



ifeq ("$(SRC_EXIST)","1")
    patch: prepare $(PATCH_SRC)
		@mkdir -p $(OUTPUT_FOLDER)
		@rm -f $(PATCH_LIB)
		@$(AR) -r $(PATCH_LIB) $(PATCH_SRC)
    check_region:
		@$(AR) -r $(CHKRGN_LIB) $(M_SRC)
		@$(CC) -o $(OUTPUT_FOLDER)/$(OUTPUT)_chkrgn.elf $(LFLAGS_CHKRGN) $(CFLAGS) $(INCDIR)
		@rm -f $(OUTPUT_FOLDER)/$(OUTPUT)_chkrgn.a
		@rm -f $(OUTPUT_FOLDER)/$(OUTPUT)_chkrgn.elf
else
    patch:
    check_region:
endif

prepare:
	$(ROOT)/scripts/SubWCRev.exe $(ROOT) $(ROOT)/APS/project/common/svn_rev_patch.txt $(ROOT)/APS/project/common/svn_rev_patch.h

#############   clean   ##############
clean:
ifeq ("$(SRC_EXIST)","1")
	@rm -f $(M_SRC)
	@rm -f $(SDK_SRC)
	@rm -f $(PATCH_SRC)
	@rm -f $(OUTPUT_FOLDER)/*
else
	@rm -f $(M_SRC)
	@rm -f $(OUTPUT_FOLDER)/$(OUTPUT).*
endif

