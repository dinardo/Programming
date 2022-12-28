/*
c++ -std=c++14 -o testPackerUnpacker testPackerUnpacker.cpp
*/

#include <iostream>
#include <vector>

#include "PackerUnpacker.h"
#include "Commands.h"


int main()
{
  uint32_t val = 0b101010111100; // 0xabc
  uint32_t a, b, c;

  auto input = bitWise::unpack<3,5,4>(val);
  std::tie(a,b,c) = input;


  std::cout << "Unpacking: 0x" << std::hex << val << std::endl;
  std::cout << "a = 0x" << a << std::endl;
  std::cout << "b = 0x" << b << std::endl;
  std::cout << "c = 0x" << c << std::dec << std::endl;


  std::tie(a,b,c) = bitWise::revertTuple(input);
  std::cout << "\nReverted: " << std::hex << std::endl;
  std::cout << "a = 0x" << a << std::endl;
  std::cout << "b = 0x" << b << std::endl;
  std::cout << "c = 0x" << c << std::dec << std::endl;


  uint64_t output = bitWise::pack<3,3,3>(0b11,0b10,0b01);
  std::cout << "\nPacking" << std::endl;
  std::cout << "output = 0x" << std::hex << output << std::dec << std::endl;


  auto arr = bitWise::unpackTuple2Array<uint8_t>(input);
  std::cout << "\nUnpacking in array: 0x" << std::hex << val << std::endl;
  std::cout << "a = 0x" << +arr[0] << std::endl;
  std::cout << "b = 0x" << +arr[1] << std::endl;
  std::cout << "c = 0x" << +arr[2] << std::dec << std::endl;


  input = bitWise::unpackSequence<3,4>(val);
  std::tie(a,b,c) = input;


  std::cout << "\nUnpacking sequence: 0x" << std::hex << val << std::endl;
  std::cout << "a = 0x" << a << std::endl;
  std::cout << "b = 0x" << b << std::endl;
  std::cout << "c = 0x" << c << std::dec << std::endl;


  std::cout << "\nUnpacking iterable into another iterable:" << std::endl;
  const int nValsIn  =  6;
  const int sizeIn   = 16;
  std::array<uint16_t, nValsIn> inArray;

  const int sizeOut  =  5;
  const int nValsOut = sizeIn * nValsIn / sizeOut + ((sizeIn * nValsIn) % sizeOut != 0 ? 1 : 0);
  std::array<uint8_t, nValsOut> outArray;

  inArray[0] = 0xa06c;
  inArray[1] = 0xb17d;
  inArray[2] = 0xc28e;
  inArray[3] = 0xd39f;
  inArray[4] = 0xe4a0;
  inArray[5] = 0xf5b1;

  bitWise::unpackIterable2Iterable<sizeOut>(inArray.begin(), inArray.end(), outArray.begin());
  for (auto i = 0u; i < nValsOut; i++)
    std::cout << "output iterable[" << i << "] = 0x" << std::hex << +outArray[i] << std::dec << std::endl;


  // #################
  // # Pack commands #
  // #################
  std::cout << "\nTesting commands:" << std::endl;
  std::vector<uint16_t> words;
  Commands::serialize(Commands::WriteCmd{0, 2, 3, 31}, words);
  Commands::serialize(Commands::SyncCmd(), words);
  for (auto& w : words)
    std::cout << "Word n = " << std::hex << w << std::dec << std::endl;


  return 0;
}
