from Crypto.Signature import PKCS1_PSS
from Crypto.Hash import SHA
from Crypto.PublicKey import RSA
from Crypto import Random

key = RSA.importKey(open('pubkey.der').read())
h = SHA.new()
h.update(message)
verifier = PKCS1_PSS.new(key)
if verifier.verify(h, signature):
    print("The signature is authentic.")
else:
    print("The signature is not authentic.")