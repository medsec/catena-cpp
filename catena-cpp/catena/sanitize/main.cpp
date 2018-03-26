// This runs the different functions of Catena for address and memory sanitizing
// purposes.

#ifndef CATENA_SANITIZE
#define CATENA_SANITIZE

#include <iostream>
#include "./catena.h"
#include "./config.h"
#include "helpers/misc.h"

/******************************************************************************
  RUN CATENA FUNCTIONS
 ******************************************************************************/

void run_hash() {
  uint8_t *out = reinterpret_cast<uint8_t*>(MALLOC(64));
  size_t out_len = 64;

  catena::Catena *catena = catena::Catena::Butterfly();
  uint8_t g = 16;

  const size_t pwd_len = 3;
  uint8_t pwd[3] = {0x01, 0x23, 0x45};

  const size_t salt_len = 3;
  uint8_t salt[3] = {0x67, 0x89, 0xab};

  const size_t a_data_len = 3;
  uint8_t a_data[3] = {0x00, 0x00, 0x00};

  const size_t gamma_len = salt_len;
  uint8_t *gamma = salt;

  catena->Hash(pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g, g,
      gamma, gamma_len,
      out, out_len);

  delete catena;
  FREE(out);
}

void run_hash_martymcfly() {
  uint8_t *out = reinterpret_cast<uint8_t*>(MALLOC(64));
  size_t out_len = 64;

  catena::Catena *catena = catena::Catena::MartyMcFly();
  uint8_t g = 16;

  const size_t pwd_len = 3;
  uint8_t pwd[3] = {0x01, 0x23, 0x45};

  const size_t salt_len = 3;
  uint8_t salt[3] = {0x67, 0x89, 0xab};

  const size_t a_data_len = 3;
  uint8_t a_data[3] = {0x00, 0x00, 0x00};

  const size_t gamma_len = salt_len;
  uint8_t *gamma = salt;

  catena->Hash(pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g, g,
      gamma, gamma_len,
      out, out_len);

  delete catena;
  FREE(out);
}


void run_keyed_hash() {
  uint8_t *out = reinterpret_cast<uint8_t*>(MALLOC(64));
  size_t out_len = 64;

  catena::Catena *catena = catena::Catena::Dragonfly();
  uint8_t g = 21;

  const size_t pwd_len = 3;
  uint8_t pwd[3] = {0x01, 0x23, 0x45};

  const size_t salt_len = 3;
  uint8_t salt[3] = {0x67, 0x89, 0xab};

  const size_t a_data_len = 3;
  uint8_t a_data[3] = {0x00, 0x00, 0x00};

  const size_t gamma_len = salt_len;
  uint8_t *gamma = salt;

  const size_t key_len = 3;
  uint8_t key[3] = {0x67, 0x89, 0xab};

  const size_t user_id_len = 3;
  uint8_t userId[3] = {0x55, 0x0f, 0xab};

  catena->KeyedHash(
      pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g, g,
      gamma, gamma_len,
      key, key_len,
      userId, user_id_len,
      out, out_len);

  delete catena;
  FREE(out);
}

void run_server_relief_client() {
  uint8_t *out = reinterpret_cast<uint8_t*>(MALLOC(65));
  size_t out_len = 64;

  catena::Catena *catena = catena::Catena::Butterfly();
  uint8_t g = 16;

  const size_t pwd_len = 3;
  uint8_t pwd[3] = {0x01, 0x23, 0x45};

  const size_t salt_len = 3;
  uint8_t salt[3] = {0x67, 0x89, 0xab};

  const size_t a_data_len = 3;
  uint8_t a_data[3] = {0x00, 0x00, 0x00};

  const size_t gamma_len = salt_len;
  uint8_t *gamma = salt;

  catena->ServerReliefClient(
      pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g, g,
      gamma, gamma_len,
      out, out_len);

  delete catena;
  FREE(out);
}


