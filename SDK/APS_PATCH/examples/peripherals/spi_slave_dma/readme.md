# Function
This example introduces how to set SPI slave mode.

In this demo, SPI_2 is initialized as SPI slave.
    - The pin-assignment:
        - IO_25: SPI2_CS
        - IO_24: SPI2_CLK
        - IO_28: SPI2_RX
        - IO_29: SPI2_TX
        
        - IO_9 : Sync (Output)
        
## FIFO test flow
1. Master and slave sync
2. TX/RX by DMA.


## Demo
    User can use OPL2500 SPI master mode example to demo.
    |      | Master | Slave |
    | ---- | ------ | ----- |
    | CS   | IO8    | IO25  |
    | CLK  | IO9    | IO24  |
    | MOSI | IO17   | IO28  |
    | MISO | IO16   | IO29  |
    | Sync | IO7    | IO9   |


# Notes
- An external SPI master device is required.
- The DesignWare SPI hardware limitation
    ```
    If the slave device is receive only, the minimum frequency of ssi_clk is six times the maximum expected 
    frequency of the bit-rate clock from the master device (sclk_in). The sclk_in signal is double synchronized to 
    the ssi_clk domain, and then it is edge detected.
    If the slave device is transmit and receive, the minimum frequency of ssi_clk is 12 times the maximum 
    expected frequency of the bit-rate clock from the master device (sclk_in). This minimum frequency is to 
    ensure that data on the master's rxd line is stable before the master's shift control logic captures the data. 
    ```
    - Clock definition
        - F_ssi_clk: The SPI hardware peripheral clock
          Configuration API: `Hal_Sys_Spi2SrcSelect`
        - F_sclk_in: The SPI clock transmitted by SPI master
        
    - SPI slave is receive-only mode
        F_ssi_clk >= 6 x (max F_sclk_in)
        If SPI master transmits at 10MHz SPI clock, SPI slave peripheral clock must be 60 MHz or above.

   - SPI slave is transmit-receive mode
        F_ssi_clk >= 12 x (max F_sclk_in)
        If SPI master transmits at 10MHz SPI clock, SPI slave peripheral clock must be 120 MHz or above.
