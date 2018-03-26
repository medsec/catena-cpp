#include "hash/blamka.h"

BlaMka::BlaMka() {
  state_ = reinterpret_cast<uint8_t*>(MALLOC(2048));
  idx_ = 0;
  out_ = reinterpret_cast<uint8_t*>(MALLOC(1024));
}

BlaMka::~BlaMka() {
  FREE(state_);
  FREE(out_);
}

inline uint64_t Ba_to_int(uint8_t* a) {
  uint64_t res = 0;
  res |= (uint64_t) (a[7] & 0xFF) << 56;
  res |= (uint64_t) (a[6] & 0xFF) << 48;
  res |= (uint64_t) (a[5] & 0xFF) << 40;
  res |= (uint64_t) (a[4] & 0xFF) << 32;
  res |= (uint64_t) (a[3] & 0xFF) << 24;
  res |= (uint64_t) (a[2] & 0xFF) << 16;
  res |= (uint64_t) (a[1] & 0xFF) << 8;
  res |= a[0] & 0xFF;
  return res;
}

inline void Int_to_Ba(uint64_t a, uint8_t *b) {
  b[7] = (a >> 56) & 0xFF;
  b[6] = (a >> 48) & 0xFF;
  b[5] = (a >> 40) & 0xFF;
  b[4] = (a >> 32) & 0xFF;
  b[3] = (a >> 24) & 0xFF;
  b[2] = (a >> 16) & 0xFF;
  b[1] = (a >> 8) & 0xFF;
  b[0] = a & 0xFF;
}

inline uint64_t Rotr64(const uint64_t val, const uint8_t bits) {
  return (val >> bits) | (val << (64 - bits));
}

void BlaMka::Gl(uint8_t *a, uint8_t *b, uint8_t *c, uint8_t *d) {
  uint64_t a_int = Ba_to_int(a);
  uint64_t b_int = Ba_to_int(b);
  uint64_t c_int = Ba_to_int(c);
  uint64_t d_int = Ba_to_int(d);

  a_int = a_int + b_int;
  d_int = Rotr64((d_int ^ a_int), 32);
  c_int = c_int + d_int;
  b_int = Rotr64((b_int ^ c_int), 24);
  a_int = a_int + b_int;
  d_int = Rotr64((d_int ^ a_int), 16);
  c_int = c_int + d_int;
  b_int = Rotr64((b_int ^ c_int), 63);

  Int_to_Ba(a_int, a);
  Int_to_Ba(b_int, b);
  Int_to_Ba(c_int, c);
  Int_to_Ba(d_int, d);
}

inline void reverse(uint8_t* arr) {
  uint8_t tmp = arr[0];
  arr[0] = arr[7];
  arr[7] = tmp;

  tmp = arr[1];
  arr[1] = arr[6];
  arr[6] = tmp;

  tmp = arr[2];
  arr[2] = arr[5];
  arr[5] = tmp;

  tmp = arr[3];
  arr[3] = arr[4];
  arr[4] = tmp;
}

void BlaMka::P(uint8_t *p1, uint8_t *p2, uint8_t *p3, uint8_t *p4,
               uint8_t *p5, uint8_t *p6, uint8_t *p7, uint8_t *p8) {
  uint8_t *v0 = p1;
  uint8_t *v1 = reinterpret_cast<uint8_t*>((size_t)p1 + 8);
  uint8_t *v2 = p2;
  uint8_t *v3 = reinterpret_cast<uint8_t*>((size_t)p2 + 8);
  uint8_t *v4 = p3;
  uint8_t *v5 = reinterpret_cast<uint8_t*>((size_t)p3 + 8);
  uint8_t *v6 = p4;
  uint8_t *v7 = reinterpret_cast<uint8_t*>((size_t)p4 + 8);
  uint8_t *v8 = p5;
  uint8_t *v9 = reinterpret_cast<uint8_t*>((size_t)p5 + 8);
  uint8_t *v10 = p6;
  uint8_t *v11 = reinterpret_cast<uint8_t*>((size_t)p6 + 8);
  uint8_t *v12 = p7;
  uint8_t *v13 = reinterpret_cast<uint8_t*>((size_t)p7 + 8);
  uint8_t *v14 = p8;
  uint8_t *v15 = reinterpret_cast<uint8_t*>((size_t)p8 + 8);
  Gl(v0, v4, v8, v12);
  Gl(v1, v5, v9, v13);
  Gl(v2, v6, v10, v14);
  Gl(v3, v7, v11, v15);
  Gl(v0, v5, v10, v15);
  Gl(v1, v6, v11, v12);
  Gl(v2, v7, v8, v13);
  Gl(v3, v4, v9, v14);

  reverse(v0);
  reverse(v1);
  reverse(v2);
  reverse(v3);
  reverse(v4);
  reverse(v5);
  reverse(v6);
  reverse(v7);
  reverse(v8);
  reverse(v9);
  reverse(v10);
  reverse(v11);
  reverse(v12);
  reverse(v13);
  reverse(v14);
  reverse(v15);
}

