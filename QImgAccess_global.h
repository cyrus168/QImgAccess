#ifndef QIMGACCESS_GLOBAL_H
#define QIMGACCESS_GLOBAL_H

#include <QImage>

#include <cv.h>
#include <highgui.h>

//
//
//
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
#define LOWBYTE(a) ((BYTE)(a))
#define HIBYTE(a)  ((BYTE)(((WORD)(a) >> 8) & 0xFF))
#define LOWORD(a)  ((WORD)(a))
#define HIWORD(a)  ((WORD)(((DWORD)(a) >> 16) & 0xFFFF))

#define MAKEWORD( h, l ) \
  ( ( ( WORD )( h ) << 8 ) | ( BYTE )( l ) )

#define MAKEDWORD( h, l ) \
  ( ( ( DWORD )( h ) << 16 ) | ( WORD )( l ) )

//
//
//
enum ImageType
{
    IMG_UNKNOWN_TYPE = -1,
    IMG_BW8,
    IMG_BW16,
    IMG_C24,
};

struct C24
{
    BYTE c1;
    BYTE c2;
    BYTE c3;
};

//
//
//
bool cvqCopyImage(const IplImage *pIplImage, QImage &qImage);

#endif // QIMGACCESS_GLOBAL_H
