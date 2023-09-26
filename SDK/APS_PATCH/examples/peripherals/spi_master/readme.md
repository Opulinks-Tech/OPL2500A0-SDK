# Function
This example introduces how to set SPI master mode.

In this demo, SPI_3 was used to issue the command to query device ID.
    - The pin-assignment:
        - IO_08: SPI3_CS0
        - IO_09: SPI3_CLK
        - IO_16: SPI3_RXD1
        - IO_17: SPI3_TXD0

## FIFO test flow
1. Master and slave sync
2. Sync done, run next step
   Otherwise, delay a while and repeat step 1.
3. Master send data and receive data.
   If DMA enabled, the data will be TX/RX by DMA.
4. Dump TX and RX data
5. Wait 5 seconds
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
- An external SPI slave device is required.
- In example.h, set TEST_DMA_EN to 1 to enable demo SPI master mode with DMA
  Set TEST_DMA_EN to 0 will use FIFO mode to send and receive data.