#include "./catena.h"

#include <iostream>
#include "helpers/misc.h"

namespace catena {

  //****************************************************************************
  // PUBLIC
  //****************************************************************************

  Catena::Catena(VERSION version, uint8_t lambda,
                 HashAlgorithm *h, size_t n,
                 HashAlgorithm *h_prime, size_t k,
                 catena::gamma::GammaAlgorithm *gamma,
                 catena::graph::GraphAlgorithm *f) {
    if (n < 1 || k < 1 ||
        ((k / n) * n) != k ||
        f == NULL_PTR) {
      throw std::runtime_error("Invalid parameters");
    }

    version_id_ = version;
    lambda_ = lambda;

    h_ = h;
    n_ = n;
    h_prime_ = h_prime;
    k_ = k;

    gamma_ = gamma;
    f_ = f;
    xss_ = new xorshift1024star::XorShift1024Star{};
  }


  Catena::Catena(VERSION version, uint8_t lambda,
                 HashAlgorithm *h, size_t n,
                 HashAlgorithm *h_prime, size_t k,
                 catena::gamma::GammaAlgorithm *gamma,
                 catena::graph::GraphAlgorithm *f,
                 xorshift1024star::XorShift1024Star *xss) {
    if (n < 1 || k < 1 ||
        ((k / n) * n) != k ||
        f == NULL_PTR) {
      throw std::runtime_error("Invalid parameters");
    }

    version_id_ = version;
    lambda_ = lambda;

    h_ = h;
    n_ = n;
    h_prime_ = h_prime;
    k_ = k;

    gamma_ = gamma;
    f_ = f;
    xss_ = xss;
  }


  Catena::~Catena() {
    delete h_;
    delete xss_;

    if (h_prime_ != h_) {
      delete h_prime_;
    }

    delete gamma_;
    delete f_;
  }


  Catena *Catena::Dragonfly() {
    const size_t n = 64;
    const size_t k = 64;

    const uint8_t lambda = 2;

    Blake2b *h = new Blake2b(n);
    Blake2b_1 *h_prime = new Blake2b_1();
    catena::gamma::GammaAlgorithm *gamma =
        new catena::gamma::SaltMix(h, h_prime);
    catena::graph::GraphAlgorithm *f =
        new catena::graph::BitReversalGraph(h, n, h_prime, k, lambda);

    return new Catena(VERSION::CATENA_DRAGONFLY, lambda,
                      h, n, h_prime, k, gamma, f);
  }


  Catena *Catena::DragonflyFull() {
    const size_t n = 64;
    const uint8_t lambda = 2;

    Blake2b *blake2b = new Blake2b(n);
    catena::gamma::GammaAlgorithm *gamma =
        new catena::gamma::SaltMix(blake2b, blake2b);
    catena::graph::GraphAlgorithm *f =
        new catena::graph::BitReversalGraph(blake2b, n, blake2b, n, lambda);

    return new Catena(VERSION::CATENA_DRAGONFLY_FULL,
                      lambda, blake2b, n, blake2b, n, gamma, f);
  }


  Catena *Catena::Butterfly() {
    const size_t n = 64;
    const size_t k = 64;

    const uint8_t lambda = 4;

    Blake2b *h = new Blake2b(n);
    Blake2b_1 *h_prime = new Blake2b_1();
    catena::gamma::GammaAlgorithm *gamma =
        new catena::gamma::SaltMix(h, h_prime);
    catena::graph::GraphAlgorithm *f =
        new catena::graph::DoubleButterflyGraph(h, n, h_prime, k, lambda);

    return new Catena(VERSION::CATENA_BUTTERFLY,
                      lambda, h, n, h_prime, k, gamma, f);
  }


  Catena *Catena::ButterflyFull() {
    const size_t n = 64;
    const uint8_t lambda = 4;

    Blake2b *blake2b = new Blake2b(n);
    catena::gamma::GammaAlgorithm *gamma =
        new catena::gamma::SaltMix(blake2b, blake2b);
    catena::graph::GraphAlgorithm *f =
        new catena::graph::DoubleButterflyGraph(blake2b, n,
                                                 blake2b, n, lambda);

    return new Catena(VERSION::CATENA_BUTTERFLY_FULL,
                      lambda, blake2b, n, blake2b, n, gamma, f);
  }


