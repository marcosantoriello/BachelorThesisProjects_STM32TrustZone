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
#define ED25519_PUB_KEY_SIZE 32 /* Compressed */
#define MAX_INPUT_SIZE 1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[1];
uint8_t input_buffer[1024];
uint8_t input_index = 0;
byte publicKeyDer[4096];
uint32_t publicKeyDerSz;
int encrypt = 0;
int decrypt = 0;

word32 inputSz = 0;
byte encrypted[256];
word32 encryptedSz = sizeof(encrypted);
byte decrypted[384];
word32 decryptedSz = sizeof(decrypted);

unsigned char edPubKey[ED25519_PUB_KEY_SIZE];
uint32_t edPubKeySz;

char complete_buffer[MAX_INPUT_SIZE] = {0};
int dec_message_part_index = 0; /* indicates the current part of the message to decrypt  */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void print_hex(const unsigned char* data, size_t len);
void process_command(const char *buffer);
int is_input_complete(const char *input);
int hex_to_bytes(const char* hex_str, unsigned char* byte_array, size_t byte_array_len);


/* Retargets the C library printf function to the USART. */
#include <stdio.h>
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}
#ifdef __GNUC__
int _write(int file,char *ptr, int len)
{
    int DataIdx;
    for (DataIdx= 0; DataIdx< len; DataIdx++) {
        __io_putchar(*ptr++);
    }
    return len;
}
#endif
/* USER CODE END PFP */

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

	if (strcmp(buffer, "Encrypt") == 0) {
		encrypt = 1;
		printf("Insert the text to encrypt:\r\n");
		memset(complete_buffer, 0, sizeof(complete_buffer));
		dec_message_part_index = 0;
	} else if (strcmp(buffer, "Decrypt") == 0) {
		decrypt = 1;
		printf("Insert the text to decrypt:\r\n");
		memset(complete_buffer, 0, sizeof(complete_buffer));
		dec_message_part_index = 0;

	} /* --- RSA ENCRYPTION --- */
	else if ((strcmp(buffer, "Encrypt") != 0 && strcmp(buffer, "Decrypt") != 0) && encrypt == 1) {
		memset(encrypted, 0, sizeof(encrypted)); // resetting enc buffer before encrypting
		inputSz = strlen(buffer);
		SECURE_rsa_encrypt((byte *)buffer, inputSz, encrypted, &encryptedSz);
		encrypt = 0;
		printf("Successfully encrypted your data!\r\nCiphertext:\r\n");
		print_hex(encrypted, encryptedSz);
		printf("\r\n");
		inputSz = 0;
	}
	/* --- RSA DECRYPTION --- */
	else if ((strcmp(buffer, "Encrypt") != 0 && strcmp(buffer, "Decrypt") != 0) && decrypt == 1) {
		strcat(complete_buffer, buffer);

		if (dec_message_part_index < 2) {
			dec_message_part_index++;
			printf("Insert the next part of the text to decrypt:\r\n");
		} else {
			unsigned char encrypted_data[256];
			int ret = hex_to_bytes(complete_buffer, encrypted_data, sizeof(encrypted_data));
			if (ret == 0) {
				memset(decrypted, 0, sizeof(decrypted));  // Resetting buffer
				SECURE_rsa_decrypt(encrypted_data, sizeof(encrypted_data),
						decrypted, &decryptedSz);
				printf("Decrypted text:\r\n");
				printf("%s\r\n", decrypted);
				decrypt = 0;  // Resetting decryption flag
			} else {
				printf("Invalid hex string!\r\n");
				printf("\r\n");
			}
		}
	} else {
		encrypt = 0;
		decrypt = 0;
		printf("Command unknown\r\n");
	}
}


/*--- UTILITY ---*/
int is_input_complete(const char *input) {
	return (strlen(input) >= 300);
}

void print_hex(const unsigned char* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\r\n");
}

int hex_to_bytes(const char* hex_str, unsigned char* byte_array, size_t byte_array_len) {
    size_t hex_len = strlen(hex_str);
    if (hex_len % 2 != 0) {
        return -1;
    }

    if (hex_len / 2 > byte_array_len) {
        return -1;
    }

    for (size_t i = 0; i < hex_len; i += 2) {
        if (!isxdigit(hex_str[i]) || !isxdigit(hex_str[i + 1])) {
            return -1;
        }

        char byte_str[3] = { hex_str[i], hex_str[i + 1], '\0' };
        byte_array[i / 2] = (unsigned char)strtol(byte_str, NULL, 16);
    }

    return 0;
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
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, rx_buffer, sizeof(rx_buffer));

	printf("Welcome!\r\n");

	/* --- RETRIEVING RSA PUBLIC KEY --- */
	printf("\r\nRSA public key:\r\n");
	SECURE_get_rsa_pk(publicKeyDer, &publicKeyDerSz);
	print_hex(publicKeyDer, publicKeyDerSz);

	/* --- RETRIEVING ED25519 PUBLIC KEY --- */
	printf("\r\nED25519 public key:\r\n");
	SECURE_get_ed25519_pk(edPubKey, &edPubKeySz);
	print_hex(edPubKey, edPubKeySz);
	printf("\r\n");
	printf("-Type Encrypt to encrypt your data\r\n");
	printf("-Type Decrypt to decrypt your data\r\n");

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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
