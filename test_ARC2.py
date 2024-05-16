from Crypto.Cipher import ARC2

key = b'Sixteen byte key'
cipher = ARC2.new(key, ARC2.MODE_CFB)
msg = cipher.iv + cipher.encrypt(b'Attack at dawn')