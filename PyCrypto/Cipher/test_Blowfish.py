from Crypto.Cipher import Blowfish
from Crypto import Random
from struct import pack

bs = Blowfish.block_size
key = b'An arbitrarily long key'
iv = Random.new().read(bs)
cipher = Blowfish.new(key, Blowfish.MODE_CBC, iv)
plaintext = b'docendo discimus '
plen = bs - divmod(len(plaintext),bs)[1]
padding = [plen]*plen
padding = pack('b'*plen, *padding)
msg = iv + cipher.encrypt(plaintext + padding)