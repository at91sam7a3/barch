#pragma once
#include <filesystem>
#include <functional>

namespace barch {

struct Bitmap
{
  int width;
  int height;
  unsigned char* data;
};

std::size_t convertGs2Barch(Bitmap& bitmap, std::function<void(int)> progress);
void convertBarch2Gs(Bitmap& bitmap, size_t compressedDataSize, std::function<void(int)> progress);
} // namespace barch
