from Crypto.Cipher import CAST
from Crypto import Random

key = b'Sixteen byte key'
iv = Random.new().read(CAST.block_size)
cipher = CAST.new(key, CAST.MODE_OPENPGP, iv)
plaintext = b'sona si latine loqueris '
msg = cipher.encrypt(plaintext)

eiv = msg[:CAST.block_size+2]
ciphertext = msg[CAST.block_size+2:]
cipher = CAST.new(key, CAST.MODE_OPENPGP, eiv)
print(cipher.decrypt(ciphertext))