  Catena *Catena::MartyMcFly() {
    const size_t n = 64;
    const size_t k = 1024;
    const uint8_t lambda = 1;
    xorshift1024star::XorShift1024Star *xss =
                         new xorshift1024star::XorShift1024Star{};

    Blake2b *blake2b = new Blake2b(n);
    BlaMka *h_prime = new BlaMka();
    catena::gamma::GammaAlgorithm *gamma = NULL_PTR;
    catena::graph::GraphAlgorithm *f =
        new catena::graph::DRSampleGraph(blake2b, n,
                                         h_prime, k,
                                         lambda, xss);

    return new Catena(VERSION::CATENA_MARTYMCFLY,
                      lambda, blake2b, n, h_prime, k, gamma, f, xss);
  }


  Catena *Catena::MartyMcFlyFull() {
    const size_t n = 64;
    const uint8_t lambda = 1;
    xorshift1024star::XorShift1024Star *xss =
                         new xorshift1024star::XorShift1024Star{};

    Blake2b *blake2b = new Blake2b(n);
    catena::gamma::GammaAlgorithm *gamma = NULL_PTR;
    catena::graph::GraphAlgorithm *f =
        new catena::graph::DRSampleGraph(blake2b, n,
                                         blake2b, n,
                                         lambda, xss);

    return new Catena(VERSION::CATENA_MARTYMCFLY_FULL,
                      lambda, blake2b, n, blake2b, n, gamma, f, xss);
  }


  void Catena::Hash(
      uint8_t *pwd, size_t pwd_len,
      uint8_t *salt, size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low, uint8_t g_high,
      uint8_t *gamma, size_t gamma_len,
      uint8_t *out, uint16_t out_len) {
    Hash(
        MODE::PASSWORD_SCRAMBLING,
        pwd, pwd_len,
        salt, salt_len,
        a_data, a_data_len,
        g_low, g_high,
        gamma, gamma_len,
        out, out_len);
  }


  void Catena::KeyedHash(
      uint8_t *pwd, size_t pwd_len,
      uint8_t *salt, size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low, uint8_t g_high,
      uint8_t *gamma, size_t gamma_len,
      uint8_t *key, size_t key_len,
      uint8_t *user_id, size_t user_id_len,
      uint8_t *out, uint16_t out_len) {
    Hash(
        pwd, pwd_len,
        salt, salt_len,
        a_data, a_data_len,
        g_low, g_high,
        gamma, gamma_len,
        out, out_len);

    AddKey(out, out_len, key, key_len, user_id, user_id_len, g_high);
  }


  void Catena::ServerReliefClient(
      uint8_t *pwd, size_t pwd_len,
      uint8_t *salt, size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low, uint8_t g_high,
      uint8_t *gamma, size_t gamma_len,
      uint8_t *out, uint16_t out_len) {
    ServerRelief(
        MODE::PASSWORD_SCRAMBLING,
        pwd, pwd_len,
        salt, salt_len,
        a_data, a_data_len,
        g_low, g_high,
        gamma, gamma_len,
        out, out_len);
  }


  void Catena::ServerReliefClientKeyed(
      uint8_t *pwd, size_t pwd_len,
      uint8_t *salt, size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low, uint8_t g_high,
      uint8_t *gamma, size_t gamma_len,
      uint8_t *key, size_t key_len,
      uint8_t *user_id, size_t user_id_len,
      uint8_t *out, uint16_t out_len) {
    ServerRelief(
        MODE::PASSWORD_SCRAMBLING,
        pwd, pwd_len,
        salt, salt_len,
        a_data, a_data_len,
        g_low, g_high,
        gamma, gamma_len,
        out, out_len);

    AddKey(out, out_len, key, key_len, user_id, user_id_len, g_high);
  }


