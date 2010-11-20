#include <QStylePainter>
#include <QImage>
#include <QMdiArea>

#include "QImgAccess_global.h"
#include "mainwindow.h"
#include "imgchildwnd.h"
#include "PixmapWidget.h"

//-----------------------------------------------------------------------------
ImgChildWnd::ImgChildWnd(QString sFileName, QString sVarName, QWidget *parent) :
        QWidget(parent), m_pIplImage(0)
{    
    QImage img;
    bool br = false;

    br = loadImageFile(sFileName, img);

    if (br)
    {
        createContent(&img, parent);

        setWindowTitle(sVarName);
    }
}
//-----------------------------------------------------------------------------
ImgChildWnd::ImgChildWnd(QSize Size, ImageType Type, QString sVarName, QWidget *parent) :
        m_pIplImage(0)
{
    CvSize size = cvSize(Size.width(), Size.height());

    switch (Type)
    {
    case IMG_BW8:
            m_pIplImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
        break;


    case IMG_C24:
            m_pIplImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
        break;

    default:
        break;
    }

    if (m_pIplImage != 0)
    {
        QImage img;

        cvSetZero(m_pIplImage);

        cvqCopyImage(m_pIplImage, img);

        createContent(&img, parent);
    }

    setWindowTitle(sVarName);
}
//-----------------------------------------------------------------------------
ImgChildWnd::~ImgChildWnd()
{
    if (m_pIplImage != NULL)
    {
        cvReleaseImage(&m_pIplImage);
        m_pIplImage = NULL;
    }
}

//-----------------------------------------------------------------------------
void ImgChildWnd::createContent(const QImage *pImage, QWidget *parent)
{
    m_pMainWnd = reinterpret_cast<MainWindow *>(parent);

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    layout->addWidget(scrollArea);

    pixmapWidget = new PixmapWidget(pImage, this);

    scrollArea->setWidget(pixmapWidget);

    //
    setLayout(layout);

    //
    m_fZoomFactor = 1.0f;
    pixmapWidget->setZoomFactor(m_fZoomFactor);

    //
    connect(pixmapWidget, SIGNAL(zoomFactorChanged(float)), this, SLOT(zoomActionUpdate(float)));
    connect(pixmapWidget, SIGNAL(mouseImgPixelMoved(int,int)), this, SLOT(mouseMoved(int,int)));
}

//-----------------------------------------------------------------------------
MainWindow* ImgChildWnd::getMainWindow(void)
{
    return m_pMainWnd;
}
//-----------------------------------------------------------------------------
void ImgChildWnd::zoomIn()
{
    float factor = pixmapWidget->getZoomFactor();
    float fNew = factor * 2.0f;

    if (fNew <= 8.0f)
        pixmapWidget->setZoomFactor(fNew);
}
//-----------------------------------------------------------------------------
void ImgChildWnd::zoomOut()
{
    float factor = pixmapWidget->getZoomFactor();
    float fNew = factor / 2.0f;

    if (fNew >= 0.125f)
        pixmapWidget->setZoomFactor(fNew);
}
//-----------------------------------------------------------------------------
void ImgChildWnd::zoomNormal()
{
    pixmapWidget->setZoomFactor(1.0f);
}
//-----------------------------------------------------------------------------
void ImgChildWnd::zoomActionUpdate(float fZoomFactor)
{
    if (fZoomFactor >= 8.0f)
    {
        emit zoomInActionAvailable(false);
    }
    else if (fZoomFactor <= 0.125f)
    {
        emit zoomOutActionAvailable(false);
    }
    else
    {
        emit zoomInActionAvailable(true);
        emit zoomOutActionAvailable(true);
        emit zoomNormalActionAvailable(true);
    }

    emit zoomFactorChanged(fZoomFactor);
}

//-----------------------------------------------------------------------------
void ImgChildWnd::mouseMoved(int X, int Y)
{
    bool bClearStatusbar = false;
    MainWindow *pWnd = getMainWindow();

    if (pWnd == 0 || m_pIplImage == 0)
        return;

    if (X >= m_pIplImage->width)
        bClearStatusbar = true;

    if (Y >= m_pIplImage->height)
        bClearStatusbar = true;

    if (bClearStatusbar)
    {
        pWnd->updateStatusbar(WM_STATUSBAR_MOUSEMOVE_MSG_CLEAR, 0, 0);
    }
    else
    {
        DWORD lParam = MAKEDWORD(X, Y);
        DWORD wParam = 0;

        switch (m_pIplImage->depth)
        {
        case IPL_DEPTH_8U:
            if (m_pIplImage->nChannels == 1)
            {
                BYTE *ptr = (BYTE *)(m_pIplImage->imageData + Y * m_pIplImage->widthStep);

                wParam = MAKEDWORD(MAKEWORD(0,0), MAKEWORD(0, ptr[X]));

                pWnd->updateStatusbar(WM_STATUSBAR_MOUSEMOVE_MSG_GRAY, lParam, wParam);
            }
            else if (m_pIplImage->nChannels == 3)
            {
                C24 *ptr = (C24 *)(m_pIplImage->imageData + Y * m_pIplImage->widthStep);
                C24 val = ptr[X];

                wParam = MAKEDWORD(MAKEWORD(0, val.c3), MAKEWORD(val.c2, val.c1));

                pWnd->updateStatusbar(WM_STATUSBAR_MOUSEMOVE_MSG_COLOR, lParam, wParam);
            }
            break;

        default:
            break;
        }
    }

}

//-----------------------------------------------------------------------------
void ImgChildWnd::resizeEvent(QResizeEvent *)
{
}

//-----------------------------------------------------------------------------
bool ImgChildWnd::loadImageFile(QString sFileName, QImage &img)
{
    bool br = false;    

    m_pIplImage = cvLoadImage(sFileName.toAscii().constData(), CV_LOAD_IMAGE_UNCHANGED);
    if (m_pIplImage != NULL)
    {
        br = cvqCopyImage(m_pIplImage, img);

        if (br)
        {
            setWindowTitle(sFileName);
        }
    }


    return br;
}
//-----------------------------------------------------------------------------
