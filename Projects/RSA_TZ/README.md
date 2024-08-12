# RSA TrustZone
This project contains the implementation of RSA encryption/decryption mechanisms.<br>
Public and private keys are generated in the Secure region of the STM32U575ZI MCU. The Secure region is also responsible for encrypting and decrypting data.<br>
Once the Public Key has been generated, it is sent to the Non-Secure world.
