/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[1];
uint8_t input_buffer[64];
uint8_t input_index = 0;
byte publicKeyDer[4096];
int encrypt = 0;
int decrypt = 0;

byte encrypted[256];
word32 encryptedSz = sizeof(encrypted);
byte decrypted[256];
word32 decryptedSz = sizeof(decrypted);

char b64_encrypted[350];
word32 b64_encryptedSz = sizeof(b64_encrypted);
char b64_decrypted[350];
word32 b64_decryptedSz = sizeof(b64_decrypted);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */
void send_message(const char *msg);
void process_command(const char *buffer);
int hexCharToInt(char c);
void hexStringToByteArray(const char* hexString, byte *byteArray, size_t byteArraySize);
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* --- UART FUNCTIONS --- */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1) {
	  if (rx_buffer[0] =='\r') {
		  input_buffer[input_index] = '\0';
		  process_command((char *)input_buffer);

		  // Resetting input buffer...
		  memset(input_buffer, 0, sizeof(input_buffer));
		  input_index = 0;
	  } else {
		  input_buffer[input_index++] = rx_buffer[0];
	  }
	  HAL_UART_Receive_IT(&huart1, rx_buffer, sizeof(rx_buffer));
  }
}

void process_command(const char *buffer) {
	char *msg;
	if (strcmp(buffer, "Encrypt") == 0) {
		encrypt = 1;
		send_message("Insert the text to encrypt:\r\n");
	} else if (strcmp(buffer, "Decrypt") == 0) {
		decrypt = 1;
		send_message("Insert the text to decrypt:\r\n");
	} /* --- RSA ENCRYPTION --- */
	else if ((strcmp(buffer, "Encrypt") != 0 && strcmp(buffer, "Decrypt") != 0)
			&& encrypt == 1) {
		memset(encrypted, 0, sizeof(encrypted)); // resetting enc buffer before encrypting
		SECURE_rsa_encrypt((byte *)buffer, sizeof(buffer), encrypted, &encryptedSz);
		encrypt = 0;
		send_message("Successfully encrypted your data!\r\nCiphertext:\r\n");
		for (size_t i = 0; i < sizeof(encrypted); i++) {
		  	char hex_str[3];
		  	sprintf(hex_str, "%02X", encrypted[i]);
		  	send_message(hex_str);
		}
		send_message("\r\n");
	} /* --- RSA DECRYPTION --- */
	else if ((strcmp(buffer, "Encrypt") != 0 && strcmp(buffer, "Decrypt") != 0)
			&& decrypt == 1) {
		memset(decrypted, 0, sizeof(decrypted)); // resetting dec buffer before encrypting
		SECURE_rsa_decrypt((byte *)buffer, sizeof(buffer), decrypted, &decryptedSz);
		send_message("Decrypted text:\r\n");
		send_message((char *) decrypted);
	}
	else {
		msg ="Command unknown\n\r";
		encrypt = 0;
		decrypt = 0;
		HAL_UART_Transmit_IT(&huart1, (uint8_t *)msg, strlen(msg));
	}
}

void send_message(const char *msg) {
	HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, rx_buffer, sizeof(rx_buffer));

  send_message("Welcome to RSA TZ!\r\n");

  /* --- RETRIEVING RSA PUBLIC KEY --- */
  send_message("Here it is the public key to encrypt:\r\n");
  SECURE_get_rsa_pk(publicKeyDer);
  for (size_t i = 0; i < sizeof(publicKeyDer); i++) {
	  if (publicKeyDer[i] == '\0') {
		  break;
	  }
	  // sending as printable characters
	  char hex_str[3];
	  sprintf(hex_str, "%02X", publicKeyDer[i]);
	  send_message(hex_str);
  }
  send_message("\r\n");

  send_message("Type Encrypt to encrypt a message.\r\nType Decrypt to decrypt a message.\r\nType PubKey to get the public key.\r\n");




//  /* --- TESTING RSA ENCRYPTION --- */
//  byte plaintext[] = "Test encryption data";
//  byte encrypted[256];
//  word32 encryptedSz = sizeof(encrypted);
//
//
//  SECURE_rsa_encrypt(plaintext, sizeof(plaintext), encrypted, &encryptedSz);
//  send_message("Ciphertext:\r\n");
//  for (size_t i = 0; i < sizeof(encrypted); i++) {
//  	  // sending as printable characters
//  	  char hex_str[3];
//  	  sprintf(hex_str, "%02X", encrypted[i]);
//  	  send_message(hex_str);
//   }
//  send_message("\r\n");
//
//  /* --- TESTING RSA DECRYPTION --- */
//  byte decrypted[256];
//  word32 decryptedSz = sizeof(decrypted);
//  SECURE_rsa_decrypt(encrypted, encryptedSz, decrypted, &decryptedSz);
//  send_message("Plaintext:\r\n");
//
//  send_message((char *) decrypted);
//  send_message("\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
