/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v1.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

static volatile uint32_t s_txhead = 0;
static volatile uint32_t s_txtail = 0;
static volatile uint32_t s_rxhead = 0;
static volatile uint32_t s_rxtail = 0;
static volatile uint32_t s_txDropCounter = 0;
static volatile uint32_t s_rxDropCounter = 0;

static USBD_CDC_LineCodingTypeDef s_linecoding = {
  115200, /* baud rate*/
  0x00,   /* stop bits-1*/
  0x00,   /* parity - none*/
  0x08    /* nb. of bits 8*/
};
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);
static int8_t CDC_TransmitCplt_FS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
static uint8_t CDC_RXQueue_Enqueue(const uint8_t *buffer, uint32_t length);
static uint8_t CDC_TXQueue_Enqueue(const uint8_t *buffer, uint32_t length);
static const uint8_t* CDC_TXQueue_Dequeue(uint32_t *length);
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS,
  CDC_TransmitCplt_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:
    	s_linecoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
    	s_linecoding.format     = pbuf[4];
    	s_linecoding.paritytype = pbuf[5];
    	s_linecoding.datatype   = pbuf[6];
    break;

    case CDC_GET_LINE_CODING:
		pbuf[0] = (uint8_t)(s_linecoding.bitrate);
		pbuf[1] = (uint8_t)(s_linecoding.bitrate >> 8);
		pbuf[2] = (uint8_t)(s_linecoding.bitrate >> 16);
		pbuf[3] = (uint8_t)(s_linecoding.bitrate >> 24);
		pbuf[4] = s_linecoding.format;
		pbuf[5] = s_linecoding.paritytype;
		pbuf[6] = s_linecoding.datatype;
	break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  uint8_t dataHandled = CDC_DataReceivedHandler(Buf, *Len);

  if (dataHandled == CDC_RX_DATA_NOTHANDLED) {
	  if (CDC_RXQueue_Enqueue(Buf, *Len) != USBD_OK) {
		  s_rxDropCounter++;
	  }
  }

  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note must not be called from interrupt context which can interrupt USB interrupt
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(const uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */

  if (Len > 4096 + APP_TX_DATA_SIZE) {
	  return USBD_FAIL;
  }

  if (CDC_IsBusy()) {
	  result =  CDC_TXQueue_Enqueue(Buf, Len);
	  if (result != USBD_OK) {
		  s_txDropCounter++;
	  }
	  return result;
  }

  // due to automatic de-queueing from interrupt, at this point the transmit queue has to be empty
  // physical limit is 4096, use 4095 to avoid ZLP packet

  if (Len <= 4095) {
	  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t*)Buf, Len);
	  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  } else {
	  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t*)Buf, 4095);
	  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
	  CDC_TXQueue_Enqueue((uint8_t*)Buf + 4095, Len - 4095);
  }

  /* USER CODE END 7 */
  return result;
}

/**
  * @brief  CDC_TransmitCplt_FS
  *         Data transmited callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_TransmitCplt_FS(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 13 */
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);

  uint32_t queueLength;
  const uint8_t * queueData = CDC_TXQueue_Dequeue(&queueLength);
  if (queueLength > 0) {
	  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t*)queueData, queueLength);
	  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  }

  /* USER CODE END 13 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @brief  CDC_TXQueue_GetReadAvailable
  *         Check how many bytes are waiting for sending in the transmission queue
  *
  *         @note this is usually not of concern for the user
  *
  *
  * @retval number of enqueued bytes in the transmission queue
  */
uint32_t CDC_TXQueue_GetReadAvailable()
{
	return s_txhead - s_txtail;
}

/**
  * @brief  CDC_TXQueue_GetWriteAvailable
  *         Check how much space is left in the transmission queue
  *
  *
  * @retval number of available bytes in the transmission queue
  */
uint32_t CDC_TXQueue_GetWriteAvailable()
{
	return APP_TX_DATA_SIZE - CDC_TXQueue_GetReadAvailable();
}

/**
  * @brief  CDC_RXQueue_GetReadAvailable
  *         Check how many bytes are enqueued in the reception queue
  *
  *
  * @retval number of available bytes in the reception queue
  */
