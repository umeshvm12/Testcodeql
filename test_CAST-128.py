from Crypto.Cipher import CAST

key = b'Sixteen byte key'
cipher = CAST.new(key, CAST.MODE_OPENPGP)
plaintext = b'sona si latine loqueris '
msg = cipher.encrypt(plaintext)

eiv = msg[:CAST.block_size+2]
ciphertext = msg[CAST.block_size+2:]
cipher = CAST.new(key, CAST.MODE_OPENPGP, eiv)
print(cipher.decrypt(ciphertext))