from Crypto.Cipher import PKCS1_OAEP
from Crypto.PublicKey import RSA

key = RSA.importKey(open('privkey.der').read())
cipher = PKCS1_OAEP.new(key)
message = cipher.decrypt(ciphertext)