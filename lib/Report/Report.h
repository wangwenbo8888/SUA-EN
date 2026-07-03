#ifndef REPORT_H_
#define REPORT_H_

#include <QObject>
#include <QLoggingCategory>

#include "constant.h"
#include "variable.h"
#include "report_global.h"

Q_DECLARE_LOGGING_CATEGORY(REPORT)

// 脚踏开关和指示灯控制类
class REPORTSHARED_EXPORT Report : public QObject
{
    Q_OBJECT

public:
    Report(QObject *parent=0);
    ~Report();

    void createPDF(QString filePath);

signals:


private:

private slots:

};


#endif // REPORT_H_
