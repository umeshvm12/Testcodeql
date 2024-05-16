from Crypto.Cipher import DES
from Crypto import Random
from Crypto.Util import Counter

key = b'-8B key-'
nonce = Random.new().read(DES.block_size/2)
ctr = Counter.new(DES.block_size*8/2, prefix=nonce)
cipher = DES.new(key, DES.MODE_CTR, counter=ctr)
plaintext = b'We are no longer the knights who say ni!'
msg = nonce + cipher.encrypt(plaintext)