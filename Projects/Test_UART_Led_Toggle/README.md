# UART Led Toggle Application

This application takes an input from USART1:
- if the input is *LED ON*, then it turns the blue led on
- if the input is *LED OFF*, the it turns the blue led off
- else it prints *Unknown command*

---

In order to test the application, I used PuTTY.<br>
In order to make it work:
- `Connection type` needs to be set to `Serial`
- In `Serial line` needs to be inserted the COMX port where the STM board is connected
- `Speed` needs to be set as `115200`
- Under `Terminal`, `Local line editing` needs to be set as `Force on`
- Optionally, to see the input that is being inserted, `Local echo` needs to be set as `Force on`