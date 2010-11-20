#include "createimgdlg.h"
#include "ui_createimgdlg.h"

//-----------------------------------------------------------------------------
CreateImgDlg::CreateImgDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateImgDlg)
{
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(DataAccepted()));
}
//-----------------------------------------------------------------------------
CreateImgDlg::~CreateImgDlg()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void CreateImgDlg::DataAccepted()
{
    int w, h;

    w = ui->edtWidth->text().toInt();
    h = ui->edtHeight->text().toInt();

    m_ImgSize = QSize(w, h);

    if (ui->btnGray->isChecked())
        m_ImgType = IMG_BW8;
    else if (ui->btnColor->isChecked())
        m_ImgType = IMG_C24;

    m_ImgVarName = ui->edtVarName->text();
}

//-----------------------------------------------------------------------------
QSize CreateImgDlg::GetImgSize()
{
    return m_ImgSize;
}
//-----------------------------------------------------------------------------
ImageType CreateImgDlg::GetImgType()
{
    return m_ImgType;
}

//-----------------------------------------------------------------------------
QString CreateImgDlg::GetImgVarName()
{
    return m_ImgVarName;
}
//-----------------------------------------------------------------------------
