#############   SDK source    ##############
#Source
SDK_SRC  = $(ROOT)/APS_PATCH/project/opl2500/startup/sys_init_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/project/opl2500/startup/irq_patch.o
#Driver
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_system/hal_system_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_auxadc/hal_auxadc_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_spi/hal_flash_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_spi/hal_qspi_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_system/hal_sys_rcc_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_gpio/hal_pin_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_vic/hal_vic_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_uart/hal_uart_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/driver/chip/opl2500/hal_dma/hal_dma_patch.o
#lwip
SDK_SRC += $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/lwip_helper_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/ports/rom_if/lwip_jmptbl_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/lwip/src/core/memp_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/lwip/src/core/pbuf_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/middleware/third_party/lwip-2.0.3/ports/freertos/netif/wlannetif_patch.o
#http_client
SDK_SRC += $(ROOT)/APS/middleware/third_party/httpclient/httpclient.o
#mw_fim
SDK_SRC += $(ROOT)/APS_PATCH/middleware/netlink/mw_fim/mw_fim_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/middleware/netlink/mw_fim/mw_fim_default_group01_patch.o
SDK_SRC += $(ROOT)/APS_PATCH/middleware/netlink/mw_fim/mw_fim_default_group02_patch.o
#common
SDK_SRC += $(ROOT)/APS_PATCH/middleware/netlink/common/sys_ctrl/sys_common_ctrl_patch.o
