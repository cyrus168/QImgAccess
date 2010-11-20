#ifndef IMGCHILDWND_H
#define IMGCHILDWND_H

#include <QScrollArea>
#include <QVBoxLayout>

#include "QImgAccess_global.h"

class MainWindow;
class PixmapWidget;

class ImgChildWnd : public QWidget
{
    Q_OBJECT

public:    
    explicit ImgChildWnd(QString sFileName, QString sVarName, QWidget *parent = 0);
    explicit ImgChildWnd(QSize imgSize, ImageType Type, QString sVarName, QWidget *parent = 0);
    ~ImgChildWnd();

signals:
    void zoomInActionAvailable(bool);
    void zoomNormalActionAvailable(bool);
    void zoomOutActionAvailable(bool);
    void zoomFactorChanged(float);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomNormal();

    void zoomActionUpdate(float fZoomFactor);
    void mouseMoved(int X, int Y);

private:
    IplImage *m_pIplImage;

    QVBoxLayout *layout;
    QScrollArea *scrollArea;
    PixmapWidget *pixmapWidget;
    MainWindow *m_pMainWnd;

    float m_fZoomFactor;

    void createContent(const QImage *pImage, QWidget *Parnet = 0);
    MainWindow* getMainWindow(void);

protected:           
    void resizeEvent(QResizeEvent *);

    bool loadImageFile(QString sFileName, QImage &img);

};

#endif // IMGCHILDWND_H
