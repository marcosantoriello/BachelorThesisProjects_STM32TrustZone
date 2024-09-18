import binascii
from Crypto.PublicKey import RSA

# Chiave pubblica esadecimale copiata dall'output della STM32 (supponiamo che sia in DER)
public_key_hex = "30820122300d06092a864886f70d01010105000382010f003082010a0282010100cb9a108f724cc45d064bb2de784947181c547b7d8b291e9294a947f7278fb18a709ccc5487455fdf92de0a454b1c86f599c895228ceb5185e87b45cacf0d81c9451e5d48e99846cce5a3fa028b734991c7b9cb36e5fbbe7c4cc5137e2f789d3f6d135275ab94c3dcc1ef76abd09c0a77a3d14994174b117b5b41e536334fd98974e99c050fec7bc901792275e41664a13065d2cbae60c179dd8775f89d8e477dab9c9dc29729df3bc4659b16906ebf793db46e7ef73e82455cb4acf79cf29b06ef6e0beb8e0c2bb71d5428af4673eae2811c67f509baf16f3f9e2082df30c28b10b4f8d6dfc5432ff8a0b094adb6c7b988601b279cb8c30a97e495d4a8f38abf0203010001"

# Converti l'esadecimale in binario
public_key_der = binascii.unhexlify(public_key_hex)

# Importa la chiave pubblica in formato DER
public_key = RSA.import_key(public_key_der)

# Scrivi la chiave pubblica in un file PEM
with open("public_key.pem", "wb") as pem_file:
    pem_file.write(public_key.export_key(format="PEM"))

print("Chiave pubblica scritta in public_key.pem")
