#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "QImgAccess_global.h"

class ImgChildWnd;

#define WM_STATUSBAR_MOUSEMOVE_MSG_CLEAR    1001
#define WM_STATUSBAR_MOUSEMOVE_MSG_GRAY     1002
#define WM_STATUSBAR_MOUSEMOVE_MSG_COLOR    1003

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QLabel *m_plblStatusbarMousePos;
    QLabel *m_plblStatusbarImageVal;
    QLabel *m_plblStatusbarZoom;

private slots:
    void newImageFile();
    void openImageFile();

    void zoomIn();
    void zoomOut();
    void zoomNormal();

    void updateActions();

    void imgWndZoomChanged(float fZoomFactor);

public slots:
    void updateStatusbar(int nType, DWORD lParam, DWORD wParam);

private:
    ImgChildWnd* createMdiNewImage(QSize imgSize, ImageType Type, QString sVarName);
    ImgChildWnd* createMdiNewImage(QString sFileName, QString sVarName);


protected:
    void createActions();
    void createToolbars();
    void createStatusbar();

    //
    void addImgChildWnd(ImgChildWnd *);
    ImgChildWnd* activeImgChildWnd();
};

#endif // MAINWINDOW_H
