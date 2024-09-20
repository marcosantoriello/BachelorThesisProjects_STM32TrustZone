import binascii
from Crypto.PublicKey import RSA

# Chiave pubblica esadecimale copiata dall'output della STM32 (supponiamo che sia in DER)
public_key_hex = "30820122300d06092a864886f70d01010105000382010f003082010a0282010100c36967f9ce7203f51819d836e8b32f0c409380af64d176a414ece6c935c0cdac014b74dc8d94927559852d6246551dc014dc51b534d295aca2c5e421d3e38060bf6490912f2dbd857ca33c133087ffaec6a8e6dece228074873559024629c34373e2aa388aceed74430a1993af561af059f7c36b8c9866b7aaad96aa01089f091819aa8bce9fdc012aac0ee4501af3f598a83d469e7d3ec76a46abc0262b759efcb58b63ba72022f80b38c2efcf6b1df881274094abe7923358b93ebe192bf9e68b10c31ca4a67fda39fda58f5f0d8fcb78252731fc82f1419c25bb94c4fec8d15708aa2fb5317967c787a59c9128ee88f72c804ba2edc93ab978cb531ad5c190203010001"

# Converti l'esadecimale in binario
public_key_der = binascii.unhexlify(public_key_hex)

# Importa la chiave pubblica in formato DER
public_key = RSA.import_key(public_key_der)

# Scrivi la chiave pubblica in un file PEM
with open("public_key.pem", "wb") as pem_file:
    pem_file.write(public_key.export_key(format="PEM"))

print("Chiave pubblica scritta in public_key.pem")
