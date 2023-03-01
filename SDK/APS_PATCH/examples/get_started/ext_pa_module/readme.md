# Function
This example shows how to enable ext-PA mode in ext-PA module.

In this demo,
Do the two steps to active ext-Pa mode:
    - 1) Config " hal_pin_config_project.h "
         Assigned three pins (TX_EN/RX_EN/LNA_EN) according to schematic.
         The pins are MUST assinged to PIN_TYPE_GPIO_OUT_LOW.
    - 2) Set Hal_ExtPa_Pin_Set(), default value was disable.

# Notes
1. The ext-PA pins in the official module are:
    TX_EN : GPIO_04
    RX_EN : GPIO_06
    LNA_EN: GPIO_18

