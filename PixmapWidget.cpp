
#include <QPainter>
#include <QMouseEvent>

#include <stdio.h>
#include <stdlib.h>

#include "PixmapWidget.h"

//-----------------------------------------------------------------------------
PixmapWidget::PixmapWidget(const QImage *image, QWidget *parent) :
    QWidget(parent)
{
    m_Pixmap = QPixmap::fromImage(*image);
    m_fZoomScale = 1.0f;

    // enable mouse-tracking, prevent mouse-move being ignored
    setMouseTracking(true);

    setMinimumSize(m_Pixmap.width() * m_fZoomScale, m_Pixmap.height() * m_fZoomScale);
}
//-----------------------------------------------------------------------------
float PixmapWidget::getZoomFactor()
{
    return m_fZoomScale;
}

//-----------------------------------------------------------------------------
void PixmapWidget::paintEvent(QPaintEvent *)
{
    bool bDrawBorder = false;
    int dx = 0, dy = 0;
    int dw = 0, dh = 0;
    QPainter painter(this);

    dw = m_Pixmap.width() * m_fZoomScale;
    dh = m_Pixmap.height() * m_fZoomScale;

    if (width() > dw)
    {
        dx = (width() - dw) / 2;
        bDrawBorder = true;
    }

    if (height() > dh)
    {
        dy = (height() - dh) / 2;
        bDrawBorder = true;
    }

    painter.save();
        painter.translate(dx, dy);
        painter.scale(m_fZoomScale, m_fZoomScale);
        painter.drawPixmap(0, 0, m_Pixmap);
    painter.restore();

    if (bDrawBorder)
    {
        painter.setPen(Qt::darkGray);
        painter.drawRect(dx - 1, dy - 1, dw + 1, dh + 1);
    }    
}
//-----------------------------------------------------------------------------
void PixmapWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint pt;
    int dx, dy, dw, dh;

    dw = m_Pixmap.width() * m_fZoomScale;
    dh = m_Pixmap.height() * m_fZoomScale;

    dx = dy = 0;
    if (width() > dw)
        dx = (width() - dw) / 2;

    if (height() > dh)
        dy = (height() - dh) / 2;


    pt.setX((e->x() - dx) / m_fZoomScale);
    pt.setY((e->y() - dy) / m_fZoomScale);

    if (pt.x() >= 0 && pt.y() >= 0)
        emit mouseImgPixelMoved(pt.x(), pt.y());
}

//-----------------------------------------------------------------------------
void PixmapWidget::setZoomFactor(float f)
{
    int w, h;
    QWidget *pWnd;

    if (f == m_fZoomScale)
        return;

    m_fZoomScale = f;

    emit zoomFactorChanged(f);

    w = m_Pixmap.width() * m_fZoomScale;
    h = m_Pixmap.height() * m_fZoomScale;

    setMinimumSize(w, h);

    pWnd = dynamic_cast<QWidget *>( parent() );
    if (pWnd != NULL)
        pWnd->resize(QSize(w, h));

    repaint();
}
//-----------------------------------------------------------------------------

