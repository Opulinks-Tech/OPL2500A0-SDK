# Description
The central device will scan nearby peripherals to see if any of them provide a
service UUID that matches one we are interested in, and then connect to retrieve it.

# Function
This example demonstrates how to:
Use central role to:
    ¡EStart scanning
    ¡EGet scan result and find target service UUID (default Service UUID 0x180D, HRS)
    ¡EConnect to target peripheral
    ¡EDiscover all primary services
    ¡EDiscover all characteristic descriptors
    ¡ERead GAP service¡¦s ¡§Device name¡¨
    ¡ECharacteristic Read/Write/enable Notification operation
    ¡EDisconnect the devices, for example execute ¡§at+blediscon¡¨ on the Central,
      the central automatically reconnect and that Heart Rate measurements are displayed on the UART.

# Notes
User can use TI Btool or use Nordic "nRF connect" mobile APP to scan and connect peripheral device
to check GATT services & characteristics.
