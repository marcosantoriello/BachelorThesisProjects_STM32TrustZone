# BachelorThesisProjects_STM32TrustZone

This repository contains all the projects developed for my Bachelor's thesis titled **"Study of ARM TrustZone and Implementation of Security Solutions for Embedded Devices"**. The projects demonstrate the use of ARM TrustZone technology on **STM32** microcontrollers, focusing on how to isolate secure operations in an embedded environment and implement robust security solutions.

## Overview

The aim of these projects is to explore the ARM TrustZone technology, which provides a **hardware-enforced isolation** between the secure world and the non-secure world within a single processor. This isolation is crucial for securing sensitive operations, such as key management and cryptographic operations, on resource-constrained embedded systems.

The primary project implemented for this thesis is called **SignatureProject**, which demonstrates a **challenge-response authentication protocol** where cryptographic keys and operations are securely handled by the ARM TrustZone.

In addition, the repository includes several smaller projects designed to experiment some general features of the board.

## Projects

### SignatureProject

The [**SignatureProject**](https://github.com/marcosantoriello/BachelorThesisProjects_STM32TrustZone/tree/main/Projects/SignatureProject) is the main project in this repository. It implements a **challenge-response authentication protocol** that leverages the ARM TrustZone on an STM32U575ZI microcontroller. In this protocol, the board acts as the secure endpoint, and an external terminal initiates the connection.

#### Key Features:
- **Challenge Generation:** The board generates a challenge, which is then signed by the external terminal.
- **TrustZone Verification:** The ARM TrustZone verifies the signed challenge to ensure the authenticity of the external terminal.
- **RSA Encryption:** Once the terminal is authenticated, the board encrypts data using RSA before transmitting it to the terminal.
- **Cryptographic Operations:** All cryptographic operations, including key generation, encryption, and decryption, are handled securely within the TrustZone.

## Requirements
To run and develop these projects, you'll need the following tools and software:
- [**STM32CubeIDE**](https://www.st.com/en/development-tools/stm32cubeide.html): The integrated development environment used for building and debugging the projects.
- [**STM32U575ZI Nucleo-144 (MB1549) Board**](https://www.st.com/en/evaluation-tools/nucleo-u575zi-q.html): The microcontroller board used for running the projects.
- [**wolfSSL**](https://www.wolfssl.com/): The external cryptographic library used for some of the cryptographic functions in the SignatureProject.
  
## Getting Started
1. **Clone the repository**:
    ```bash
        git clone https://github.com/marcosantoriello/BachelorThesisProjects_STM32TrustZone.git
    ```
2. **Open the desired project in STM32CubeIDE**.
3. **Build and Flash** the project via STM32CubeIDE with your board connected to your computer.
4. **Run the Application**