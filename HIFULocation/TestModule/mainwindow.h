#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QLabel;
class QLineEdit;
class QPushButton;

#include <QMainWindow>

#include "location.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onBtnAngleClicked();
    void onBtnLAxisClicked();
    void onBtnFAxisClicked();
    void onBtnZAxisClicked();

    void onStatusUpdated(AID, APOS);
    void onErrorUpdated(ERRORTYPE);

private:
    QPushButton* m_btnAngle;
    QPushButton* m_btnLAxis;
    QPushButton* m_btnFAxis;
    QPushButton* m_btnZAxis;
    QLineEdit* m_editAngle;
    QLineEdit* m_editLAxis;
    QLineEdit* m_editFAxis;
    QLineEdit* m_editZAxis;
    QLabel* m_labelAngle;
    QLabel* m_labelLAxis;
    QLabel* m_labelFAxis;
    QLabel* m_labelZAxis;
    QLabel* m_status;

    Location* m_location;
};

#endif // MAINWINDOW_H
