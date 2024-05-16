from Crypto.Signature import PKCS1_v1_5
from Crypto.Hash import SHA
from Crypto.PublicKey import RSA

message = 'To be signed'
key = RSA.importKey(open('privkey.der').read())
h = SHA.new(message)
signer = PKCS1_v1_5.new(key)
signature = signer.sign(h)