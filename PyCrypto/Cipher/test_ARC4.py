from Crypto.Cipher import ARC4
from Crypto.Hash import SHA
from Crypto import Random

key = b'Very long and confidential key'
nonce = Random.new().read(16)
tempkey = SHA.new(key+nonce).digest()
cipher = ARC4.new(tempkey)
msg = nonce + cipher.encrypt(b'Open the pod bay doors, HAL')