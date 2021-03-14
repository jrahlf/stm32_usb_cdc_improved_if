# Improved usb cdc interface for stm32

### Advantages
- Same API as provided as by ST but with additional and improved functionality
- const correct
- easy to use with automatic buffer management for sending and receiving data
  - achieves up to 1MB/s (USB FS) when transmitting to Linux host (tested on STM32F4 with fcpu > 100MHz)
- Added functionality: 
  - functions to check if packets were dropped both for transmission and reception
  - `CDC_IsComPortOpen()` can be used to check if transmitted packets are actually read by computer side
- Attempts to avoid zero length packets (small latency vs speed tradeoff)

### How to use
- replace `USB_DEVICE/App/usbd_cdc_if.c` and `USB_DEVICE/App/usbd_cdc_if.h` with the ones provided in the repository
  - adjust  `APP_RX_DATA_SIZE` and `APP_TX_DATA_SIZE` if needed
- Send data via `CDC_Transmit(const void* Buf, uint32_t Len)` or `CDC_TransmitString(const char *string)`
   -`CDC_Transmit` is not reentrant safe / "interrupt safe"
- Receive data by polling `uint32_t CDC_RXQueue_Dequeue(void* Dst, uint32_t MaxLen)` or by overriding `uint8_t CDC_DataReceivedHandler(const uint8_t *Data, uint32_t len)` and handling data immediately upon reception


### TODO
- check if the physical limit of 4096 bytes for the USB packet area really applies to all devices

### Tested on
- STM32F4


### API
```c
uint8_t CDC_Transmit(const void* Buf, uint32_t Len);
uint8_t CDC_TransmitString(const char *string);
uint8_t CDC_IsBusy();
uint32_t CDC_RXQueue_Dequeue(void* Dst, uint32_t MaxLen);
uint32_t CDC_TXQueue_GetReadAvailable();
uint32_t CDC_TXQueue_GetWriteAvailable();
uint32_t CDC_RXQueue_GetReadAvailable();
uint32_t CDC_RXQueue_GetWriteAvailable();
uint32_t CDC_GetDroppedTxPackets();
uint32_t CDC_GetDroppedRxPackets();
void CDC_ResetDroppedTxPackets();
void CDC_ResetDroppedRxPackets();
uint8_t CDC_DataReceivedHandler(const uint8_t *Data, uint32_t len);
uint32_t CDC_GetLastTransmitStartTick();
uint32_t CDC_GetLastTransmitCompleteTick();
uint8_t CDC_IsComportOpen();
```
