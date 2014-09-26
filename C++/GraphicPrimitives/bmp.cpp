#include "bmp.h"

//#pragma pack( pop )
BMP::BMP(){
    //Initialized fields
    FileHeader.ID                                   = 0x4d42; // == 'BM' (little-endian)
    FileHeader.reserved1                            = 0;
    FileHeader.reserved2                            = 0;
    FileHeader.pixelArrayOffsetInBytes              = sizeof( FileHeader ) + sizeof( DIBHeader );
    DIBHeader.headerSizeInBytes                     = 40;
    DIBHeader.colorPlaneCount                       = 1;
    DIBHeader.bitsPerPixel                          = 24;
    DIBHeader.bitmapCompessionMethod                = 0;
    DIBHeader.horizontalResolutionInPixelsPerMeter  = 2835; // == 72 ppi
    DIBHeader.verticalResolutionInPixelsPerMeter    = 2835; // == 72 ppi
    DIBHeader.paletteColorCount                     = 0;
    DIBHeader.importantColorCount                   = 0;
}
