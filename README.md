# Improved usb cdc interface for stm32

### Advantages
- Same API as provided as by ST but with additional and improved functionality
- const correct
- easy to use buffer management for sending and receiving data
  - achieves up to 1MB/s (USB FS) when transmitting to Linux host (MB = 10^6Byte) (tested on STM32F4 with fcpu > 100MHz)
  - functions to check if packets were dropped both for transmission and reception

### How to use
- replace `USB_DEVICE/App/usbd_cdc_if.c` and `USB_DEVICE/App/usbd_cdc_if.h` with the ones provided in the repository
  - adjust  `APP_RX_DATA_SIZE` and `APP_TX_DATA_SIZE` if needed

### TODO
- transmitting data from interrupt currently not allowed
- check if the physical limit of 4096 bytes for the USB packet area really applies to all devices
- generalize for USB HS
- volatile variables not optimal