  void Catena::ServerReliefFinal(
      uint8_t *intermediate,
      uint8_t g_high,
      uint8_t *out, uint16_t out_len) {
    uint8_t *buffer = reinterpret_cast<uint8_t*>(MALLOC(k_ + 1));
    buffer[0] = g_high;
    MEMCPY(buffer + 1, intermediate, k_);

    uint8_t *hash = reinterpret_cast<uint8_t*>(MALLOC(n_));
    h_->Hash(buffer, k_ + 1, hash);
    MEMCPY(out, hash, out_len);

    FREE(hash);
    FREE(buffer);
  }


  void Catena::KeyedServerReliefFinal(
      uint8_t *intermediate,
      uint8_t g_high,
      uint8_t *out,     uint16_t out_len,
      uint8_t *key,     size_t key_len,
      uint8_t *user_id, size_t user_id_len) {
    ServerReliefFinal(intermediate, g_high, out, out_len);
    AddKey(out, out_len, key, key_len, user_id, user_id_len, g_high);
  }


  uint8_t *Catena::Update(
      uint8_t *hash,  uint16_t hash_len,
      uint8_t g_low,  uint8_t g_high,
      uint8_t *gamma, size_t gamma_len) {
    size_t buf_len = k_ + 1;
    size_t trunc_len = n_ - hash_len;

    uint8_t *buf =  reinterpret_cast<uint8_t*>(MALLOC(buf_len));
    uint8_t *x =    reinterpret_cast<uint8_t*>((size_t)buf + 1);
    uint8_t *fill = reinterpret_cast<uint8_t*>((size_t)buf + 1 + hash_len);

    MEMCPY(x, hash, hash_len);
    MEMSET(fill, 0x00, trunc_len);

    for (uint8_t g = g_low + 1; g <= g_high; ++g) {
      Flap(g, x, gamma, gamma_len, x);

      buf[0] = g;

      h_->Hash(buf, buf_len, x);
      MEMSET(fill, 0x00, trunc_len);
    }

    MEMCPY(hash, buf+1, hash_len);

    FREE(buf);

    return hash;
  }


  uint8_t *Catena::KeyedUpdate(
      uint8_t *hash,    uint16_t hash_len,
      uint8_t g_low,    uint8_t g_high,
      uint8_t *gamma,   size_t gamma_len,
      uint8_t *key,     size_t key_len,
      uint8_t *user_id, size_t user_id_len) {
    AddKey(hash, hash_len, key, key_len, user_id, user_id_len, g_high);
    Update(hash, hash_len, g_low, g_high, gamma, gamma_len);

    return AddKey(hash, hash_len, key, key_len, user_id, user_id_len, g_high);
  }


  uint8_t *Catena::GenKey(
      uint8_t *pwd,    size_t pwd_len,
      uint8_t *salt,   size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low,   uint8_t g_high,
      uint8_t *gamma,  size_t gamma_len,
      uint8_t *key_id, size_t key_id_len,
      uint8_t *key,    uint16_t key_len,
      uint16_t out_len) {
    uint16_t len = (key_len + n_ - 1) / n_;

    uint8_t *buf = reinterpret_cast<uint8_t*>(MALLOC(len * n_));
    uint8_t *x =   reinterpret_cast<uint8_t*>(MALLOC(out_len));
    uint8_t *key_ptr = buf;

    uint16_t *temp = reinterpret_cast<uint16_t*>(MALLOC(2));

    Hash(MODE::KEY_DERIVIATION,
         pwd, pwd_len,
         salt, salt_len,
         a_data, a_data_len,
         g_low, g_high,
         gamma, gamma_len,
         x, out_len);

    for (uint16_t i = 1; i <= len; ++i) {
      temp[0] = i;

      h_->Update(temp, 2);
      h_->Update(key_id, key_id_len);

      temp[0] = key_len;

      h_->Update(temp, 2);
      h_->Update(x, out_len);

      h_->Final(key_ptr);
      key_ptr = reinterpret_cast<uint8_t*>(PTR_TO(key_ptr, 1, n_));
    }

    MEMCPY(key, buf, key_len);
    FREE(buf);
    FREE(x);
    FREE(temp);

    return key;
  }


