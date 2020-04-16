/* Copyright (C) 2019 IBM Corp.
* This program is Licensed under the Apache License, Version 2.0
* (the "License"); you may not use this file except in compliance
* with the License. You may obtain a copy of the License at
*   http://www.apache.org/licenses/LICENSE-2.0
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License. See accompanying LICENSE file.
*/
#include <iostream>

#include <helib/helib.h>
#include <helib/replicate.h>


/*  Example of BGV scheme  */
int main(int argc, char *argv[]) {

    long p = 1021;      // Plaintext prime modulus
    long r = 1;         // Hensel lifting (default = 1)
    long bits = 320;    // Number of bits of the modulus chain
    long c = 2;         // Number of columns of Key-Switching matix (default = 2 or 3)
    long k = 128;
    long s = 4;
    long d = 1;
    long w = 64;

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
        ptxt[i] = i;
    }

    // Create a ciphertext
    helib::Ctxt ctxt(public_key);
    // Encrypt the plaintext using the public_key
    ea.encrypt(ctxt, public_key, ptxt);

    for (int i = 0; i < 4; i++) {
        ctxt *= ctxt;
    }

    // Decrypt the modified ciphertext
    std::vector<long> decrypted_rep(nslots);
    ea.decrypt(ctxt, secret_key, decrypted_rep);

    // Print the decrypted plaintext
    std::cout << "Decrypted Replicated Sum: " << helib::vecToStr(decrypted_rep) << std::endl;

    return EXIT_SUCCESS;
}
