# Improved usb cdc interface for stm32

### Advantages
- Same API as provided as by ST but with additional and improved functionality
- const correct
- easy to use buffer management for sending and receiving data
  - achieves up to 1MB/s (USB FS) when transmitting to Linux host (MB = 10^6Byte) (tested on STM32F4 with fcpu > 100MHz)
  - functions to check if packets were dropped both for transmission and reception
- Attempts to avoid zero length packets (small latency vs speed tradeoff)

### How to use
- replace `USB_DEVICE/App/usbd_cdc_if.c` and `USB_DEVICE/App/usbd_cdc_if.h` with the ones provided in the repository
  - adjust  `APP_RX_DATA_SIZE` and `APP_TX_DATA_SIZE` if needed
- Send data via `CDC_Transmit_FS(const uint8_t* Buf, uint16_t Len)` or `CDC_TransmitString_FS(const char *string)`
- Receive data via `CDC_RXQueue_Dequeue(uint8_t* Dst, uint32_t MaxLen)` or by overriding `CDC_DataReceivedHandler(const uint8_t *Data, uint32_t len)`

### TODO
- transmitting data from interrupt currently not allowed
- check if the physical limit of 4096 bytes for the USB packet area really applies to all devices
- generalize for USB HS
- volatile variables not optimal
- WIP

### Tested on
- STM32F4