  void Catena::ProofOfWorkServer(
      uint8_t *pwd,    size_t pwd_len,
      uint8_t *salt,   size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low,   uint8_t g_high,
      uint8_t *gamma,  size_t gamma_len,
      uint16_t p,
      uint8_t mode,
      uint8_t *out_pwd,   size_t out_pwd_len,
      uint8_t *out_salt,  size_t out_salt_len,
      uint8_t *out_proof, uint16_t out_proof_len) {
    uint8_t* tmp_pwd = reinterpret_cast<uint8_t*>(MALLOC(pwd_len));
    for (size_t i = 0; i < pwd_len; ++i) {
      tmp_pwd[i] = pwd[i];
    }
    Hash(MODE::PROOF_OF_WORK,
         pwd, pwd_len,
         salt, salt_len,
         a_data, a_data_len,
         g_low, g_high,
         gamma, gamma_len,
         out_proof, out_proof_len);
    out_salt_len = salt_len;
    if (mode == 0) {
      for (size_t i = 0; i < pwd_len; ++i) {
        out_pwd[i] = tmp_pwd[i];
      }
      out_pwd = tmp_pwd;
      out_pwd_len = pwd_len;

      // Compute salt shortened by t bits

      // number of bytes that are affected by p
      size_t num_p_bytes = (p + 7) / 8;
      // not affected by p
      size_t num_public_bytes = salt_len - num_p_bytes;

      // the last num_p_bytes - 1 bytes can be set to 0
      for (size_t i = 1; i < num_p_bytes; ++i) {
        out_salt[salt_len - i] = 0;
      }
      // the first salt_len - num_p_bytes - 1 bytes can be set to the real value
      for (size_t i = 0; i < num_public_bytes; ++i) {
        out_salt[i] = salt[i];
      }
      // only one byte contains only parts of the real salt: s & 11111000 for
      // p = 3
      uint8_t num_bits = p - ((num_p_bytes - 1) * 8);
      uint8_t and_val = 256 - (1 << num_bits);
      out_salt[num_public_bytes] = salt[num_public_bytes] & and_val;
    } else if (mode == 1) {
      out_salt = salt;
      out_salt_len = salt_len;
      out_pwd = {};
      out_pwd_len = 0;
    }
    FREE(tmp_pwd);
  }


  void Catena::ProofOfWorkPepper(
      uint8_t *pwd,    size_t pwd_len,
      uint8_t *salt,   size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low,   uint8_t g_high,
      uint8_t *gamma,  size_t gamma_len,
      uint16_t p,
      uint8_t mode,
      uint8_t *proof,    uint16_t proof_len,
      uint8_t *out_salt, uint16_t out_salt_len) {
    uint16_t upper_bound = 1 << p;
    uint8_t* tmp_hash = reinterpret_cast<uint8_t*>(MALLOC(proof_len));

    uint8_t num_bytes = (p / 8) + 1;
    unsigned int seed = (unsigned int)(time(NULL));
    uint64_t random_offset = (rand_r(&seed)) % upper_bound;
    uint8_t* tmp_salt = reinterpret_cast<uint8_t*>(MALLOC(salt_len));
    // because HASH() sets pwd to 00*
    uint8_t* tmp_pwd = reinterpret_cast<uint8_t*>(MALLOC(pwd_len));
    uint8_t step = 1;

    uint64_t test = random_offset;

    for (size_t i = 0; i < upper_bound; ++i) {
      // resetting tmp_pwd since it gets overwritten in Hash()
      for (size_t idx = 0; idx < pwd_len; ++idx) {
        tmp_pwd[idx] = pwd[idx];
      }

      // Set tmp_salt to testing value.  Since we only cut the last p bits we
      // can simply add without having to worry about carry bits.
      size_t s_len_minus_one = salt_len - 1;
      for (size_t idx = 0; idx < salt_len - num_bytes; ++idx) {
        tmp_salt[idx] = salt[idx];
      }
      for (size_t idx = salt_len - num_bytes; idx < salt_len; ++idx) {
        tmp_salt[idx] =
          salt[idx] + (test & (0xFF << ((s_len_minus_one - idx) * 8)));
      }

      Hash(MODE::PROOF_OF_WORK,
           tmp_pwd, pwd_len,
           tmp_salt, salt_len,
           a_data, a_data_len,
           g_low, g_high,
           gamma, gamma_len,
           tmp_hash, proof_len);

      if (uintx::Equal(tmp_hash, proof, proof_len)) {
        break;
      }

      test = (test + step) % upper_bound;
    }

    for (size_t idx = 0; idx < salt_len; ++idx) {
      out_salt[idx] = tmp_salt[idx];
    }

    if (!uintx::Equal(tmp_hash, proof, proof_len)) {
      throw std::runtime_error("PROOF OF WORK: CAN NOT FIND VALID SOLUTION!");
    }

    FREE(tmp_hash);
    FREE(tmp_salt);
    FREE(tmp_pwd);
  }


