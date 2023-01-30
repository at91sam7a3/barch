#include "decompressor.hpp"

namespace barch {

Decompressor::Decompressor()
  : currentIndex(0)
  , bitPos(7)
{}

bool Decompressor::popBit()
{
  if (isEmpty())
  {
    throw("Trying to read empty buffer");
  }
  bool b;
  b = (buffer[currentIndex] >> bitPos) & 1;
  if (bitPos == 0)
  {
    currentIndex++;
    bitPos = 7;
  }
  else
  {
    bitPos--;
  }
  return b;
}

unsigned char Decompressor::popByte()
{
  unsigned char res = 0x00;
  for (int i = 7; i >= 0; --i)
  {
    bool b = popBit();
    res = res | b << i;
  }
  return res;
}

bool Decompressor::isEmpty()
{
  if ((currentIndex == buffer.size() - 1) && (bitPos == 0))
    return true;
  else
    return false;
}

} // namespace barch
