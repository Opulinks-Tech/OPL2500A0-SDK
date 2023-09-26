# Function
This example introduces how to set SPI slave mode.

In this demo, SPI_2 was initialized as SPI slave and keep receiving data.
    - The pin-assignment:
        - IO_25: SPI2_CS
        - IO_24: SPI2_CLK
        - IO_28: SPI2_RX
        - IO_29: SPI2_TX
        
## FIFO test flow
1. Master and slave sync
2. Sync done, run next step
   Otherwise, delay a while and repeat step 1
3. Put TX data to FIFO and wait to receive data
   If DMA enabled, the data will be TX/RX by DMA.
4. Dump RX and TX data
5. Wait 4 seconds
6. Repeat step 1 ~ 5

## Demo
    User can use SPI slave mode example as slave to demo.
    |      | Master | Slave |
    | ---- | ------ | ----- |
    | CS   | IO8    | IO25  |
    | CLK  | IO9    | IO24  |
    | MOSI | IO17   | IO28  |
    | MISO | IO16   | IO29  |


# Notes
- An external SPI master device is required.
- In example.h, set TEST_DMA_EN to 1 to enable demo SPI slave mode with DMA
  Set TEST_DMA_EN to 0 will use FIFO mode to receive and send data.