  void Catena::ProofOfWorkPassword(
      uint8_t *pwd,    size_t pwd_len,
      uint8_t *salt,   size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low,   uint8_t g_high,
      uint8_t *gamma,  size_t gamma_len,
      uint16_t p,
      uint8_t mode,
      uint8_t *proof,   uint16_t proof_len,
      uint8_t *out_pwd, uint16_t out_pwd_len) {
    uint16_t upper_bound = 1 << p;
    uint8_t* tmp_hash = reinterpret_cast<uint8_t*>(MALLOC(proof_len));

    out_pwd_len = (p + 7) / 8;
    unsigned int seed = (unsigned int)(time(NULL));
    uint8_t random_offset = rand_r(&seed) % upper_bound;
    uint8_t* tmp_pwd = reinterpret_cast<uint8_t*>(MALLOC(out_pwd_len));

    uint64_t test = random_offset;

    for (size_t i = 0; i < upper_bound; ++i) {
      for (uint8_t idx = 0; idx < out_pwd_len; ++idx) {
        tmp_pwd[idx] = ((test >> (idx * 8)) & 0xFF);
      }

      Hash(MODE::PROOF_OF_WORK,
           tmp_pwd, out_pwd_len,
           salt, salt_len,
           a_data, a_data_len,
           g_low, g_high,
           gamma, gamma_len,
           tmp_hash, proof_len);

      if (uintx::Equal(tmp_hash, proof, proof_len)) {
        break;
      }

      test = (test + 1) % upper_bound;
    }

    for (uint8_t idx = 0; idx < out_pwd_len; ++idx) {
      out_pwd[idx] = ((test >> (idx * 8)) & 0xFF);
    }

    if (!uintx::Equal(tmp_hash, proof, proof_len)) {
      throw std::runtime_error("PROOF OF WORK: CAN NOT FIND VALID SOLUTION!");
    }

    FREE(tmp_hash);
    FREE(tmp_pwd);
  }

  //****************************************************************************
  // PROTECTED
  //****************************************************************************

  inline size_t Catena::CalcTweakLen() {
    return (n_ << 1) + 6;
  }


