from Crypto.Cipher import ARC4
from Crypto.Hash import SHA256, HMAC
from Crypto.Random import get_random_bytes

key = b'Very long and confidential key'
nonce = get_random_bytes(16)
tempkey = HMAC.new(key, nonce, digestmod=SHA256).digest()
cipher = ARC4.new(tempkey)
msg = nonce + cipher.encrypt(b'Open the pod bay doors, HAL')