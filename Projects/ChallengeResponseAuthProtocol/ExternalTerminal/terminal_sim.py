from binascii import hexlify

from Crypto.Signature import pkcs1_15
from Crypto.PublicKey import RSA
from Crypto.Hash import SHA256

import binascii
import serial
import time

# Apri la chiave pubblica PEM
with open("public_key.pem", "r") as key_file:
    public_key = RSA.import_key(key_file.read())

print('[+] Successfully read RSA public key')
    
# Configurazione della seriale
ser = serial.Serial('COM3', 115200, bytesize=8, timeout=10)
time.sleep(3)
# ser.timeout = 5

print('[*] Sending challenge...')
challenge = b'This is a test'
ser.write(challenge)
ser.write(b'\n')

print('[*] Waiting for the signature...')
# Ricevi la firma dalla scheda STM32
signature = ser.read(256)

if len(signature) == 256:
    print(signature)
    try:
        h = SHA256.new(challenge)
        pkcs1_15.new(public_key).verify(h, signature)
        print("[+] Verified signature, authentication succeed!")
    except (ValueError, TypeError):
        print("[-] Not a valid signature, authentication failed.")
else:
    print("[-] There was an error in receiving the signature or the length is invalid.")
