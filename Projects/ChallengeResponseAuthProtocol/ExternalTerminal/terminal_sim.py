import serial
from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256

# Apri la chiave pubblica PEM
with open("public_key.pem", "r") as key_file:
    public_key = RSA.import_key(key_file.read())

print('Successfully read RSA public key')
    
# Configurazione della seriale
ser = serial.Serial('COM3', 115200, bytesize=8)

print('Sending challenge...')
challenge = b'This is a test'
ser.write(challenge)

print('Waiting for the signature...')
# Ricevi la firma dalla scheda STM32
signature = ser.read(256)

try:
    h = SHA256.new(challenge)
    pkcs1_15.new(public_key).verify(h, signature)
    print("Firma verificata, autenticazione riuscita!")
except (ValueError, TypeError):
    print("Firma non valida, autenticazione fallita.")
