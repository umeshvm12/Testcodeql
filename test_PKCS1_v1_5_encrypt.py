from Crypto.Cipher import PKCS1_v1_5
from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes

aes_key = get_random_bytes(16)

rsa_key = RSA.importKey(open('pubkey.der').read())
cipher = PKCS1_v1_5.new(rsa_key)
ciphertext = cipher.encrypt(aes_key)