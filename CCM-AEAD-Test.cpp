#include "stdafx.h"

#include <iostream>
using std::cout;
using std::cerr;

#include <string>
using std::string;

#include "cryptlib.h"
using CryptoPP::AAD_CHANNEL;
using CryptoPP::DEFAULT_CHANNEL;
using CryptoPP::BufferedTransformation;
// using CryptoPP::AuthenticatedSymmetricCipher;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;

#include "aes.h"
using CryptoPP::AES;

#include "ccm.h"
using CryptoPP::CCM;

#include "hex.h"
using CryptoPP::HexEncoder;

#include "assert.h"

int main(int argc, char* argv[])
{
    // Test Vector 003
    byte key[]={ 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
        0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f };
    byte iv[]={ 0x10,0x11,0x12,0x13,0x14,0x15,0x16,
        0x17,0x18,0x19,0x1a,0x1b };

    const byte aa[] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13 };
    string adata = string( (const char*)aa, sizeof(aa) );

    const byte pa[] = { 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
        0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37 };
    string pdata = string( (const char*)pa, sizeof(pa) );
    const int TAG_SIZE = 8;

    //CTX e3b201a9f5b71a7a9b1ceaeccd97e70b6176aad9a4428aa5
    //TAG 484392fbc1b09951

    // Encrypted, with Tag
    string cipher, encoded;

    // Recovered
    string radata, rpdata;

    /*********************************\
    \*********************************/

    try
    {
        CCM< AES, TAG_SIZE >::Encryption e;
        e.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
        e.SpecifyDataLengths( adata.size(), pdata.size(), 0 );

        AuthenticatedEncryptionFilter ef( e,
            new StringSink( cipher )
            ); // AuthenticatedEncryptionFilter

        // AuthenticatedEncryptionFilter::ChannelPut
        //  defines two channels: DEFAULT_CHANNEL (empty) and AAD_CHANNEL
        //   DEFAULT_CHANNEL is encrypted and authenticated
        //   AAD_CHANNEL is authenticated
        ef.ChannelPut( AAD_CHANNEL, (const byte*)adata.data(), adata.size() );
        ef.ChannelMessageEnd(AAD_CHANNEL);

        // Authenticated data *must* be pushed before
        //  Confidential/Authenticated data
        ef.ChannelPut( DEFAULT_CHANNEL, (const byte*)pdata.data(), pdata.size() );
        ef.ChannelMessageEnd(DEFAULT_CHANNEL);

        // Pretty print
        StringSource( cipher, true,
            new HexEncoder( new StringSink( encoded ), true, 16, " " ) );
    }
    catch( CryptoPP::BufferedTransformation::NoChannelSupport& e )
    {
        // The tag must go in to the default channel:
        //  "unknown: this object doesn't support multiple channels"
        cerr << "Caught NoChannelSupport..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }
    catch( CryptoPP::InvalidArgument& e )
    {
        cerr << "Caught InvalidArgument..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }

    /*********************************\
    \*********************************/

    // Attack the first and last byte
    //if( cipher.size() > 1 )
    //{
    // cipher[ 0 ] |= 0x0F;
    // cipher[ cipher.size()-1 ] |= 0x0F;
    //}

    /*********************************\
    \*********************************/

    try
    {
        // Break the cipher text out into it's
        //  components: Encrypted and MAC
        string enc = cipher.substr( 0, cipher.length()-TAG_SIZE );
        string tag = cipher.substr( cipher.length()-TAG_SIZE );

        // Sanity checks
        assert( cipher.size() == enc.size() + tag.size() );
        assert( enc.size() == pdata.size() );
        assert( TAG_SIZE == tag.size() );

        // Not recovered - sent via clear channel
        radata = adata;

        CCM< AES, TAG_SIZE >::Decryption d;
        d.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
        d.SpecifyDataLengths( radata.size(), enc.size(), 0 );

        // Object will not throw an exception
        //  during decryption\verification _if_
        //  verification fails.
        //AuthenticatedDecryptionFilter df( d, NULL,
        // AuthenticatedDecryptionFilter::MAC_AT_BEGIN );

        AuthenticatedDecryptionFilter df( d, NULL,
            AuthenticatedDecryptionFilter::MAC_AT_BEGIN | 
            AuthenticatedDecryptionFilter::THROW_EXCEPTION );

        // The order of the following calls are important
        df.ChannelPut( DEFAULT_CHANNEL, (const byte*)tag.data(), tag.size() );
        df.ChannelPut( AAD_CHANNEL, (const byte*)adata.data(), adata.size() );
        df.ChannelPut( DEFAULT_CHANNEL, (const byte*)enc.data(), enc.size() );

        df.ChannelMessageEnd( AAD_CHANNEL );
        df.ChannelMessageEnd( DEFAULT_CHANNEL );

        // If the object does not throw, here's the only
        // opportunity to check the data's integrity
        bool b = false;
        b = df.GetLastResult();
        assert( true == b );

        // Remove data from channel
        string retrieved;
        size_t n = (size_t)-1;

        // Plain text recovered from enc.data()
        df.SetRetrievalChannel( DEFAULT_CHANNEL );
        n = (size_t)df.MaxRetrievable();
        retrieved.resize( n );

        if( n > 0 ) { df.Get( (byte*)retrieved.data(), n ); }
        rpdata = retrieved;
        assert( rpdata == pdata );

        // All is well - work with data
        cout << "Decrypted and Verified data. Ready for use." << endl;
        cout << endl;

        cout << "adata length: " << adata.size() << endl;
        cout << "pdata length: " << pdata.size() << endl;
        cout << endl;

        //cout << "adata: " << adata << endl;
        //cout << "pdata: " << pdata << endl;
        //cout << endl;

        cout << "cipher text (enc + tag): " << endl << " " << encoded << endl;
        cout << endl;

        cout << "recovered adata length: " << radata.size() << endl;
        cout << "recovered pdata length: " << rpdata.size() << endl;
        cout << endl;

        //cout << "recovered adata: " << radata << endl;
        //cout << "recovered pdata: " << rpdata << endl;
        //cout << endl;
    }
    catch( CryptoPP::InvalidArgument& e )
    {
        cerr << "Caught InvalidArgument..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }
    catch( CryptoPP::HashVerificationFilter::HashVerificationFailed& e )
    {
        cerr << "Caught HashVerificationFailed..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }

    /*********************************\
    \*********************************/

    return 0;
}
