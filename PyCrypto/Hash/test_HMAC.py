from Crypto.Hash import HMAC

secret = b'Swordfish'
h = HMAC.new(secret)
h.update(b'Hello')
print(h.hexdigest())