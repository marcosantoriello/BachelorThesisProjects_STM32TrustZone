import binascii
from Crypto.PublicKey import RSA

# Chiave pubblica esadecimale copiata dall'output della STM32 (supponiamo che sia in DER)
public_key_hex = "30820122300d06092a864886f70d01010105000382010f003082010a0282010100a96244b9d081dd23739d5be8c37e56a10ce40f1dc0f30825c550c1d7e118c75aadcb263b369fa1ae6717971865fe36c3d64e7ba107d6f5c5b08c21fd7a0dff1a796811cf27b9079db96187599afea39e2361c6c18c505c414b133a5ea5fa680910be4eefe15123cfb16648dd03c828151fd4a1749ab4f023646cbe23912ee4f52b17e806e7e9b57c41848fa5e7acdb98a77b238373082d68ee0c807483671fa3af4e63001dafc66a546f03ae059350453b65766bd2d6c3658cfe3b60ebb48d3c5677cf5737675d401dcd7d09c215d50d13dd355dc27d81a6c10515b0a4ce4e47e65690369c9ee6fde569d0ded27e570ffb3c87ec2b8d5f89a81ea996e4ccd3470203010001"

# Converti l'esadecimale in binario
public_key_der = binascii.unhexlify(public_key_hex)

# Importa la chiave pubblica in formato DER
public_key = RSA.import_key(public_key_der)

# Scrivi la chiave pubblica in un file PEM
with open("public_key.pem", "wb") as pem_file:
    pem_file.write(public_key.export_key(format="PEM"))

print("Chiave pubblica scritta in public_key.pem")
