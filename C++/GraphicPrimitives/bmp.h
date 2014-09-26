#ifndef BMP_H
#define BMP_H

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

#pragma pack( push, 1 )
struct BMP
{
    BMP();
    struct {
        uint16_t ID;
        uint32_t fileSizeInBytes;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t pixelArrayOffsetInBytes;
    } FileHeader;

    struct {
        uint32_t headerSizeInBytes;
        uint32_t bitmapWidthInPixels;
        uint32_t bitmapHeightInPixels;
        uint16_t colorPlaneCount;
        uint16_t bitsPerPixel;
        uint32_t bitmapCompessionMethod;
        uint32_t bitmapSizeInBytes;
        int32_t horizontalResolutionInPixelsPerMeter;
        int32_t verticalResolutionInPixelsPerMeter;
        uint32_t paletteColorCount;
        uint32_t importantColorCount;
    } DIBHeader;
};

#endif // BMP_H
