
#include <iostream>

#include <helib/helib.h>
#include <helib/replicate.h>
#include <helib/keySwitching.h>


/*  Example of BGV scheme  */
int main(int argc, char *argv[]) {

    long p = 1021;      // Plaintext prime modulus
    long r = 1;         // Hensel lifting (default = 1)
    long bits = 433;    // Number of bits of the modulus chain
    long c = 2;         // Number of columns of Key-Switching matix (default = 2 or 3)
    long k = 128;       // Security level
    long s = 1;         // Minimum number of plaintext slots
    long d = 1;         // Embedding degree
    long w = 64;        // hamming weight of secret key

    bool param_choice = false;
    if (param_choice) {
        p=2;
        r=1;
        bits=230;
        c=2;
        k=128;
        s=4;
        d=10;
        w=64;
    }

    // Cyclotomic polynomial - defines phi(m)
    long m = helib::FindM(k, bits, c, p, d, s, 0);
    std::cout << "Initialising context object..." << std::endl;
    // Intialise context
    helib::Context context(m, p, r);
    // Modify the context, adding primes to the modulus chain
    std::cout  << "Building modulus chain..." << std::endl;
    buildModChain(context, bits, c);

    int primeSizes[25];
    for(int i = 0; i < 25; i++) {
      primeSizes[i] = int(round(log2(context.ithPrime(i))));
    }

    // Print the context
    context.zMStar.printout();
    std::cout << std::endl;

    // Print the security level
    std::cout << "Security: " << context.securityLevel() << std::endl;

    // Secret key management
    std::cout << "Creating secret key..." << std::endl;
    // Create a secret key associated with the context
    helib::SecKey secret_key(context);
    // Generate the secret key
    secret_key.GenSecKey();
    std::cout << "Generating key-switching matrices..." << std::endl;
    // Compute key-switching matrices that we need
    helib::addSome1DMatrices(secret_key);
    // Public key management
    // Set the secret key (upcast: SecKey is a subclass of PubKey)
    const helib::PubKey& public_key = secret_key;

    // Get the EncryptedArray of the context
    const helib::EncryptedArray& ea = *(context.ea);

    // Get the number of slot (phi(m))
    long nslots = ea.size();
    std::cout << "Number of slots: " << nslots << std::endl;

    // Create a vector of long with nslots elements
    std::vector<long> ptxt(nslots);
    // Set it with numbers 0..nslots - 1

    for (int i = 0; i < nslots; ++i) {
        ptxt[i] = nslots - i;
    }

    // Create a ciphertext
    helib::Ctxt ctxt(public_key);
    helib::Ctxt prod(public_key);
    // Encrypt the plaintext using the public_key
    ea.encrypt(ctxt, public_key, ptxt);
    std::vector<helib::CtxtPart> ctxt_parts = ctxt.getParts();
    helib::IndexMap<NTL::vec_long> data = ctxt_parts[1].getMap();
    std::cout << data.getIndexSet() << std::endl;
    // long int ctxt_size = 0;
    // for (int i = 6; i < 12; i++) {
    //   ctxt_size += data[i].length();
    // }
    // std::cout << ctxt_size * 8 << std::endl;
    // NTL::vec_long first_prime_data = data[data.getIndexSet().first()];
    // std::cout << first_prime_data.length() << std::endl;
    prod = ctxt;

    for (int i = 0; i < 15; i++) {
        std::cout << "ctxt^" << i+1 << std::endl;
        std::cout << "Capacity: " << prod.capacity() << std::endl;
        std::cout << "Noise bound: " << prod.getNoiseBound() << std::endl;
        int modSize = 0;
        for (long i = prod.getPrimeSet().first(); i <= prod.getPrimeSet().last(); i = prod.getPrimeSet().next(i)) {
          modSize += primeSizes[i];
        }

        prod.multiplyBy(ctxt);
    }

    // Decrypt the modified ciphertext
    std::vector<long> decrypted_rep(nslots);
    ea.decrypt(prod, secret_key, decrypted_rep);

    // Print the decrypted plaintext
    std::cout << "Decrypted Product: " << helib::vecToStr(decrypted_rep) << std::endl;

    return EXIT_SUCCESS;
}