  void Catena::ComputeTweak(
      MODE mode,
      uint8_t lambda,
      uint16_t out_len,
      uint16_t salt_len,
      uint8_t *a_data,
      size_t a_data_len,
      uint8_t *out) {
    char *version_id;
    size_t version_id_len;

    switch (version_id_) {
      case VERSION::CATENA_DRAGONFLY:
        version_id = new char[9]{ 'D', 'r', 'a', 'g', 'o', 'n', 'f', 'l', 'y' };
        version_id_len = 9;
        break;
      case VERSION::CATENA_DRAGONFLY_FULL:
        version_id = new char[14]{ 'D', 'r', 'a', 'g', 'o', 'n', 'f', 'l', 'y',
                                   '-', 'F', 'u', 'l', 'l' };
        version_id_len = 14;
        break;
      case VERSION::CATENA_BUTTERFLY:
        version_id = new char[9]{ 'B', 'u', 't', 't', 'e', 'r', 'f', 'l', 'y' };
        version_id_len = 9;
        break;
      case VERSION::CATENA_BUTTERFLY_FULL:
        version_id = new char[14]{ 'B', 'u', 't', 't', 'e', 'r', 'f', 'l', 'y',
                                   '-', 'F', 'u', 'l', 'l' };
        version_id_len = 14;
        break;
      case VERSION::CATENA_MARTYMCFLY:
        version_id = new char[11]{ 'M', 'a', 'r', 't', 'y', '-', 'M', 'c', 'F',
                                    'l', 'y'};
        version_id_len = 11;
        break;
      case VERSION::CATENA_MARTYMCFLY_FULL:
        version_id = new char[16]{ 'M', 'a', 'r', 't', 'y', '-', 'M', 'c', 'F',
                                    'l', 'y', '-', 'F', 'u', 'l', 'l'};
        version_id_len = 16;
        break;
      default:
        out = NULL_PTR;
        return;
    }

    h_->Hash(version_id, version_id_len, out);

    delete [] version_id;

    *reinterpret_cast<uint8_t*>((size_t)out + n_) = (uint8_t)mode;
    *reinterpret_cast<uint8_t*>((size_t)out + n_ + 1) = lambda;
    *reinterpret_cast<uint16_t*>((size_t)out + n_ + 2) = LE(out_len);
    *reinterpret_cast<uint32_t*>((size_t)out + n_ + 4) = LE(salt_len);
    h_->Hash(a_data, a_data_len, PTR_TO(out, n_ + 6, 1));
  }


  void Catena::HInit(uint8_t *data, uint8_t *out) {
    size_t x_len = n_ + 1;
    uint8_t *x = reinterpret_cast<uint8_t*>(MALLOC(x_len));

    MEMCPY(PTR_TO(x, 1, 1), data, n_);

    uint8_t len = k_ / n_;
    uint8_t *out_ptr = out;

    for (uint8_t i = 0; i < len; ++i) {
      x[0] = i;
      h_->Hash(x, x_len, out_ptr);

      // Should that be (out_ptr, i*n, n_)?
      out_ptr = reinterpret_cast<uint8_t*>(PTR_TO(out_ptr, 1, n_));
    }

    for (uint8_t i = 0; i < len; ++i) {
      x[0] = i + len;
      h_->Hash(x, x_len, out_ptr);
      out_ptr = reinterpret_cast<uint8_t*>(PTR_TO(out_ptr, 1, n_));
    }

    FREE(x);
  }


  void Catena::Flap(
      uint8_t g,
      uint8_t *data,
      uint8_t *gamma,
      size_t gamma_len,
      uint8_t *out) {
    uint64_t len = 1 << g;

    uint8_t *v = reinterpret_cast<uint8_t*>(MALLOC((len + 2) * k_));
    uint8_t *v_2 = v;
    uint8_t *v_1 = reinterpret_cast<uint8_t*>(PTR_TO(v_2, 1, k_));
    uint8_t *v_0;
    uint8_t *v_first = reinterpret_cast<uint8_t*>(PTR_TO(v_1, 1, k_));

    HInit(data, v);
    h_prime_->Reset();

    for (uint64_t i = 0; i < len; ++i) {
      v_0 = reinterpret_cast<uint8_t*>(PTR_TO(v, (i + 2), k_));

      h_prime_->Update(v_1, k_);
      h_prime_->Update(v_2, k_);
      h_prime_->Final(v_0);
      v_2 = v_1;
      v_1 = v_0;
    }

    if (gamma_ != NULL_PTR) {
      h_prime_->Reset();
      gamma_->Execute(g, reinterpret_cast<uint8_t*>(v_first), gamma, gamma_len);
    }

    h_prime_->Reset();
    f_->Execute(g, v_first);

    MEMCPY(out, v_1, k_);
    FREE(v);
  }


