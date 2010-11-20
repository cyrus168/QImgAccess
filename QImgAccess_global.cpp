
#include <QVector>
#include <QColor>

#include "QImgAccess_global.h"

bool cvqCopyImage(const IplImage *pIplImage, QImage &qImage)
{
    int w, h;

    if (!CV_IS_IMAGE(pIplImage))
        return false;

    w = pIplImage->width;
    h = pIplImage->height;

    if (w != qImage.width() || h != qImage.height())
    {
        if (pIplImage->depth != IPL_DEPTH_8U)
            return false;

        switch (pIplImage->nChannels)
        {
            case 1:
            {
                QVector<QRgb> clrTable(256);
                for (int i = 0; i < 256; i++)
                    clrTable[i] = QColor(i, i ,i).rgb();

                qImage = QImage(w, h, QImage::Format_Indexed8);

                qImage.setColorTable(clrTable);
            }

            break;

        case 3:
            qImage = QImage(w, h, QImage::Format_RGB888);
            break;
        }
    }

    if (pIplImage->widthStep != qImage.bytesPerLine())
        return false;

    if (pIplImage->nChannels == 1)
    {
       for (int i = 0; i < h; i++)
       {
           char *pDataSrc = NULL;

           pDataSrc = pIplImage->imageData + pIplImage->widthStep * i;

           memcpy(qImage.scanLine(i), pDataSrc, pIplImage->widthStep);
       }
    }
    else if (pIplImage->nChannels == 3)
    {
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++)
            {
                CvScalar clr = cvGet2D(pIplImage, j, i);

                qImage.setPixel(i, j, QColor(clr.val[2], clr.val[1], clr.val[0]).rgb());
            }
        }
    }

    if (pIplImage->origin != IPL_ORIGIN_TL)
        qImage = qImage.mirrored(false, true);

    return true;
}
