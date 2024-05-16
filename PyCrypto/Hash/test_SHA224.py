from Crypto.Hash import SHA224

h = SHA224.new()
h.update(b'Hello')
print(h.hexdigest())