  uint8_t *Catena::AddKey(
      uint8_t *hash,    uint16_t hash_len,
      uint8_t *key,     size_t key_len,
      uint8_t *user_id, size_t user_id_len,
      uint8_t g_high) {
    uint8_t *key_stream = reinterpret_cast<uint8_t*>(MALLOC(n_));

    key_stream[0] = g_high;

    h_->Update(key, key_len);
    h_->Update(user_id, user_id_len);
    h_->Update(key_stream, 1);
    h_->Update(key, key_len);
    h_->Final(key_stream);

    uintx::XorArray(hash,
                    key_stream,
                    hash_len,
                    hash);

    FREE(key_stream);

    return hash;
  }


  void Catena::Hash(
      MODE mode,
      uint8_t *pwd,    size_t pwd_len,
      uint8_t *salt,   size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low,   uint8_t g_high,
      uint8_t *gamma,  size_t gamma_len,
      uint8_t *out,    uint16_t out_len) {
    // check for invalid inputs
    if ((pwd == NULL_PTR && pwd_len > 0) ||
        (salt == NULL_PTR && salt_len > 0) ||
        (a_data == NULL_PTR && a_data_len > 0) ||
        g_low < 1 || g_low > g_high || g_high < 1 ||
        g_high > (sizeof(size_t) * 8 - 1) ||
        (gamma == NULL_PTR && gamma_len > 0) ||
        out_len < 1) {
      throw std::runtime_error("Invalid inputs");
    }

    size_t tweak_len = CalcTweakLen();
    size_t buffer_len = k_ + 1;
    size_t trunc_len = n_ - out_len;

    uint8_t *tweak     = reinterpret_cast<uint8_t*>(MALLOC(tweak_len));
    // buffer is g || x, where x is the hashed output of flap
    uint8_t *buffer    = reinterpret_cast<uint8_t*>(MALLOC(buffer_len));
    uint8_t *x         = reinterpret_cast<uint8_t*>((size_t)buffer + 1);
    uint8_t *x_out_len = reinterpret_cast<uint8_t*>((size_t)x + out_len);

    ComputeTweak(
        mode,
        lambda_,
        out_len,
        salt_len,
        a_data,
        a_data_len,
        tweak);

    // initialize random seed with tweak
    uint64_t *seed = new uint64_t[16];
    uint64_t *mem_seed = new uint64_t[16];
    uint8_t num_hashes_in_seed = (128 / n_);
    uint8_t *tmp = reinterpret_cast<uint8_t*>(MALLOC(num_hashes_in_seed * n_));
    h_prime_->Reset();
    h_->Update(tweak, tweak_len);

    for (size_t i = 0; i < num_hashes_in_seed; ++i) {
      uint8_t* tmp_x = reinterpret_cast<uint8_t*>((size_t)tmp + (i * n_));
      h_->Final(tmp_x);
      h_->Update(tmp, (i + 1) * n_);
    }

    for (size_t i = 0; i < 16; ++i) {
      uint64_t tmp_seed = 0;
      for (size_t j = 0; j < 8; ++j) {
        tmp_seed += (uint64_t) tmp[i * 8 + j] << (8 * (7 - j));
      }
      seed[i] = tmp_seed;
    }
    MEMCPY(mem_seed, seed, 128);
    xss_->Seed(seed);
    // clean buffer of h
    h_->Final(tmp);

    h_->Update(tweak, tweak_len);
    h_->Update(pwd, pwd_len);
    h_->Update(salt, salt_len);
    h_->Final(x);

    // clear pwd from memory
    MEMSET(pwd, 0, pwd_len);

    Flap((g_low + 1) >> 1, x, gamma, gamma_len, x);

    h_->Hash(x, k_, x);
    for (uint8_t g = g_low; g <= g_high; ++g) {
      MEMCPY(seed, mem_seed, 128);
      xss_->Seed(seed);
      Flap(g, x, gamma, gamma_len, x);
      buffer[0] = g;

      h_->Hash(buffer, buffer_len, x);
      // truncate to out_len
      MEMSET(x_out_len, 0, trunc_len);
    }

    MEMCPY(out, x, out_len);
    delete[] seed;
    delete[] mem_seed;
    FREE(buffer);
    FREE(tweak);
    FREE(tmp);
  }