void run_keyed_server_relief_client() {
  uint8_t *out = reinterpret_cast<uint8_t*>(MALLOC(65));
  size_t out_len = 64;

  catena::Catena *catena = catena::Catena::Dragonfly();
  uint8_t g = 21;

  const size_t pwd_len = 3;
  uint8_t pwd[3] = {0x01, 0x23, 0x45};

  const size_t salt_len = 3;
  uint8_t salt[3] = {0x67, 0x89, 0xab};

  const size_t a_data_len = 3;
  uint8_t a_data[3] = {0x00, 0x00, 0x00};

  const size_t gamma_len = salt_len;
  uint8_t *gamma = salt;

  const size_t key_len = 3;
  uint8_t key[3] = {0x67, 0x89, 0xab};

  const size_t user_id_len = 3;
  uint8_t userId[3] = {0x55, 0x0f, 0xab};

  catena->ServerReliefClientKeyed(
      pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g, g,
      gamma, gamma_len,
      key, key_len,
      userId, user_id_len,
      out, out_len);

  delete catena;
  FREE(out);
}


void run_server_relief_server() {
  uint8_t *out = reinterpret_cast<uint8_t*>(MALLOC(64));
  size_t out_len = 64;

  catena::Catena *catena = catena::Catena::Butterfly();

  uint8_t hash[64] = {
    0xf6, 0xd3, 0x4c, 0x05, 0xc4, 0x3e, 0x0e, 0xeb,
    0x12, 0x07, 0xe2, 0x88, 0x4f, 0xe5, 0x0b, 0x5d,
    0xa5, 0x84, 0xc1, 0xe3, 0x04, 0xa8, 0x8a, 0xae,
    0x35, 0x79, 0x89, 0xcf, 0xc8, 0xc2, 0x38, 0xaf,
    0xdf, 0x18, 0xbe, 0xb0, 0xda, 0x2b, 0xb2, 0xc6,
    0xb7, 0x8a, 0x55, 0xa2, 0x65, 0xb2, 0xa9, 0x8b,
    0x37, 0x9e, 0x19, 0xba, 0xa4, 0x74, 0xec, 0xba,
    0x29, 0x82, 0xa6, 0xc9, 0x24, 0x0b, 0xca, 0x49};

  catena->ServerReliefFinal(hash, 10, out, out_len);

  delete catena;
  FREE(out);
}


void run_ci_update() {
  catena::Catena *catena = catena::Catena::ButterflyFull();

  size_t out_len = 64;
  uint8_t g = 14;
  uint8_t g_new = 16;
  const size_t gamma_len = 3;
  uint8_t gamma[3] = {0x67, 0x89, 0xab};

  uint8_t hash[64] = {
    0xf6, 0xd3, 0x4c, 0x05, 0xc4, 0x3e, 0x0e, 0xeb,
    0x12, 0x07, 0xe2, 0x88, 0x4f, 0xe5, 0x0b, 0x5d,
    0xa5, 0x84, 0xc1, 0xe3, 0x04, 0xa8, 0x8a, 0xae,
    0x35, 0x79, 0x89, 0xcf, 0xc8, 0xc2, 0x38, 0xaf,
    0xdf, 0x18, 0xbe, 0xb0, 0xda, 0x2b, 0xb2, 0xc6,
    0xb7, 0x8a, 0x55, 0xa2, 0x65, 0xb2, 0xa9, 0x8b,
    0x37, 0x9e, 0x19, 0xba, 0xa4, 0x74, 0xec, 0xba,
    0x29, 0x82, 0xa6, 0xc9, 0x24, 0x0b, 0xca, 0x49};

  catena->Update(
      hash, out_len,
      g, g_new,
      gamma, gamma_len);

  delete catena;
}


