#include "cryptlib.h"
#include "secblock.h"
#include "chacha.h"
#include "osrng.h"
#include "files.h"
#include "hex.h"

#include <iostream>
#include <string>

int main()
{
    using namespace CryptoPP;

    AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(std::cout));
    std::string plain("My Plaintext!! My Dear plaintext!!"), cipher, recover;

    SecByteBlock key(32), iv(24);
    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    std::cout << "Key: ";
    encoder.Put((const byte*)key.data(), key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "IV: ";
    encoder.Put((const byte*)iv.data(), iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    // Encryption object
    XChaCha20::Encryption enc;    
    enc.SetKeyWithIV(key, key.size(), iv, iv.size());

    // Perform the encryption
    cipher.resize(plain.size());
    enc.ProcessData((byte*)&cipher[0], (const byte*)plain.data(), plain.size());

    std::cout << "Plain: " << plain << std::endl;

    std::cout << "Cipher: ";
    encoder.Put((const byte*)cipher.data(), cipher.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    XChaCha20::Decryption dec;
    dec.SetKeyWithIV(key, key.size(), iv, iv.size());

    // Perform the decryption
    recover.resize(cipher.size());
    dec.ProcessData((byte*)&recover[0], (const byte*)cipher.data(), cipher.size());

    std::cout << "Recovered: " << recover << std::endl;

    return 0;
}