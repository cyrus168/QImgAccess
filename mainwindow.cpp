#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QMdiSubWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imgchildwnd.h"
#include "createimgdlg.h"

//---------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createToolbars();
    createStatusbar();

    setWindowTitle(tr("QImgAccess"));

    setAttribute(Qt::WA_DeleteOnClose);
}
//---------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//---------------------------------------------------------------------------
void MainWindow::createActions()
{
    /// menu actions
    //
    ui->actionOpen->setStatusTip(tr("Open A Image File!"));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openImageFile()));

    ui->actionClose->setStatusTip(tr("Close image/vector!"));

    ui->actionExit->setStatusTip(tr("Exit This Application!"));
    ui->actionExit->setShortcut(tr("Ctrl+Q"));
    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    //
    ui->actionNewImage->setStatusTip(tr("Create A New Image!"));
    connect(ui->actionNewImage, SIGNAL(triggered()), this, SLOT(newImageFile()));    

    connect(ui->actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(ui->actionZoomNormal, SIGNAL(triggered()), this, SLOT(zoomNormal()));

    //
    ui->actionAboutQt->setStatusTip(tr("Show the Qt library's About box"));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    ///
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateActions()));
}
//---------------------------------------------------------------------------
void MainWindow::createToolbars()
{
    ui->mainToolBar->addAction(ui->actionZoomIn);
    ui->mainToolBar->addAction(ui->actionZoomOut);
}
//---------------------------------------------------------------------------
void MainWindow::createStatusbar()
{
    m_plblStatusbarMousePos = new QLabel("(X, Y)");
    m_plblStatusbarMousePos->setAlignment(Qt::AlignLeft);
    m_plblStatusbarMousePos->setMinimumSize(m_plblStatusbarMousePos->sizeHint());

    m_plblStatusbarImageVal = new QLabel("0");
    m_plblStatusbarImageVal->setAlignment(Qt::AlignLeft);
    m_plblStatusbarImageVal->setMinimumSize(m_plblStatusbarImageVal->sizeHint());

    m_plblStatusbarZoom = new QLabel("100%");
    m_plblStatusbarZoom->setAlignment(Qt::AlignLeft);
    m_plblStatusbarZoom->setMinimumSize(m_plblStatusbarZoom->sizeHint());

    ui->statusBar->addWidget(m_plblStatusbarMousePos);
    ui->statusBar->addWidget(m_plblStatusbarImageVal);
    ui->statusBar->addWidget(m_plblStatusbarZoom);

    ui->statusBar->layout()->setAlignment(m_plblStatusbarMousePos, Qt::AlignLeft);
    ui->statusBar->layout()->setAlignment(m_plblStatusbarImageVal, Qt::AlignLeft);
    ui->statusBar->layout()->setAlignment(m_plblStatusbarZoom, Qt::AlignRight);
}

//---------------------------------------------------------------------------
void MainWindow::updateActions()
{
    bool bEnable = false;
    ImgChildWnd *pImgWnd = activeImgChildWnd();

    bEnable = (pImgWnd != 0);

    ui->actionZoomIn->setEnabled(bEnable);
    ui->actionZoomNormal->setEnabled(bEnable);
    ui->actionZoomOut->setEnabled(bEnable);
}
//---------------------------------------------------------------------------
void MainWindow::imgWndZoomChanged(float fZoomFactor)
{
    QString sValue;

    sValue = QString::number(fZoomFactor * 100.0f) + "%";

    m_plblStatusbarZoom->setText(sValue);
}

//---------------------------------------------------------------------------
void MainWindow::updateStatusbar(int nType, DWORD lParam, DWORD wParam)
{
    QString sPos, sValue;
    int x, y;
    int c1, c2, c3;

    switch (nType)
    {
    case WM_STATUSBAR_MOUSEMOVE_MSG_CLEAR:
        sPos = "(X, Y)";
        sValue = "";
        break;

    case WM_STATUSBAR_MOUSEMOVE_MSG_GRAY:
        x = HIWORD(lParam);
        y = LOWORD(lParam);
        sPos = "(X=" + QString::number(x) + ", Y=" + QString::number(y) + ")";

        c1 = wParam;
        sValue = "Gray = " + QString::number(c1);

        break;

    case WM_STATUSBAR_MOUSEMOVE_MSG_COLOR:
        x = HIWORD(lParam);
        y = LOWORD(lParam);
        sPos = "(X=" + QString::number(x) + ", Y=" + QString::number(y) + ")";

        c1 = LOWBYTE(HIWORD(wParam));
        c2 = HIBYTE(LOWORD(wParam));
        c3 = LOWBYTE(LOWORD(wParam));

        sValue = "R=" + QString::number(c1) +
                 ", G=" + QString::number(c2) +
                 ", B=" + QString::number(c3);
        break;

    default:
        break;
    }

    m_plblStatusbarMousePos->setText(sPos);
    m_plblStatusbarImageVal->setText(sValue);
}