void BlaMka::Compress() {
    uint8_t *x = reinterpret_cast<uint8_t*>((size_t)state_);
    uint8_t *y = reinterpret_cast<uint8_t*>((size_t)state_ + 1024);

    uint8_t *r = new uint8_t[1024];
    uintx::XorArray(x, y, 1024, r);

    // r is 1024 bytes long and is from now on viewed as an 8X8 matrix with
    // 16 byte registers
    uint8_t *q = new uint8_t[1024];

    MEMCPY(q, r, 1024);
    P(q, q + 16, q + 32, q + 48, q + 64, q + 80, q + 96, q + 112);
    P(q + 128, q + 144, q + 160, q + 176, q + 192, q + 208, q + 224, q + 240);
    P(q + 256, q + 272, q + 288, q + 304, q + 320, q + 336, q + 352, q + 368);
    P(q + 384, q + 400, q + 416, q + 432, q + 448, q + 464, q + 480, q + 496);
    P(q + 512, q + 528, q + 544, q + 560, q + 576, q + 592, q + 608, q + 624);
    P(q + 640, q + 656, q + 672, q + 688, q + 704, q + 720, q + 736, q + 752);
    P(q + 768, q + 784, q + 800, q + 816, q + 832, q + 848, q + 864, q + 880);
    P(q + 896, q + 912, q + 928, q + 944, q + 960, q + 976, q + 992, q + 1008);

    for (size_t idx = 0; idx < 128; ++idx) {
      reverse(reinterpret_cast<uint8_t*>(PTR_TO(q, idx, 8)));
    }

    uint8_t *z = new uint8_t[1024];
    MEMCPY(z, q, 1024);
    P(z, z + 128, z + 256, z + 384, z + 512, z + 640, z + 768, z + 896);
    P(z + 16, z + 144, z + 272, z + 400, z + 528, z + 656, z + 784, z + 912);
    P(z + 32, z + 160, z + 288, z + 416, z + 544, z + 672, z + 800, z + 928);
    P(z + 48, z + 176, z + 304, z + 432, z + 560, z + 688, z + 816, z + 944);
    P(z + 64, z + 192, z + 320, z + 448, z + 576, z + 704, z + 832, z + 960);
    P(z + 80, z + 208, z + 336, z + 464, z + 592, z + 720, z + 848, z + 976);
    P(z + 96, z + 224, z + 352, z + 480, z + 608, z + 736, z + 864, z + 992);
    P(z + 112, z + 240, z + 368, z + 496, z + 624, z + 752, z + 880, z + 1008);

    for (size_t idx = 0; idx < 128; ++idx) {
      reverse(reinterpret_cast<uint8_t*>(PTR_TO(z, idx, 8)));
    }


    uintx::XorArray(z, r, 1024, reinterpret_cast<uint8_t*>(out_));
    delete[] r;
    delete[] q;
    delete[] z;
}

void BlaMka::Update(void *data, size_t data_len) {
  if (idx_ + data_len <= 2048) {
    MEMCPY(reinterpret_cast<uint8_t*>((size_t)state_ + idx_), data, data_len);
    idx_ += data_len;
  } else {
    throw std::runtime_error("To much data given to BlaMka!");
  }
}

void BlaMka::Final(void *out) {
  if (idx_ == 2048) {
    Compress();
    MEMCPY(out, out_, 1024);
    idx_ = 0;
  } else {
    throw std::runtime_error("BlaMka state was not filled before hashing!");
  }
}

void BlaMka::Hash(void *data, size_t data_len, void *out) {
  Update(data, data_len);
  Final(out);
}

