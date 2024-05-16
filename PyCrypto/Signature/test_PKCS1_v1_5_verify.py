from Crypto.Signature import PKCS1_v1_5
from Crypto.Hash import SHA
from Crypto.PublicKey import RSA

key = RSA.importKey(open('pubkey.der').read())
h = SHA.new(message)
verifier = PKCS1_v1_5.new(key)
if verifier.verify(h, signature):
   print("The signature is authentic.")
else:
   print("The signature is not authentic.")