#include "hash/blake2b.h"
#include <iostream>

uint64_t Blake2b::Rotr64(const uint64_t val, const uint8_t bits) {
  return (val >> bits) | (val << (64 - bits));
}

void Blake2b::AddToT(uint8_t val) {
  uint64_t temp = t_[0];
  t_[0] += val;

  if (t_[0] < temp) {
    ++t_[1];
  }
}

void Blake2b::Mix(uint64_t *a, uint64_t *b, uint64_t *c, uint64_t *d,
                  const uint64_t x, const uint64_t y) {
  a += *b + x;
  d = reinterpret_cast<uint64_t*>(Rotr64(*d ^ *a, BLAKE2B_R1));
  c += *d;
  b = reinterpret_cast<uint64_t*>(Rotr64(*b ^ *c, BLAKE2B_R2));
  a += *b + y;
  d = reinterpret_cast<uint64_t*>(Rotr64(*d ^ *a, BLAKE2B_R3));
  c += *d;
  b = reinterpret_cast<uint64_t*>(Rotr64(*b ^ *c, BLAKE2B_R4));
}

void Blake2b::Compress(bool is_last_block) {
  uint64_t v[16] = {
    h_[0], h_[1], h_[2], h_[3], h_[4], h_[5], h_[6], h_[7],
    BLAKE2B_IV_0, BLAKE2B_IV_1, BLAKE2B_IV_2, BLAKE2B_IV_3,
    BLAKE2B_IV_4 ^ t_[0],  // v[12]
    BLAKE2B_IV_5 ^ t_[1],  // v[13]
    (is_last_block ? ~(uint64_t)BLAKE2B_IV_6 : BLAKE2B_IV_6),  // v[14]
    BLAKE2B_IV_7
  };

#ifdef BIG_ENDIAN_PLATFORM
  for (uint8_t i = 0; i < 16; ++i) {
    b_[i] = LE(b_[i]);
  }
#endif

  BLAKE2B_MIX_INIT

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_00_00], b_[BLAKE2B_SIGMA_00_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_00_02], b_[BLAKE2B_SIGMA_00_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_00_04], b_[BLAKE2B_SIGMA_00_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_00_06], b_[BLAKE2B_SIGMA_00_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_00_08], b_[BLAKE2B_SIGMA_00_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_00_10], b_[BLAKE2B_SIGMA_00_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_00_12], b_[BLAKE2B_SIGMA_00_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_00_14], b_[BLAKE2B_SIGMA_00_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_01_00], b_[BLAKE2B_SIGMA_01_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_01_02], b_[BLAKE2B_SIGMA_01_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_01_04], b_[BLAKE2B_SIGMA_01_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_01_06], b_[BLAKE2B_SIGMA_01_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_01_08], b_[BLAKE2B_SIGMA_01_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_01_10], b_[BLAKE2B_SIGMA_01_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_01_12], b_[BLAKE2B_SIGMA_01_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_01_14], b_[BLAKE2B_SIGMA_01_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_02_00], b_[BLAKE2B_SIGMA_02_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_02_02], b_[BLAKE2B_SIGMA_02_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_02_04], b_[BLAKE2B_SIGMA_02_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_02_06], b_[BLAKE2B_SIGMA_02_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_02_08], b_[BLAKE2B_SIGMA_02_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_02_10], b_[BLAKE2B_SIGMA_02_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_02_12], b_[BLAKE2B_SIGMA_02_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_02_14], b_[BLAKE2B_SIGMA_02_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_03_00], b_[BLAKE2B_SIGMA_03_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_03_02], b_[BLAKE2B_SIGMA_03_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_03_04], b_[BLAKE2B_SIGMA_03_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_03_06], b_[BLAKE2B_SIGMA_03_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_03_08], b_[BLAKE2B_SIGMA_03_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_03_10], b_[BLAKE2B_SIGMA_03_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_03_12], b_[BLAKE2B_SIGMA_03_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_03_14], b_[BLAKE2B_SIGMA_03_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_04_00], b_[BLAKE2B_SIGMA_04_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_04_02], b_[BLAKE2B_SIGMA_04_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_04_04], b_[BLAKE2B_SIGMA_04_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_04_06], b_[BLAKE2B_SIGMA_04_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_04_08], b_[BLAKE2B_SIGMA_04_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_04_10], b_[BLAKE2B_SIGMA_04_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_04_12], b_[BLAKE2B_SIGMA_04_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_04_14], b_[BLAKE2B_SIGMA_04_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_05_00], b_[BLAKE2B_SIGMA_05_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_05_02], b_[BLAKE2B_SIGMA_05_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_05_04], b_[BLAKE2B_SIGMA_05_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_05_06], b_[BLAKE2B_SIGMA_05_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_05_08], b_[BLAKE2B_SIGMA_05_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_05_10], b_[BLAKE2B_SIGMA_05_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_05_12], b_[BLAKE2B_SIGMA_05_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_05_14], b_[BLAKE2B_SIGMA_05_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_06_00], b_[BLAKE2B_SIGMA_06_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_06_02], b_[BLAKE2B_SIGMA_06_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_06_04], b_[BLAKE2B_SIGMA_06_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_06_06], b_[BLAKE2B_SIGMA_06_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_06_08], b_[BLAKE2B_SIGMA_06_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_06_10], b_[BLAKE2B_SIGMA_06_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_06_12], b_[BLAKE2B_SIGMA_06_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_06_14], b_[BLAKE2B_SIGMA_06_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_07_00], b_[BLAKE2B_SIGMA_07_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_07_02], b_[BLAKE2B_SIGMA_07_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_07_04], b_[BLAKE2B_SIGMA_07_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_07_06], b_[BLAKE2B_SIGMA_07_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_07_08], b_[BLAKE2B_SIGMA_07_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_07_10], b_[BLAKE2B_SIGMA_07_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_07_12], b_[BLAKE2B_SIGMA_07_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_07_14], b_[BLAKE2B_SIGMA_07_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_08_00], b_[BLAKE2B_SIGMA_08_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_08_02], b_[BLAKE2B_SIGMA_08_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_08_04], b_[BLAKE2B_SIGMA_08_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_08_06], b_[BLAKE2B_SIGMA_08_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_08_08], b_[BLAKE2B_SIGMA_08_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_08_10], b_[BLAKE2B_SIGMA_08_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_08_12], b_[BLAKE2B_SIGMA_08_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_08_14], b_[BLAKE2B_SIGMA_08_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_09_00], b_[BLAKE2B_SIGMA_09_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_09_02], b_[BLAKE2B_SIGMA_09_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_09_04], b_[BLAKE2B_SIGMA_09_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_09_06], b_[BLAKE2B_SIGMA_09_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_09_08], b_[BLAKE2B_SIGMA_09_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_09_10], b_[BLAKE2B_SIGMA_09_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_09_12], b_[BLAKE2B_SIGMA_09_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_09_14], b_[BLAKE2B_SIGMA_09_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_00_00], b_[BLAKE2B_SIGMA_00_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_00_02], b_[BLAKE2B_SIGMA_00_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_00_04], b_[BLAKE2B_SIGMA_00_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_00_06], b_[BLAKE2B_SIGMA_00_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_00_08], b_[BLAKE2B_SIGMA_00_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_00_10], b_[BLAKE2B_SIGMA_00_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_00_12], b_[BLAKE2B_SIGMA_00_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_00_14], b_[BLAKE2B_SIGMA_00_15]);

  BLAKE2B_MIX(v[0], v[4], v[8], v[12],
              b_[BLAKE2B_SIGMA_01_00], b_[BLAKE2B_SIGMA_01_01]);
  BLAKE2B_MIX(v[1], v[5], v[9], v[13],
              b_[BLAKE2B_SIGMA_01_02], b_[BLAKE2B_SIGMA_01_03]);
  BLAKE2B_MIX(v[2], v[6], v[10], v[14],
              b_[BLAKE2B_SIGMA_01_04], b_[BLAKE2B_SIGMA_01_05]);
  BLAKE2B_MIX(v[3], v[7], v[11], v[15],
              b_[BLAKE2B_SIGMA_01_06], b_[BLAKE2B_SIGMA_01_07]);
  BLAKE2B_MIX(v[0], v[5], v[10], v[15],
              b_[BLAKE2B_SIGMA_01_08], b_[BLAKE2B_SIGMA_01_09]);
  BLAKE2B_MIX(v[1], v[6], v[11], v[12],
              b_[BLAKE2B_SIGMA_01_10], b_[BLAKE2B_SIGMA_01_11]);
  BLAKE2B_MIX(v[2], v[7], v[8], v[13],
              b_[BLAKE2B_SIGMA_01_12], b_[BLAKE2B_SIGMA_01_13]);
  BLAKE2B_MIX(v[3], v[4], v[9], v[14],
              b_[BLAKE2B_SIGMA_01_14], b_[BLAKE2B_SIGMA_01_15]);

  h_[0] ^= v[0] ^ v[8];
  h_[1] ^= v[1] ^ v[9];
  h_[2] ^= v[2] ^ v[10];
  h_[3] ^= v[3] ^ v[11];
  h_[4] ^= v[4] ^ v[12];
  h_[5] ^= v[5] ^ v[13];
  h_[6] ^= v[6] ^ v[14];
  h_[7] ^= v[7] ^ v[15];
}

