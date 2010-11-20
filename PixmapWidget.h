#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QImage>

class PixmapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PixmapWidget(const QImage *image, QWidget *parent = 0);

    float getZoomFactor();

signals:
    void zoomFactorChanged(float);
    void mouseImgPixelMoved(int X, int Y);

public slots:
    void setZoomFactor(float);

private:
    QPixmap m_Pixmap;
    float   m_fZoomScale;

protected:
    void paintEvent(QPaintEvent *);    
    void mouseMoveEvent(QMouseEvent *);
};

#endif // PIXMAPWIDGET_H
