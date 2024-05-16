from Crypto.Hash import MD2

h = MD2.new()
h.update(b'Hello')
print(h.hexdigest())