  void Catena::ServerRelief(
      MODE mode,
      uint8_t *pwd,    size_t pwd_len,
      uint8_t *salt,   size_t salt_len,
      uint8_t *a_data, size_t a_data_len,
      uint8_t g_low,   uint8_t g_high,
      uint8_t *gamma,  size_t gamma_len,
      uint8_t *out,    uint16_t out_len) {
    // check for invalid inputs
    if ((pwd == NULL_PTR && pwd_len > 0) ||
        (salt == NULL_PTR && salt_len > 0) ||
        (a_data == NULL_PTR && a_data_len > 0) ||
        g_low < 1 || g_low > g_high || g_high < 1 ||
        g_high > (sizeof(size_t) * 8 - 1) ||
        (gamma == NULL_PTR && gamma_len > 0) ||
        out_len < 1) {
      throw std::runtime_error("Invalid inputs");
    }
    size_t tweak_len = CalcTweakLen();
    size_t buffer_len = k_ + 1;
    size_t trunc_len = n_ - out_len;

    uint8_t *tweak     = reinterpret_cast<uint8_t*>(MALLOC(tweak_len));
    // buffer is g || x, where x is the hashed output of flap
    uint8_t *buffer    = reinterpret_cast<uint8_t*>(MALLOC(buffer_len));
    uint8_t *x         = reinterpret_cast<uint8_t*>((size_t)buffer + 1);
    uint8_t *x_out_len = reinterpret_cast<uint8_t*>((size_t)x + out_len);

    ComputeTweak(
        mode,
        lambda_,
        out_len,
        salt_len,
        a_data,
        a_data_len,
        tweak);

    // initialize random seed with tweak
    uint64_t *seed = new uint64_t[16];
    uint64_t *mem_seed = new uint64_t[16];
    uint8_t num_hashes_in_seed = (128 / n_);
    uint8_t *tmp = reinterpret_cast<uint8_t*>(MALLOC(num_hashes_in_seed * n_));
    h_prime_->Reset();
    h_->Update(tweak, tweak_len);

    for (size_t i = 0; i < num_hashes_in_seed; ++i) {
      uint8_t* tmp_x = reinterpret_cast<uint8_t*>((size_t)tmp + (i * n_));
      h_->Final(tmp_x);
      h_->Update(tmp, (i + 1) * n_);
    }

    for (size_t i = 0; i < 16; ++i) {
      uint64_t tmp_seed = 0;
      for (size_t j = 0; j < 8; ++j) {
        tmp_seed += (uint64_t) tmp[i * 8 + j] << (8 * (7 - j));
      }
      seed[i] = tmp_seed;
    }
    MEMCPY(mem_seed, seed, 128);
    xss_->Seed(seed);
    // clean buffer of h
    h_->Final(tmp);

    h_->Update(tweak, tweak_len);
    h_->Update(pwd, pwd_len);
    h_->Update(salt, salt_len);
    h_->Final(x);

    // clear pwd from memory
    MEMSET(pwd, 0, pwd_len);

    Flap((g_low + 1) >> 1, x, gamma, gamma_len, x);

    h_->Hash(x, k_, x);
    for (uint8_t g = g_low; g < g_high; ++g) {
      MEMCPY(seed, mem_seed, 128);
      xss_->Seed(seed);
      Flap(g, x, gamma, gamma_len, x);
      buffer[0] = g;

      h_->Hash(buffer, buffer_len, x);
      // truncate to out_len
      MEMSET(x_out_len, 0, trunc_len);
    }
    MEMCPY(seed, mem_seed, 128);
    xss_->Seed(seed);
    Flap(g_high, x, gamma, gamma_len, x);
    MEMCPY(out, x, buffer_len - 1);

    delete[] seed;
    delete[] mem_seed;
    FREE(buffer);
    FREE(tweak);
    FREE(tmp);
  }
}  // namespace catena
