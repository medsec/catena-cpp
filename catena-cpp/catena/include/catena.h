#ifndef CATENA_INCLUDE_CATENA_H_
#define CATENA_INCLUDE_CATENA_H_

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "helpers/misc.h"
#include "./config.h"
#include "hash/blake2b.h"
#include "hash/blake2b_1.h"
#include "hash/blamka.h"
#include "components/gamma/saltmix.h"
#include "components/graph/bit_reversal_graph.h"
#include "components/graph/double_butterfly_graph.h"
#include "components/graph/drsample_graph.h"
#include "helpers/xorshift1024star.h"

namespace catena {

enum VERSION {
  CATENA_DRAGONFLY,
  CATENA_DRAGONFLY_FULL,
  CATENA_BUTTERFLY,
  CATENA_BUTTERFLY_FULL,
  CATENA_MARTYMCFLY,
  CATENA_MARTYMCFLY_FULL
};

enum MODE {
  PASSWORD_SCRAMBLING = 0,
  KEY_DERIVIATION = 1,
  PROOF_OF_WORK = 2
};

class Catena {
 public:
    // Constructor of an arbitrary Catena instance
    //     version: version ID, specified in /include/catena.h enum VERSION
    //     lambda:  number of graph layers to be used
    //     h:       primary hash algorithm, should be cryptographic secure (e.g.
    //              Blake2b)
    //     n:       output length of h in bytes
    //     h_prime: secondary hash algorithm, can be weaker than h (e.g.
    //              Blake2b-1)
    //     k:       output length of h_prime in bytes
    //     gamma:   algorithm to be used for gamma (e.g. SaltMix) or NULL_PTR
    //     f:       graph algorithm to be used
    Catena(
        VERSION version,
        uint8_t lambda,
        HashAlgorithm *h,
        size_t n,
        HashAlgorithm *h_prime,
        size_t k,
        catena::gamma::GammaAlgorithm *gamma,
        catena::graph::GraphAlgorithm *f);

    // Constructor of an arbitrary Catena instance
    //     version: version ID, specified in /include/catena.h enum VERSION
    //     lambda:  number of graph layers to be used
    //     h:       primary hash algorithm, should be cryptographic secure (e.g.
    //              Blake2b)
    //     n:       output length of h in bytes
    //     h_prime: secondary hash algorithm, can be weaker than h (e.g.
    //              Blake2b-1)
    //     k:       output length of h_prime in bytes
    //     gamma:   algorithm to be used for gamma (e.g. SaltMix) or NULL_PTR
    //     f:       graph algorithm to be used
    //     xss:     xorshift1024 instance used by graph (if graph uses one)
    Catena(
        VERSION version,
        uint8_t lambda,
        HashAlgorithm *h,
        size_t n,
        HashAlgorithm *h_prime,
        size_t k,
        catena::gamma::GammaAlgorithm *gamma,
        catena::graph::GraphAlgorithm *f,
        xorshift1024star::XorShift1024Star *xss);


    // Destructor
    ~Catena();


    // Returns a Catena-Dragonfly instance
    //     VersionID:  CATENA_DRAGONFLY
    //     h:          Blake2b
    //     n:          64
    //     h_prime:    Blake2b-1
    //     k:          64
    //     gamma:      SaltMix
    //     f:          Bit-Reversal-Graph
    //     lambda:     2
    static Catena *Dragonfly();


    // Returns a Catena-Dragonfly-Full instance
    //     VersionID:  CATENA_DRAGONFLY_FULL
    //     h:          Blake2b
    //     n:          64
    //     h_prime:    Blake2b
    //     k:          64
    //     gamma:      SaltMix
    //     f:          Bit-Reversal-Graph
    //     lambda:     2
    static Catena *DragonflyFull();


    // Returns a Catena-Butterfly instance
    //     VersionID:  CATENA_BUTTERFLY
    //     h:          Blake2b
    //     n:          64
    //     h_prime:    Blake2b-1
    //     k:          64
    //     gamma:      SaltMix
    //     f:          Double-Butterfly-Graph
    //     lambda:     4
    static Catena *Butterfly();


