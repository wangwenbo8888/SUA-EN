#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMenu>

#include <QLoggingCategory>

#include "constant.h"

//#include <OperationDialog/thresholddialog.h>
//#include <OperationDialog/regiongrowingdialog.h>
//#include <OperationDialog/surfacerenderingdialog.h>
#include <QBitArray>
#include <math.h>
#include <QProcess>
#include <QVector>
#include <QTableView>
#include <QStandardItemModel>

#include "gdcmReader.h"
#include "gdcmMediaStorage.h"
#include "gdcmStringFilter.h"
#include "gdcmAttribute.h"
#include "gdcmValue.h"
#include "gdcmType.h"
#include "gdcmVR.h"
#include "gdcmPersonName.h"
#include "gdcmString.h"

//单元测试用

//const double pi=3.1415926535;
const QString STYLE_OPERATING = "background-color: rgb(0, 170, 0);color:rgb(0,0,0);border-radius:5px;";

const QString NOT_WORK = "color: rgb(146, 146, 146);border-radius:5px;";
const QString WORK = "color: rgb(4, 222, 230);border-radius:5px;";
// 背景象牙白
const QString SELECTED = "background-color: rgb(250, 255, 240);color:rgb(0,0,0);border-radius:5px;";

//const QString HORIZON_TYPE = //滑条
//        "QSlider::groove:horizontal{"
//        " border: 1px solid #999999;"
//        " height: 5px; "

//        "position: absolute;"
//        "border-radius :3px;"
//        " background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
//        " margin: 1px 0;"
//        " left: 3px; right: 3px;"
//        "}"
//        //小滑块
//        "QSlider::handle:horizontal {"
//        " background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
//       "border: 1px solid #5c5c5c;"
//        "width: 12px;"
//        " height: 12px; "
//        "margin: -2px -1; "/* handle is placed by default on the contents rect of the groove. Expand outside the groove */
//        "  border-radius: 4px;"
//        "}"
//        //滑块滑过改变样式
//        " QSlider::add-page:horizontal {"
//        "border-radius :2px;"
//        " margin: 1px 1;"
//        "background: white;"
//        //  "position: absolute;"
//        //  " left: 2px; right: 2px;"
//        "}"
//        "QSlider::sub-page:horizontal {"
//        "border-radius :2px;"
//        " margin: 1px 1;"//
//        "  background-color:#86dfc6;"
//        // "position: absolute;"
//        // " left: 2px; right: 2px;"
//        "}";

Q_DECLARE_LOGGING_CATEGORY(HIFUCONNECTION)

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    void ClearAllData();
    bool defaultdicomtest();


protected:
    //void closeEvent(QCloseEvent *event);

public slots:


signals:


private slots:


//    void on_pushButton_echoscu_clicked();

    void on_pushButton_findname_clicked();

//    void on_pushButton_findstudy_clicked();

//    void on_pushButton_movescu_clicked();

    void on_pushButton_nameconfirm_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void self_echoscu();

    void SetTableViewHeader();

    void on_pushButton_movestudy_clicked();

    void on_pushButton_input_clicked();

    void setToolTip();

private:
    Ui::MainDialog *ui;
//    QProcess findscu;
//    QVector<QString> StudyUID;
//    QVector<QString> SeriesUID;
//    void setToolTip();
    QProcess findscu;
    QVector<QString> PatientName;
    QVector<QString> PatientID;
    QVector<QString> StudyUID;
    QVector<QString> StudyDate;
    QVector<QString> SeriesUID;
    QVector<QString> Description;
    bool isechoscufailed=1;
    bool isadd_seriesuid = 0;
    bool isadd_description =0;
    int patientnameid;
    QString patientidnum;

    QStringList dcm_list;

    QStandardItemModel *model = new QStandardItemModel();
    QTableView * tableview = new QTableView();




};

#endif // MAINDIALOG_H
