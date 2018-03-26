#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#ifndef CATENA_INCLUDE_HELPERS_MISC_H_
#define CATENA_INCLUDE_HELPERS_MISC_H_

/*
  converts a single byte into a hex value
*/
char ByteToHex(uint8_t num);

/*
  converts a list of bytes into hexvalues
  bin : list of bytes
  len : number of bytes
*/
std::string Hex(uint8_t *bin, size_t len);

void PrintHex(void *ptr, size_t len);

#endif  // CATENA_INCLUDE_HELPERS_MISC_H_