uint32_t CDC_RXQueue_GetReadAvailable()
{
	return s_rxhead - s_rxtail;
}

/**
  * @brief  CDC_RXQueue_GetWriteAvailable
  *         Check how much space is left in the reception queue
  *
  *         @note this is usually not of concern for the user
  *
  *
  * @retval number of available bytes in the reception queue
  */
uint32_t CDC_RXQueue_GetWriteAvailable()
{
	return APP_RX_DATA_SIZE - CDC_RXQueue_GetReadAvailable();
}

/**
  * @brief  CDC_TXQueue_Enqueue
  *         Enqueue data into the transmission queue
  *
  * @param  buffer: data to enqueue (must not be null)
  * @param  length: length of data
  * @retval USBD_OK if ok USBD_BUSY otherwise (queue full)
  */
uint8_t CDC_TXQueue_Enqueue(const uint8_t *buffer, uint32_t length)
{
	if (length > CDC_TXQueue_GetWriteAvailable()) {
		return USBD_BUSY;
	}

	uint32_t sizeTillWrapAround = APP_TX_DATA_SIZE - (s_txhead % APP_TX_DATA_SIZE);
	uint32_t firstLength = MIN(length, sizeTillWrapAround);
	uint32_t secondLength = length - firstLength;

	// first part
	uint32_t insertIndex = s_txhead % APP_TX_DATA_SIZE;
	memcpy(UserTxBufferFS + insertIndex, buffer, firstLength);
	s_txhead += firstLength;

	// second part after wrap around
	memcpy(UserTxBufferFS, buffer + firstLength, secondLength);
	s_txhead += secondLength;

	return USBD_OK;
}

/**
  * @brief  CDC_RXQueue_Enqueue
  *         Enqueue data into the reception queue
  *
  * @param  buffer: data to enqueue (must not be null)
  * @param  length: length of data
  * @retval USBD_OK if ok USBD_BUSY otherwise (queue full)
  */
uint8_t CDC_RXQueue_Enqueue(const uint8_t *buffer, uint32_t length)
{
	if (length > CDC_RXQueue_GetWriteAvailable()) {
		return USBD_BUSY;
	}

	uint32_t sizeTillWrapAround = APP_RX_DATA_SIZE - (s_rxhead % APP_RX_DATA_SIZE);
	uint32_t firstLength = MIN(length, sizeTillWrapAround);
	uint32_t secondLength = length - firstLength;

	// first part
	uint32_t insertIndex = s_rxhead % APP_RX_DATA_SIZE;
	memcpy(UserRxBufferFS + insertIndex, buffer, firstLength);
	s_rxhead += firstLength;

	// second part after wrap around
	memcpy(UserRxBufferFS, buffer + firstLength, secondLength);
	s_rxhead += secondLength;

	return USBD_OK;
}

/**
  * @brief  CDC_TXQueue_Dequeue
  *         Dequeue data from the transmission queue
  *
  *         @note
  *         This is intended to be called from the transmission complete interrupt
  *         The caller must not be interrupted by interrupts which use CDC_transmit functions
  *         as the dequeued data is just a pointer to memory which can now be overriden again
  *
  * @param  length: pointer where the length of dequeued data is written to (must not be null)
  * @retval buffer to dequeud data
  */
const uint8_t* CDC_TXQueue_Dequeue(uint32_t *length)
{
	uint32_t queueSize = CDC_TXQueue_GetReadAvailable();
	if (queueSize == 0) {
		*length = 0;
		return NULL;
	}

	// length is capped so that we get no buffer wrap around
	// this reduces complexity and reduces memory requirements, but decreases throughput
	// length is also capped to 4096 due to ST internals
	uint32_t sizeTillWrapAround = APP_TX_DATA_SIZE - (s_txtail % APP_TX_DATA_SIZE);
	uint32_t dequeueLength = MIN(MIN(queueSize, sizeTillWrapAround), 4096);

	// this is a small optimization: if we send a packet multiple of 64 bytes (512 bytes for HS)
	// the next USB transfer slot is wasted with a ZLP, so instead send 1 byte in next slot
	// or possibly even more, if new data got enqueued
	// this is increases throughput at the cost of latency
	if (dequeueLength % 64 == 0) {
		dequeueLength--;
	}

	*length = dequeueLength;
	const uint8_t * dequeueData = UserTxBufferFS + (s_txtail % APP_TX_DATA_SIZE);
	s_txtail += dequeueLength;

	return dequeueData;
}

