#ifndef SURFACERENDERINGDIALOG_H
#define SURFACERENDERINGDIALOG_H

#include <QDialog>

namespace Ui {
class SurfaceRenderingDialog;
}

class SurfaceRenderingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SurfaceRenderingDialog(QWidget *parent = 0);
    ~SurfaceRenderingDialog();

signals:
    void SurfaceRenderingProcess();

private slots:
    void on_pushButton_SurfaceRendering_clicked();

private:
    Ui::SurfaceRenderingDialog *ui;
};

#endif // SURFACERENDERINGDIALOG_H
