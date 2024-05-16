from Crypto.Hash import MD4

h = MD4.new()
h.update(b'Hello')
print(h.hexdigest())