/**
  * @brief  CDC_RXQueue_Dequeue
  *         Dequeue all data from the reception queue, but at most MaxLen bytes
  *
  *
  * @param  Dst: Destination buffer
  * @param  MaxLen: Maximum number of bytes to read
  * @retval Number of bytes dequeued. The value is 0 if no bytes are available
  */
uint32_t CDC_RXQueue_Dequeue(uint8_t* Dst, uint32_t MaxLen)
{
	uint32_t queueSize = CDC_RXQueue_GetReadAvailable();
	if (queueSize == 0 || MaxLen == 0 || Dst == NULL) {
		return 0;
	}

	uint32_t dequeueLength = MIN(queueSize, MaxLen);
	uint32_t sizeTillWrapAround = APP_TX_DATA_SIZE - (s_rxtail % APP_RX_DATA_SIZE);
	uint32_t firstLength = MIN(dequeueLength, sizeTillWrapAround);
	uint32_t secondLength = dequeueLength - firstLength;

	// first part
	const uint8_t * dequeueData = UserRxBufferFS + (s_rxtail % APP_RX_DATA_SIZE);
	memcpy(Dst, dequeueData, firstLength);
	s_rxtail += firstLength;

	// second part after wrap around
	dequeueData = UserRxBufferFS;
	memcpy(Dst + firstLength, dequeueData, secondLength);
	s_rxtail += secondLength;

	return dequeueLength;
}

/**
  * @brief  CDC_IsBusy
  *         Check if the CDC is busy transmitting data
  *         @note If CDC is busy, one can still enqueue data
  *
  * @retval 1 if busy transmitting data
  */
uint8_t CDC_IsBusy()
{
	USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
	return hcdc->TxState != 0;
}

/**
  * @brief  CDC_GetDroppedTxPackets
  *         Get the number of dropped packets which could not be sent (and also not enqueued)
  *
  * @retval number of dropped packets
  */
uint32_t CDC_GetDroppedTxPackets()
{
	return s_txDropCounter;
}

/**
  * @brief  CDC_GetDroppedRxPackets
  *         Get the number of dropped received packets (neither handled nor enqueued)
  *
  * @retval number of dropped packets
  */
uint32_t CDC_GetDroppedRxPackets()
{
	return s_rxDropCounter;
}

/**
  * @brief  CDC_ResetDroppedTxPackets
  *         Reset the dropped packed counter for the transmission
  *
  * @retval
  */
void CDC_ResetDroppedTxPackets()
{
	s_txDropCounter = 0;
}

/**
  * @brief  CDC_ResetDroppedRxPackets
  *         Reset the dropped packed counter for the reception
  *
  * @retval
  */
void CDC_ResetDroppedRxPackets()
{
	s_rxDropCounter = 0;
}

/**
  * @brief  CDC_TransmitString_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *
  *
  * @param  string: 0-terminated C-string to send
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_TransmitString_FS(const char * string)
{
	return CDC_Transmit_FS((const uint8_t*)string, strlen(string));
}

/**
  * @brief  CDC_DataReceivedHandler
  *         Called when data is received via USB CDC
  *         This can be overwritten via user provided function
  *         @note this is called from USB interrupt context
  *
  *
  * @param  Buf: data
  * @param  len: number of bytes received (<= 64 for USB FS, <= 512 for USB HS)
  * @retval CDC_RX_DATA_HANDLED if the data is handled and should not be placed in the receive queue
  */
__weak uint8_t CDC_DataReceivedHandler(const uint8_t *Buf, uint32_t len)
{
	UNUSED(Buf);
	UNUSED(len);
	return CDC_RX_DATA_NOTHANDLED;
}


/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