Blake2b::Blake2b(uint8_t outLen, void *key, uint8_t keyLen) {
  out_len_ = outLen;
  key_ = key;
  key_len_ = keyLen;

  Reset();
}


Blake2b::~Blake2b() {
}

void Blake2b::Reset() {
  c_ = 0;

  t_[0] = 0;
  t_[1] = 0;

  h_[0] = BLAKE2B_IV_0 ^ 0x01010000 ^ (key_len_ << 8) ^ out_len_;
  h_[1] = BLAKE2B_IV_1;
  h_[2] = BLAKE2B_IV_2;
  h_[3] = BLAKE2B_IV_3;
  h_[4] = BLAKE2B_IV_4;
  h_[5] = BLAKE2B_IV_5;
  h_[6] = BLAKE2B_IV_6;
  h_[7] = BLAKE2B_IV_7;

  if (key_ != NULL_PTR) {
    MEMCPY((void*)b_, key_, key_len_);
    MEMSET((void*)((size_t)b_ + key_len_), 0x00, 128 - key_len_);
    AddToT(128);
    Compress();
  }
}

void Blake2b::Update(void *data, size_t date_len) {
  uint8_t
    c = c_,
      missing = 128 - c;

  size_t
    b = (size_t)b_;

  while (date_len > missing) {
    MEMCPY((void*)(b + c), data, missing);
    AddToT(128);

    Compress(false);

    data = reinterpret_cast<void*>((size_t)data + missing);
    date_len -= missing;

    c = 0;
    missing = 128;
  }

  MEMCPY((void*)(b + c), data, date_len);
  c += date_len;

  c_ = c;
}

void Blake2b::Final(void *out) {
  uint8_t
    c = c_;

  AddToT(c);

  MEMSET((void*)((size_t)b_ + c), 0, 128 - c);

  Compress(true);

#ifdef CATENA_BIG_ENDIAN_PLATFORM
  for (uint8_t i = 0; i < 8; ++i) {
    h_[i] = LE(h_[i]);
  }
#endif

  MEMCPY(out, h_, out_len_);
  MEMSET(b_, 0, 128);

  Reset();
}

void Blake2b::Hash(void *data, size_t date_len, void *out) {
  Reset();
  Update(data, date_len);
  Final(out);
}
