from Crypto.Hash import SHA384

h = SHA384.new()
h.update(b'Hello')
print(h.hexdigest())