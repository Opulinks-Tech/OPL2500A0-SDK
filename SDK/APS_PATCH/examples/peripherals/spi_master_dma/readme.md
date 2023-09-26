# Function
This example introduces how to set SPI master mode.

In this demo, SPI_3 is initialized as SPI master.
    - The pin-assignment:
        - IO_08: SPI3_CS0
        - IO_09: SPI3_CLK
        - IO_16: SPI3_RXD1
        - IO_17: SPI3_TXD0
        
        - IO_7 : Sync (Input)

## FIFO test flow
1. Master and slave sync
2. TX/RX by DMA.


## Demo
    User can use OPL2500 SPI slave mode example to demo.
    |      | Master | Slave |
    | ---- | ------ | ----- |
    | CS   | IO8    | IO25  |
    | CLK  | IO9    | IO24  |
    | MOSI | IO17   | IO28  |
    | MISO | IO16   | IO29  |
    | Sync | IO7    | IO9   |


# Notes
- An external SPI slave device is required.
- SPI_BAUDRATE is SPI physical bit rate.
  In OPL2500 SPI slave, SPI slave pclk has limitation
  - RX-only mode: SPI pclk > 6 * SPI bit rate
  - TRX mode: SPI pclk > 12 * SPI bit rate