void run_pow_server_pwd() {
  uint8_t *out_pwd = reinterpret_cast<uint8_t*>(MALLOC(64));
  uint8_t *out_salt = reinterpret_cast<uint8_t*>(MALLOC(64));
  uint8_t *out_proof = reinterpret_cast<uint8_t*>(MALLOC(64));
  const size_t out_proof_len = 64;

  catena::Catena *catena = catena::Catena::Dragonfly();
  uint8_t g_low = 4;
  uint8_t g_high = 5;

  const size_t pwd_len = 3;
  uint8_t pwd[3] = {0x01, 0x23, 0x45};

  const size_t salt_len = 3;
  uint8_t salt[3] = {0x67, 0x89, 0xab};

  const size_t a_data_len = 3;
  uint8_t a_data[3] = {0x00, 0x00, 0x00};

  const size_t gamma_len = salt_len;
  uint8_t *gamma = salt;

  const size_t out_pwd_len = 3;
  const size_t out_salt_len = 3;

  uint16_t p = 3;

  catena->ProofOfWorkServer(
      pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g_low, g_high,
      gamma, gamma_len,
      p, 1,
      out_pwd, out_pwd_len,
      out_salt, out_salt_len,
      out_proof, out_proof_len);

  delete catena;
  FREE(out_pwd);
  FREE(out_salt);
  FREE(out_proof);
}


void run_pow_server_pepper() {
  uint8_t *out_pwd = reinterpret_cast<uint8_t*>(MALLOC(64));
  uint8_t *out_salt = reinterpret_cast<uint8_t*>(MALLOC(64));
  uint8_t *out_proof = reinterpret_cast<uint8_t*>(MALLOC(64));
  const size_t out_proof_len = 64;

  catena::Catena *catena = catena::Catena::Dragonfly();
  uint8_t g_low = 4;
  uint8_t g_high = 5;

  const size_t pwd_len = 3;
  uint8_t pwd[3] = {0x01, 0x23, 0x45};

  const size_t salt_len = 3;
  uint8_t salt[3] = {0x67, 0x89, 0xab};

  const size_t a_data_len = 3;
  uint8_t a_data[3] = {0x00, 0x00, 0x00};

  const size_t gamma_len = salt_len;
  uint8_t *gamma = salt;

  const size_t out_pwd_len = 3;
  const size_t out_salt_len = 3;

  uint16_t p = 3;

  catena->ProofOfWorkServer(
      pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g_low, g_high,
      gamma, gamma_len,
      p, 0,
      out_pwd, out_pwd_len,
      out_salt, out_salt_len,
      out_proof, out_proof_len);

  delete catena;
  FREE(out_pwd);
  FREE(out_salt);
  FREE(out_proof);
}


void run_pow_client_pwd() {
  uint8_t *out_pwd = reinterpret_cast<uint8_t*>(MALLOC(64));
  const size_t out_pwd_len = 64;

  catena::Catena *catena = catena::Catena::Butterfly();
  uint8_t g_low = 9;
  uint8_t g_high = 9;

  const size_t pwd_len = 0;
  uint8_t pwd[0] = {};

  const size_t salt_len = 1;
  uint8_t salt[1] = {0x04};

  const size_t a_data_len = 2;
  uint8_t a_data[2] = {0x00, 0x00};

  const size_t gamma_len = 2;
  uint8_t gamma[2] = {0x00, 0x00};

  const size_t proof_hash_len = 64;
  uint8_t proof_hash[64] = {
    0xf6, 0xd3, 0x4c, 0x05, 0xc4, 0x3e, 0x0e, 0xeb,
    0x12, 0x07, 0xe2, 0x88, 0x4f, 0xe5, 0x0b, 0x5d,
    0xa5, 0x84, 0xc1, 0xe3, 0x04, 0xa8, 0x8a, 0xae,
    0x35, 0x79, 0x89, 0xcf, 0xc8, 0xc2, 0x38, 0xaf,
    0xdf, 0x18, 0xbe, 0xb0, 0xda, 0x2b, 0xb2, 0xc6,
    0xb7, 0x8a, 0x55, 0xa2, 0x65, 0xb2, 0xa9, 0x8b,
    0x37, 0x9e, 0x19, 0xba, 0xa4, 0x74, 0xec, 0xba,
    0x29, 0x82, 0xa6, 0xc9, 0x24, 0x0b, 0xca, 0x49};

  uint16_t p = 3;

  catena->ProofOfWorkPassword(
      pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g_low, g_high,
      gamma, gamma_len,
      p, 1,
      proof_hash, proof_hash_len,
      out_pwd, out_pwd_len);

  delete catena;
  FREE(out_pwd);
}

