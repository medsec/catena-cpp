#include <time.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "./catena.h"
#include "./config.h"
#include "helpers/misc.h"

bool isLittleEndian() {
  uint16_t *test = reinterpret_cast<uint16_t*>(MALLOC(2));

  *test = 1;

  return reinterpret_cast<uint8_t*>(test)[0] == 1;
}

void createBenchmark(catena::VERSION version) {
  uint8_t *out = reinterpret_cast<uint8_t*>(MALLOC(64));

  const uint64_t TIMES = 10;
  clock_t start;

  clock_t times[TIMES];
  std::ofstream performance;
  catena::Catena *catena;
  uint8_t g;

  switch (version) {
    case catena::VERSION::CATENA_DRAGONFLY:
      catena = catena::Catena::Dragonfly();
      g = 21;
      performance.open("bench_dragonfly.txt");
      performance << "Dragonfly" << std::endl;
      break;
    case catena::VERSION::CATENA_DRAGONFLY_FULL:
      catena = catena::Catena::DragonflyFull();
      g = 22;
      performance.open("bench_dragonfly-full.txt");
      performance << "Dragonfly-Full" << std::endl;
      break;
    case catena::VERSION::CATENA_BUTTERFLY:
      catena = catena::Catena::Butterfly();
      g = 16;
      performance.open("bench_butterfly.txt");
      performance << "Butterfly" << std::endl;
      break;
    case catena::VERSION::CATENA_BUTTERFLY_FULL:
      catena = catena::Catena::ButterflyFull();
      g = 17;
      performance.open("bench_butterfly-full.txt");
      performance << "Butterfly-Full" << std::endl;
      break;
    case catena::VERSION::CATENA_MARTYMCFLY:
      catena = catena::Catena::MartyMcFly();
      g = 17;
      performance.open("bench_martymcfly.txt");
      performance << "MartyMcFly" << std::endl;
      break;
    case catena::VERSION::CATENA_MARTYMCFLY_FULL:
      catena = catena::Catena::MartyMcFlyFull();
      g = 23;
      performance.open("bench_martymcfly-full.txt");
      performance << "MartyMcFly-Full" << std::endl;
      break;
    default:
      throw std::runtime_error("no valid instance of Catena specified");
  }


  const size_t pwdLen = 3;
  uint8_t pwd[pwdLen] = {0x21, 0x23, 0x45};
  performance << "pwd: " << Hex(pwd, pwdLen) << std::endl;

  const size_t saltLen = 3;
  uint8_t salt[saltLen] = {0x67, 0x89, 0xab};
  performance << "salt: " << Hex(salt, saltLen) << std::endl;

  const size_t aDataLen = 3;
  uint8_t aData[aDataLen] = {0x00, 0x00, 0x00};
  performance << "aData: " << Hex(aData, aDataLen) << std::endl;

  const size_t gammaLen = saltLen;
  uint8_t *gamma = salt;
  performance << "gamma: " << Hex(gamma, gammaLen) << std::endl;

  performance << "g: " << static_cast<int>(g) << std::endl;

  uint8_t tmp[3] =  {0x21, 0x23, 0x45};
  for (uint8_t i = 0; i < TIMES; ++i) {
    MEMCPY(pwd, tmp, 3);
    // std::cout << static_cast<int>(i) << "  ";
    start = clock();
    // std::cout<< "Pwd: " << Hex(pwd, pwdLen) << std::endl;
    // std::cout<< "salt: " << Hex(salt, saltLen) << std::endl;
    // std::cout<< "pwd: " << Hex(aData, aDataLen) << std::endl;
    // std::cout<< "gamma: " << Hex(gamma, gammaLen) << std::endl;
    catena->Hash(pwd, pwdLen,
                 salt, saltLen,
                 aData, aDataLen,
                 g, g,
                 gamma, gammaLen,
                 out, 64);
    times[i] = clock() - start;
    // std::cout << "hash: " <<  Hex(out, 64) << "\n" << std::endl;
    performance << (times[i] * 1000 / CLOCKS_PER_SEC) << std::endl;
  }

  uint64_t sum = 0;

  for (uint8_t i = 0; i < TIMES; ++i) {
    sum += times[i];
  }

  performance << "avrg: " << (sum / TIMES * 1000 / CLOCKS_PER_SEC) << std::endl;

  performance.close();

  delete catena;
  FREE(out);
}

int main() {
#ifdef SIMULATE_WASM
  memory::AddressStore::init();
#endif

  createBenchmark(catena::VERSION::CATENA_MARTYMCFLY);
  createBenchmark(catena::VERSION::CATENA_MARTYMCFLY_FULL);
  createBenchmark(catena::VERSION::CATENA_DRAGONFLY);
  createBenchmark(catena::VERSION::CATENA_DRAGONFLY_FULL);
  createBenchmark(catena::VERSION::CATENA_BUTTERFLY);
  createBenchmark(catena::VERSION::CATENA_BUTTERFLY_FULL);

  return 0;
}