    // Returns a Catena-Butterfly-Full instance
    //     VersionID:  CATENA_BUTTERFLY_FULL
    //     h:          Blake2b
    //     n:          64
    //     h_prime:    Blake2b
    //     k:          64
    //     gamma:      SaltMix
    //     f:          Double-Butterfly-Graph
    //     lambda:     4
    static Catena *ButterflyFull();


    // Returns a Catena-MartyMcFly instance
    //     VersionID:  CATENA_MARTYMCFLY
    //     h:          Blake2b
    //     n:          64
    //     h_prime:    Blake2b-1
    //     k:          64
    //     gamma:      NULL_PTR (not used)
    //     f:          DRSample-Graph
    //     lambda:     1
    static Catena *MartyMcFly();


    // Returns a Catena-MartyMcFly-Full instance
    //     VersionID:  CATENA_MARTYMCFLY_FULL
    //     h:          Blake2b
    //     n:          64
    //     h_prime:    Blake2b
    //     k:          64
    //     gamma:      NULL_PTR (not used)
    //     f:          DRSample-Graph
    //     lambda:     1
    static Catena *MartyMcFlyFull();


    // Password scrambling function of a Catena instance
    //     pwd:         password
    //     pwd_len:     length of password
    //     salt:        salt
    //     salt_len:    length of salt
    //     a_data:      associated data
    //     a_data_len:  length of associated data
    //     g_low:       minimum garlic
    //     g_high:      maximum garlic
    //     gamma:       public input
    //     gamma_len:   length of gamma
    //     out:         output variable
    //     out_len:     length of output variable
    void Hash(
        uint8_t *pwd,    size_t pwd_len,
        uint8_t *salt,   size_t salt_len,
        uint8_t *a_data, size_t a_data_len,
        uint8_t g_low,   uint8_t g_high,
        uint8_t *gamma,  size_t gamma_len,
        uint8_t *out,    uint16_t out_len);


    //  Keyed hashing function of a Catena instance
    //     pwd:         password
    //     pwd_len:     length of password
    //     salt:        salt
    //     salt_len:    length of salt
    //     a_data:      associated data
    //     a_data_len:  length of associated data
    //     g_low:       minimum garlic
    //     g_high:      maximum garlic
    //     gamma:       public input
    //     gamma_len:   length of gamma
    //     key:         encryption key
    //     key_len:     length of key
    //     out:         output variable
    //     out_len:     length of output variable
    void KeyedHash(
        uint8_t *pwd,     size_t pwd_len,
        uint8_t *salt,    size_t salt_len,
        uint8_t *a_data,  size_t a_data_len,
        uint8_t g_low,    uint8_t g_high,
        uint8_t *gamma,   size_t gamma_len,
        uint8_t *key,     size_t key_len,
        uint8_t *user_id, size_t user_id_len,
        uint8_t *out,     uint16_t out_len);


    // Client-side server relief function of a Catena instance
    //     pwd:         password
    //     pwd_len:     length of password
    //     salt:        salt
    //     salt_len:    length of salt
    //     a_data:      associated data
    //     a_data_len:  length of associated data
    //     g_low:       minimum garlic
    //     g_high:      maximum garlic
    //     gamma:       public input
    //     gamma_len:   length of gamma
    //     out:         output variable
    //     out_len:     length of output variable
    void ServerReliefClient(
        uint8_t *pwd,    size_t pwd_len,
        uint8_t *salt,   size_t salt_len,
        uint8_t *a_data, size_t a_data_len,
        uint8_t g_low,   uint8_t g_high,
        uint8_t *gamma,  size_t gamma_len,
        uint8_t *out,    uint16_t out_len);