void run_pow_client_pepper() {
  uint8_t *out_pwd = reinterpret_cast<uint8_t*>(MALLOC(64));
  const size_t out_pwd_len = 64;

  catena::Catena *catena = catena::Catena::Butterfly();
  uint8_t g_low = 9;
  uint8_t g_high = 9;

  const size_t pwd_len = 5;
  uint8_t pwd[5] = {0x0a, 0x34, 0xbc, 0xde, 0x21};

  const size_t salt_len = 1;
  uint8_t salt[1] = {0x00};

  const size_t a_data_len = 2;
  uint8_t a_data[2] = {0x00, 0x00};

  const size_t gamma_len = 2;
  uint8_t gamma[2] = {0x00, 0x00};

  const size_t proof_hash_len = 64;
  uint8_t proof_hash[64] = {
    0x12, 0x85, 0x5f, 0x67, 0x0c, 0x9b, 0x46, 0x39,
    0xaa, 0xd3, 0x10, 0xe5, 0xfc, 0x86, 0x07, 0x31,
    0xa5, 0x0f, 0x35, 0xb1, 0x28, 0x06, 0xa2, 0xac,
    0x3b, 0xac, 0x54, 0x52, 0xd0, 0x79, 0xa1, 0xbe,
    0xa5, 0x39, 0x17, 0xee, 0xb6, 0x53, 0xaa, 0x9f,
    0x8d, 0xb4, 0x5b, 0x50, 0x7e, 0x42, 0xea, 0x4f,
    0x56, 0x33, 0xc1, 0x76, 0x4b, 0x13, 0xe5, 0xcc,
    0xf2, 0x33, 0x0e, 0xae, 0x11, 0xd3, 0xb9, 0x0f};
  uint16_t p = 4;

  catena->ProofOfWorkPepper(
      pwd, pwd_len,
      salt, salt_len,
      a_data, a_data_len,
      g_low, g_high,
      gamma, gamma_len,
      p, 0,
      proof_hash, proof_hash_len,
      out_pwd, out_pwd_len);

  delete catena;
  FREE(out_pwd);
}

/******************************************************************************
  MAIN
 ******************************************************************************/

int main(int argc, char **argv) {
  std::cout << "running hash...                                  \t"
    << std::flush;
  run_hash();
  std::cout << "done\n";

  std::cout << "running hash...                                  \t"
    << std::flush;
  run_hash_martymcfly();
  std::cout << "done\n";

  std::cout << "running keyed hash...                            \t"
    << std::flush;
  run_keyed_hash();
  std::cout << "done\n";

  std::cout << "running server relief client...                  \t"
    << std::flush;
  run_server_relief_client();
  std::cout << "done\n";

  std::cout << "running keyed server relief (client)...          \t"
    << std::flush;
  run_keyed_server_relief_client();
  std::cout << "done\n";

  std::cout << "running server relief (server)...                \t"
    << std::flush;
  run_server_relief_server();
  std::cout << "done\n";

  std::cout << "running client-independent update...             \t"
    << std::flush;
  run_ci_update();
  std::cout << "done\n";

  std::cout << "running server-side proof of work for password...\t"
    << std::flush;
  run_pow_server_pwd();
  std::cout << "done\n";

  std::cout << "running server-side proof of work for salt...    \t"
    << std::flush;
  run_pow_server_pepper();
  std::cout << "done\n";

  std::cout << "running client-side proof of work for password...\t"
    << std::flush;
  run_pow_client_pwd();
  std::cout << "done\n";

  std::cout << "running client-side proof of work for salt...    \t"
    << std::flush;
  run_pow_client_pepper();
  std::cout << "done\n";

  return 0;
}

#endif

