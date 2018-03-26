#include "helpers/misc.h"

char ByteToHex(uint8_t num) {
  switch (num & 0xf) {
    case 0:
      return '0';
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    case 10:
      return 'a';
    case 11:
      return 'b';
    case 12:
      return 'c';
    case 13:
      return 'd';
    case 14:
      return 'e';
    default:
      return 'f';
  }
}

std::string Hex(uint8_t *bin, size_t len) {
  std::string str;

  for (size_t i = 0; i < len; ++i) {
    str += ByteToHex(bin[i] >> 4);
    str += ByteToHex(bin[i]);
  }

  return str;
}

void PrintHex(void *ptr, size_t len) {
  uint8_t *ptr_ = reinterpret_cast<uint8_t*>(ptr);

  std::ios prev_state(nullptr);
  prev_state.copyfmt(std::cout);

  for (size_t i = 0; i < len; ++i) {
    std::cout << std::hex << std::setw(sizeof(uint8_t) << 1) <<
      std::setfill('0') << static_cast<int>(ptr_[i]) << ' ';

    if (i % 8 == 7) {
      std::cout << std::endl;
    }
  }

  std::cout << std::endl;

  std::cout.copyfmt(prev_state);
}
