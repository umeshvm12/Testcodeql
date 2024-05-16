from Crypto.Signature import PKCS1_PSS
from Crypto.Hash import SHA
from Crypto.PublicKey import RSA
from Crypto import Random

message = 'To be signed'
key = RSA.importKey(open('privkey.der').read())
h = SHA.new()
h.update(message)
signer = PKCS1_PSS.new(key)
signature = signer.sign(key)