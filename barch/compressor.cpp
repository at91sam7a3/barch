#include "compressor.hpp"

#include <bitset>
#include <iostream>

namespace barch {

Compressor::Compressor()
  : currentByte(0x00)
  , bitPos(7)
{}

void Compressor::addByte(unsigned char b)
{
  for (int i = 7; i >= 0; i--)
  {
    addBit(static_cast<bool>((b >> i) & 1));
  }
}

void Compressor::addBit(bool b)
{
  //std::cout<<"add bit "<<b<<std::endl;
  currentByte = currentByte | (b << bitPos);
  if (bitPos == 0)
  {
    buffer.push_back(currentByte);
    std::bitset<8> a(currentByte);
    // std::cout<<"Adding byte: 0x"<<a<<std::endl;
    bitPos = 7;
    currentByte = 0;
  }
  else
  {
    bitPos--;
  }
}

void Compressor::printBuffer()
{
  /*std::bitset<8> a(currentByte);
    std::cout<<"current byte: "<<a<<std::endl;
    for(unsigned char c : buffer)
    {
        std::bitset<8> x(c);
        std::cout<<x<<",";
    }
    std::cout<<std::endl;*/
  std::cout << "total data = " << buffer.size() << std::endl;
}

void Compressor::finalize()
{
  buffer.push_back(currentByte);
}

} // namespace barch
