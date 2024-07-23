import serial

ser = serial.Serial(port='COM5', baudrate=115200)
while True:
    value = ser.readline()
    str_value = str(value)
    print(str_value)
