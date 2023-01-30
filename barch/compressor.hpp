#pragma once

#include <vector>
namespace barch {
class Compressor
{
public:
  Compressor();

public:
  void addByte(unsigned char b);
  void addBit(bool b);
  void printBuffer();
  void finalize();
  std::vector<unsigned char> buffer;

private:
  unsigned char currentByte;
  int bitPos;
};

} // namespace barch
