#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include "PackerUnpacker.h"


namespace Commands
{
  constexpr uint8_t map5to8bit[] =
    {
      0x6A, // 00: 0b01101010,
      0x6C, // 01: 0b01101100,
      0x71, // 02: 0b01110001,
      0x72, // 03: 0b01110010,
      0x74, // 04: 0b01110100,
      0x8B, // 05: 0b10001011,
      0x8D, // 06: 0b10001101,
      0x8E, // 07: 0b10001110,
      0x93, // 08: 0b10010011,
      0x95, // 09: 0b10010101,
      0x96, // 10: 0b10010110,
      0x99, // 11: 0b10011001,
      0x9A, // 12: 0b10011010,
      0x9C, // 13: 0b10011100,
      0xA3, // 14: 0b10100011,
      0xA5, // 15: 0b10100101,
      0xA6, // 16: 0b10100110,
      0xA9, // 17: 0b10101001,
      0xAA, // 18: 0b10101010,
      0xAC, // 19: 0b10101100,
      0xB1, // 20: 0b10110001,
      0xB2, // 21: 0b10110010,
      0xB4, // 22: 0b10110100,
      0xC3, // 23: 0b11000011,
      0xC5, // 24: 0b11000101,
      0xC6, // 25: 0b11000110,
      0xC9, // 26: 0b11001001,
      0xCA, // 27: 0b11001010,
      0xCC, // 28: 0b11001100,
      0xD1, // 29: 0b11010001,
      0xD2, // 30: 0b11010010,
      0xD4  // 31: 0b11010100
    };

  constexpr uint16_t Write = 0xABAB;
  constexpr uint16_t Sync  = 0xCDCD;

  template <uint16_t theCmd, size_t nFields>
  class ComposeCommand
  {
    static_assert(nFields % 2 == 0, "Commands::ComposeCommand: a command must have an even number of fields");

  public:
    static constexpr size_t nFields_ = nFields;
    static constexpr size_t theCmd_  = theCmd;

    auto serializeFields() const { return std::array<uint8_t, nFields_>(); }

  protected:
    template <int... Sizes, class... Args>
    uint8_t packAndEncode(Args&&... args) const { return Commands::map5to8bit[bitWise::pack<Sizes...>(std::forward<Args>(args)...)]; }
    // In general && allows to use a reference or a temporary
    // In this specific case of template it is good practice because we don't know what we are going to get from the parameter pack
    // std::forward either copy or move, depending on the whether we have variables or temporarires
    // Perfect forwarding: instead of these two classes, we can use std::forward

    // template <typename T, typename Arg>
    // T create(Arg& a) { return T(a); }

    // template <typename T, typename Arg>
    // T create(const Arg& a) { return T(a); }

    // template <typename T,typename Arg>
    // T create(Arg&& a) { return T(std::forward<Arg>(a)); }

    // int main()
    // {
    //   //L-value
    //   int five    = 5;
    //   int myFive1 = create<int>(five);
    //   std::cout << "myFive1: "  << myFive1 << std::endl;
    //   // R-value
    //   int myFive2 = create<int>(5);
    //   std::cout << "myFive2: " << myFive2 << std::endl;
    // }
  };

  struct WriteCmd : public ComposeCommand<Commands::Write, 4>
  {
    WriteCmd(uint8_t id, uint8_t data1, uint8_t data2, uint8_t address)
      : id(id)
      , data1(data1)
      , data2(data2)
      , address(address) {}

    uint8_t id;
    uint8_t data1;
    uint8_t data2;
    uint8_t address;

    auto serializeFields() const
    {
      std::array<uint8_t, nFields_> fields;

      fields[0] = packAndEncode<4, 1>(id, 0);
      fields[1] = packAndEncode<5>(data1);
      fields[2] = packAndEncode<5>(data2);
      fields[3] = packAndEncode<5>(address);

      return fields;
    }
  };

  struct SyncCmd : public ComposeCommand<Commands::Sync, 0> {};

  template <typename cmdType>
  void serialize(const cmdType& cmd, std::vector<uint16_t>& words)
  {
    words.push_back(cmdType::theCmd_);

    auto fields = cmd.serializeFields();

    for(auto i = 0u; i < cmdType::nFields_; i += 2)
      words.push_back(bitWise::pack<8, 8>(fields[i], fields[i+1]));
  }

  template <typename cmdType>
  auto getFrames(const cmdType& cmd)
  {
    std::vector<uint16_t> words;

    words.reserve(1 + cmdType::nFields_ / 2);
    Commands::serialize(cmd, words);

    return words;
  }
}

#endif
