#pragma once
#include <vector>

namespace barch {

class Decompressor
{
public:
  Decompressor();

  bool popBit();
  unsigned char popByte();
  bool isEmpty();
  std::vector<unsigned char> buffer;

private:
  std::size_t currentIndex;
  int bitPos;
};

} // namespace barch
