from Crypto.Cipher import ARC2
from Crypto import Random

key = b'Sixteen byte key'
iv = Random.new().read(ARC2.block_size)
cipher = ARC2.new(key, ARC2.MODE_CFB, iv)
msg = iv + cipher.encrypt(b'Attack at dawn')