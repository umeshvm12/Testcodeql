from Crypto.Hash import SHA

h = SHA.new()
h.update(b'Hello')
print(h.hexdigest())