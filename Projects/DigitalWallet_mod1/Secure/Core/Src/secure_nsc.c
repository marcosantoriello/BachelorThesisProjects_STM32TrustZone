/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Secure/Src/secure_nsc.c
  * @author  MCD Application Team
  * @brief   This file contains the non-secure callable APIs (secure world)
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

/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "secure_nsc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL;   /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern byte publicKeyDer[4096];
extern word32 publicKeyDerSz;
extern byte plaintext[256];
extern byte ciphertext[256];
extern unsigned char edPubKey[32];
extern word32 edPubKeySz;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
  if(func != NULL)
  {
    switch(CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault Interrupt occurred */
        pSecureFaultCallback = func;
        break;
      case GTZC_ERROR_CB_ID:             /* GTZC Interrupt occurred */
        pSecureErrorCallback = func;
        break;
      default:
        /* unknown */
        break;
    }
  }
}

CMSE_NS_ENTRY void SECURE_get_rsa_pk(byte *out_public_key, uint32_t *out_key_size) {
	memcpy(out_public_key, publicKeyDer, publicKeyDerSz);
	*out_key_size = publicKeyDerSz;
}

CMSE_NS_ENTRY void SECURE_rsa_encrypt(byte *input, word32 inputSz, byte *output, word32 *outputSz) {
	rsa_encrypt(input, inputSz, output, outputSz);
}

CMSE_NS_ENTRY void SECURE_rsa_decrypt(byte *input, word32 inputSz, byte *output, word32 *outputSz) {
	rsa_decrypt(input, inputSz, output, outputSz);
}

CMSE_NS_ENTRY void SECURE_get_ed25519_pk(byte *out_public_key, uint32_t *out_key_size) {
	memcpy(out_public_key, edPubKey, edPubKeySz);
	*out_key_size = edPubKeySz;
}

CMSE_NS_ENTRY void SECURE_generate_rsa_keys(void) {
	generate_rsa_key();
}

CMSE_NS_ENTRY void SECURE_generate_ed25519_keys(void) {
	generate_ed25519_key();
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

