# First TrustZone GPIO App 

This project consists of a basic TrustZone enabled application.<br>
The purpose of this project is to start learning how the Non-Secure world interacts with the Secure world in a TrustZone enable
MCU.

---
What this application does is to make a call to the function `Toggle_Pin()` whenever the `USER_BUTTON`, that has been assigned
to the Non-Secure World, is pressed. The function `Toggle_Pin()` turns ON or OFF `LED_BLUE`, that has been assigned to the Secure World.
In order to make this interaction between those two components belonging to the two different worlds possible, the method `Toggle_Pin()` has
been defined in the NSC API.
