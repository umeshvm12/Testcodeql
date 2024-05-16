from Crypto.Hash import RIPEMD

h = RIPEMD.new()
h.update(b'Hello')
print(h.hexdigest())