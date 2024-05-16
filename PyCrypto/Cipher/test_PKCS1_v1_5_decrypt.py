from Crypto.Cipher import PKCS1_v1_5
from Crypto.PublicKey import RSA
from Crypto.Hash import SHA
from Crypto import Random

key = RSA.importKey(open('privkey.der').read())

dsize = SHA.digest_size
sentinel = Random.new().read(15+dsize)      # Let's assume that average data length is 15

cipher = PKCS1_v1_5.new(key)
message = cipher.decrypt(ciphertext, sentinel)

digest = SHA.new(message[:-dsize]).digest()
if digest==message[-dsize:]:                # Note how we DO NOT look for the sentinel
    print("Encryption was correct.")
else:
    print("Encryption was not correct.")