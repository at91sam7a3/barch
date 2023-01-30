#include "barch.hpp"
#include "compressor.hpp"
#include "decompressor.hpp"
#include <chrono>
#include <thread>
#include <string.h>
#include <fstream>
#include <iostream>

namespace {
typedef unsigned char byte;
const byte BLACK = 0x00;
const byte WHITE = 0xFF;

inline void push4Bytes(std::vector<byte>& vec,  byte b)
{
    for(int i = 0; i<4;++i)
    {
        vec.push_back(b);
    }
}
}

std::size_t barch::convertGs2Barch(Bitmap &bitmap, std::function<void(int)> progress)
{
    size_t dataSize = bitmap.width * bitmap.height;
    dataSize = dataSize - dataSize %4; //our data has to be even to 4, just skip 1-3 pixels

    Compressor compressor;
    progress(0);

    for(int i = 0; i< dataSize;i+=4)
    {
        if((bitmap.data[i]==BLACK) && (bitmap.data[i+1]==BLACK) && (bitmap.data[i+2]==BLACK) && (bitmap.data[i+3]==BLACK))
        {
            compressor.addBit(1);
            compressor.addBit(0);
            continue;
        }

        if((bitmap.data[i]==WHITE) && (bitmap.data[i+1]==WHITE) && (bitmap.data[i+2]==WHITE) && (bitmap.data[i+3]==WHITE))
        {
            compressor.addBit(0);
            continue;
        }
        compressor.addBit(1);
        compressor.addBit(1);
        for(byte j=0;j<4;++j)
            compressor.addByte(bitmap.data[i+j]);
        //using namespace std::chrono_literals;
        //std::this_thread::sleep_for(1ms);
        progress(100*i/dataSize);

    }
    compressor.finalize();

    free(bitmap.data);
    bitmap.data = new unsigned char[compressor.buffer.size()];
    std::copy( compressor.buffer.data(),compressor.buffer.data()+compressor.buffer.size(),bitmap.data);
    progress(100);
    return compressor.buffer.size();
}

void barch::convertBarch2Gs( Bitmap &bitmap, size_t compressedDataSize,std::function<void(int)> progress)
{
    progress(0);
    size_t dataSize = bitmap.width * bitmap.height;
    std::vector<byte> bmpBuffer;
    Decompressor decompressor;
    decompressor.buffer = std::vector<byte>(bitmap.data,bitmap.data+compressedDataSize);
    std::ofstream raw;
    raw.open("/home/oleksii/decoding_raw_in_test.hex", std::ios::binary | std::ios::out);
    raw.write((const char*)decompressor.buffer.data(),decompressor.buffer.size());
    raw.close();
    while (!decompressor.isEmpty()) {
        bool first = decompressor.popBit();
        if(first==0)
        {
            push4Bytes(bmpBuffer,WHITE);
            continue;
        }
        bool second = decompressor.popBit();
        if(second == 0)
        {
            push4Bytes(bmpBuffer,BLACK);
            continue;
        }
        for(byte i=0;i<4;++i)
        {
            bmpBuffer.push_back(decompressor.popByte());
        }
        progress(100*bmpBuffer.size()/bmpBuffer.capacity());
    }
    std::ofstream raw2;
    raw2.open("/home/oleksii/raw_ui-decoded777.hex", std::ios::binary | std::ios::out);
    raw2.write((const char*)bmpBuffer.data(),bmpBuffer.size());
    raw2.close();
    free(bitmap.data);
    bitmap.data = (byte*)malloc(bmpBuffer.size());
    memcpy(bitmap.data, bmpBuffer.data(),bmpBuffer.size());

    progress(100);
}