    //  Keyed client-side server relief function of a Catena instance
    //     pwd:         password
    //     pwd_len:     length of password
    //     salt:        salt
    //     salt_len:    length of salt
    //     a_data:      associated data
    //     a_data_len:  length of associated data
    //     g_low:       minimum garlic
    //     g_high:      maximum garlic
    //     gamma:       public input
    //     gamma_len:   length of gamma
    //     key:         encryption key
    //     key_len:     length of key
    //     out:         output variable
    //     out_len:     length of output variable
    void ServerReliefClientKeyed(
        uint8_t *pwd,     size_t pwd_len,
        uint8_t *salt,    size_t salt_len,
        uint8_t *a_data,  size_t a_data_len,
        uint8_t g_low,    uint8_t g_high,
        uint8_t *gamma,   size_t gamma_len,
        uint8_t *key,     size_t key_len,
        uint8_t *user_id, size_t user_id_len,
        uint8_t *out,     uint16_t out_len);


    // Server-side function for server relief feature
    void ServerReliefFinal(
        uint8_t *intermediate,
        uint8_t g_high,
        uint8_t *out, uint16_t out_len);


    // Server-side function for server relief feature with a key
    void KeyedServerReliefFinal(
        uint8_t *intermediate,
        uint8_t g_high,
        uint8_t *out,     uint16_t out_len,
        uint8_t *key,     size_t key_len,
        uint8_t *user_id, size_t user_id_len);


    // Client independent update
    uint8_t *Update(
        uint8_t *hash,  uint16_t hash_len,
        uint8_t g_low,  uint8_t g_high,
        uint8_t *gamma, size_t gamma_len);


    // Client independent update with key
    uint8_t *KeyedUpdate(
        uint8_t *hash,    uint16_t hash_len,
        uint8_t g_low,    uint8_t g_high,
        uint8_t *gamma,   size_t gamma_len,
        uint8_t *key,     size_t key_len,
        uint8_t *user_id, size_t user_id_len);


    // Key derivation mode of Catena
    uint8_t *GenKey(
        uint8_t *pwd,    size_t pwd_len,
        uint8_t *salt,   size_t salt_len,
        uint8_t *a_data, size_t a_data_len,
        uint8_t g_low,   uint8_t g_high,
        uint8_t *gamma,  size_t gamma_len,
        uint8_t *key_id, size_t key_id_len,
        uint8_t *key,    uint16_t key_len,
        uint16_t out_len);


    // Server side Proof of Work function.
    // Takes parameters and calculates hash and the parameters the client
    // is given to compute the proof
    //     pwd:        password
    //     salt:       salt
    //     a_data:     associated data
    //     g_low:      minimum garlic
    //     g_high:     maximum garlic
    //     gamma:      public input
    //     p:          number of secret bits
    //     mode:       0 for guessing secret bits in the salt (pepper)
    //                 1 for guessing secret bits in the password
    //     out_pwd:    out variable for the password (may be shortened)
    //     out_salt:   out variable for the salt (may be shortened)
    //     proof_out:  out variable for the hash that is used as proof
    void ProofOfWorkServer(
        uint8_t *pwd,       size_t pwd_len,
        uint8_t *salt,      size_t salt_len,
        uint8_t *a_data,    size_t a_data_len,
        uint8_t g_low,      uint8_t g_high,
        uint8_t *gamma,     size_t gamma_len,
        uint16_t p,         uint8_t mode,
        uint8_t *out_pwd,   size_t out_pwd_len,
        uint8_t *out_salt,  size_t out_salt_len,
        uint8_t *proof_out, uint16_t proof_out_len);


    // Client-side Proof of Work function for guessing the salt
    void ProofOfWorkPepper(
        uint8_t *pwd,      size_t pwd_len,
        uint8_t *salt,     size_t salt_len,
        uint8_t *a_data,   size_t a_data_len,
        uint8_t g_low,     uint8_t g_high,
        uint8_t *gamma,    size_t gamma_len,
        uint16_t p,        uint8_t mode,
        uint8_t *proof,    uint16_t proof_len,
        uint8_t *out_salt, uint16_t out_salt_len);


    // Client-side Proof of Work function for guessing the password
    void ProofOfWorkPassword(
        uint8_t *pwd,     size_t pwd_len,
        uint8_t *salt,    size_t salt_len,
        uint8_t *a_data,  size_t a_data_len,
        uint8_t g_low,    uint8_t g_high,
        uint8_t *gamma,   size_t gamma_len,
        uint16_t p,       uint8_t mode,
        uint8_t *proof,   uint16_t proof_len,
        uint8_t *out_pwd, uint16_t out_pwd_len);

