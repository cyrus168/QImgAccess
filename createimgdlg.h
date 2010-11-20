#ifndef CREATEIMGDLG_H
#define CREATEIMGDLG_H

#include <QDialog>

#include "QImgAccess_global.h"

namespace Ui {
    class CreateImgDlg;
}

class CreateImgDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CreateImgDlg(QWidget *parent = 0);
    ~CreateImgDlg();

    QSize GetImgSize();
    QString GetImgVarName();
    ImageType GetImgType();

public slots:
    void DataAccepted();

private:
    Ui::CreateImgDlg *ui;

    ImageType m_ImgType;
    QSize m_ImgSize;
    QString m_ImgVarName;
};

#endif // CREATEIMGDLG_H