//---------------------------------------------------------------------------
void MainWindow::newImageFile()
{
    QSize size;
    ImageType type;
    CreateImgDlg dlg(this);

    if (dlg.exec() != QDialog::Accepted)
        return;

    size = dlg.GetImgSize();
    type = dlg.GetImgType();

    //
    createMdiNewImage(size, type, dlg.GetImgVarName());
}
//---------------------------------------------------------------------------
void MainWindow::openImageFile()
{
    bool bOK = false;
    QString sFileName;
    QString sVarName;

    sFileName = QFileDialog::getOpenFileName(this, tr("Open Image File"), tr("."),
        tr("Images (*.png *.jpg *.bmp *.tif)"));

    if (!sFileName.isEmpty())
    {
        sVarName = QInputDialog::getText(this, tr("Variable"), tr("Name"), QLineEdit::Normal,
                                         tr("Image"), &bOK);

        if (bOK && !sVarName.isEmpty())
            createMdiNewImage(sFileName, sVarName);
    }

}
//---------------------------------------------------------------------------
void MainWindow::zoomIn()
{
    ImgChildWnd *pWnd = activeImgChildWnd();

    if (pWnd)
        pWnd->zoomIn();
}
//---------------------------------------------------------------------------
void MainWindow::zoomOut()
{
    ImgChildWnd *pWnd = activeImgChildWnd();

    if (pWnd)
        pWnd->zoomOut();
}
//---------------------------------------------------------------------------
void MainWindow::zoomNormal()
{
    ImgChildWnd *pWnd = activeImgChildWnd();

    if (pWnd)
        pWnd->zoomNormal();
}

//---------------------------------------------------------------------------
ImgChildWnd* MainWindow::createMdiNewImage(QString sFileName, QString sVarName)
{
    ImgChildWnd *pChild = new ImgChildWnd(sFileName, sVarName, this);

    addImgChildWnd(pChild);

    return pChild;
}
//---------------------------------------------------------------------------
ImgChildWnd* MainWindow::createMdiNewImage(QSize imgSize, ImageType type, QString sVarName)
{
    ImgChildWnd *pChild = new ImgChildWnd(imgSize, type, sVarName, this);

    addImgChildWnd(pChild);

    return pChild;
}

//---------------------------------------------------------------------------
void MainWindow::addImgChildWnd(ImgChildWnd *pWnd)
{
    QMdiSubWindow *pChildWnd = NULL;

    pChildWnd = ui->mdiArea->addSubWindow(pWnd);

    connect(pWnd, SIGNAL(zoomInActionAvailable(bool)), ui->actionZoomIn, SLOT(setEnabled(bool)));
    connect(pWnd, SIGNAL(zoomOutActionAvailable(bool)), ui->actionZoomOut, SLOT(setEnabled(bool)));
    connect(pWnd, SIGNAL(zoomNormalActionAvailable(bool)), ui->actionZoomNormal, SLOT(setEnabled(bool)));

    connect(pWnd, SIGNAL(zoomFactorChanged(float)), this, SLOT(imgWndZoomChanged(float)));

    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);

    pWnd->show();
}
//---------------------------------------------------------------------------
ImgChildWnd* MainWindow::activeImgChildWnd()
{
    QMdiSubWindow *pSubWnd = 0;
    ImgChildWnd* pImgWnd = 0;

    pSubWnd = ui->mdiArea->activeSubWindow();
    if (pSubWnd)
    {
        QWidget *pWidget = pSubWnd->widget();

        if (pWidget)
            pImgWnd = qobject_cast<ImgChildWnd *>(pWidget);
    }

    return pImgWnd;
}
//---------------------------------------------------------------------------
