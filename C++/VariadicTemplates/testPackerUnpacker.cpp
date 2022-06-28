/*
c++ -std=c++14 -o testPackerUnpacker testPackerUnpacker.cpp Commands.cpp
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


  auto arr = bitWise::convertTuple2Array<uint8_t>(input);
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


  std::cout << "\nUnpacking vector:" << std::endl;
  const int nFields =  5;
  const int nBits   =  5;
  const int size    = 16;
  std::vector<uint16_t> vec(nFields,0);
  vec[0] = 0xa0;
  vec[1] = 0xb1;
  vec[2] = 0xc2;
  vec[3] = 0xd3;
  vec[4] = 0xe4;
  vec[5] = 0xf5;
  std::bitset<size*nFields>bits(0);
  for (auto i = 0u; i < nFields; i ++)
    bits |= (static_cast<std::bitset<size*nFields>>(vec[i]) << (size*i));
  auto unpacked = bitWise::unpackSequence<size,nBits>(bits);
  auto arrUnpacked_ = bitWise::convertTuple2Array<std::bitset<size*nFields>>(unpacked);
  uint8_t arrUnpacked[size];
  for (auto i = 0u; i < size; i++)
    {
      arrUnpacked[i] = arrUnpacked_[i].to_ulong();
      std::cout << "arrUnpacked[" << i << "] = 0x" << std::hex << +arrUnpacked[i] << std::dec << std::endl;
    }


  // #################
  // # Pack commands #
  // #################
  std::cout << "\nTesting commands:" << std::endl;
  std::vector<uint16_t> words;
  Commands::serialize(Commands::WriteCmd(0, 2, 3, 31), words);
  Commands::serialize(Commands::SyncCmd(), words);
  for (auto& w : words)
    std::cout << "Word n = " << std::hex << w << std::dec << std::endl;


  return 0;
}
