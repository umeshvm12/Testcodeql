from Crypto.Cipher import PKCS1_OAEP
from Crypto.PublicKey import RSA

message = 'To be encrypted'
key = RSA.importKey(open('pubkey.der').read())
cipher = PKCS1_OAEP.new(key)
ciphertext = cipher.encrypt(message)