 protected:
    VERSION version_id_;
    HashAlgorithm *h_;
    HashAlgorithm *h_prime_;
    catena::gamma::GammaAlgorithm *gamma_;
    catena::graph::GraphAlgorithm *f_;
    size_t n_;
    size_t k_;
    uint8_t lambda_;
    xorshift1024star::XorShift1024Star *xss_;

    // Returns the length of tweaks (n_**2 + 6)
    inline size_t CalcTweakLen();


    // Computes tweak ( H(V) + d + lambda + mode + sLen + H(AD) )
    //     mode:       mode of operation
    //     lambda:     depth of graph
    //     out_len:    length of catena output
    //     salt_len:   length of salt
    //     a_data:     associated data
    //     a_data_len: length of associated data
    //     out:        output variable
    void ComputeTweak(
        MODE mode,
        uint8_t lambda,
        uint16_t out_len,
        uint16_t salt_len,
        uint8_t *a_data,
        size_t a_data_len,
        uint8_t *out);


    // Generates initial hash of length k by using h.
    // Thus, we use the stronger hash function to generate a hash of the same
    // length as the weaker hash function (h_prime)
    //     data:  input data
    //     out:   output variable (make sure to allocate k_ bytes)
    void HInit(uint8_t *data, uint8_t *out);


    // Core function of Catena; calls gamma, graph and phi
    //     g:           garlic
    //     data:        output of HInit
    //     gamma:       some public input
    //     gamma_len:   length of gamma
    //     out:         output variable
    void Flap(
        uint8_t g,
        uint8_t *data,
        uint8_t *gamma,
        size_t gamma_len,
        uint8_t *out);


    // Encrypts a hash with a key
    //     hash:          current hash
    //     hash_len:      length of hash
    //     key:           key to be added
    //     key_len:       length of key
    //     user_id:       user id
    //     user_id_len:   length of user id
    //     g_high:        g_high
    uint8_t *AddKey(
        uint8_t *hash,    uint16_t hash_len,
        uint8_t *key,     size_t key_len,
        uint8_t *user_id, size_t user_id_len,
        uint8_t g_high);


    // Generates a Catena hash
    //     mode:        mode of operation (e.g. password scrambling)
    //     pwd:         password
    //     pwd_len:     length of password
    //     salt:        salt
    //     salt_len:    length of salt
    //     a_data:      associated data
    //     a_data_len:  length of associated data
    //     g_low:       minimum garlic
    //     g_high:      maximum garlic
    //     gamma:       public input
    //     gamma_len:   length of gamma
    //     out:         output variable
    //     out_len:     length of output variable
    void Hash(
        MODE mode,
        uint8_t *pwd,    size_t pwd_len,
        uint8_t *salt,   size_t salt_len,
        uint8_t *a_data, size_t a_data_len,
        uint8_t g_low,   uint8_t g_high,
        uint8_t *gamma,  size_t gamma_len,
        uint8_t *out,    uint16_t out_len);


    // Client-side server relief
    //     mode:        mode of operation (e.g. password scrambling)
    //     pwd:         password
    //     pwd_len:     length of password
    //     salt:        salt
    //     salt_len:    length of salt
    //     a_data:      associated data
    //     a_data_len:  length of associated data
    //     g_low:       minimum garlic
    //     g_high:      maximum garlic
    //     gamma:       public input
    //     gamma_len:   length of gamma
    //     out:         output variable
    //     out_len:     length of output variable
    void ServerRelief(
        MODE mode,
        uint8_t *pwd,    size_t pwd_len,
        uint8_t *salt,   size_t salt_len,
        uint8_t *a_data, size_t a_data_len,
        uint8_t g_low,   uint8_t g_high,
        uint8_t *gamma,  size_t gamma_len,
        uint8_t *out,    uint16_t out_len);
};
}  // namespace catena

#endif  // CATENA_INCLUDE_CATENA_H_

