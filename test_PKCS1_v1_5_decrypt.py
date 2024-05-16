from Crypto.Cipher import PKCS1_v1_5
from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes

rsa_key = RSA.importKey(open('privkey.der').read())

sentinel = get_random_bytes(16)

cipher = PKCS1_v1_5.new(rsa_key)
aes_key = cipher.decrypt(ciphertext, sentinel, expected_pt_len=16)

# The AES key is the random sentinel in case of error