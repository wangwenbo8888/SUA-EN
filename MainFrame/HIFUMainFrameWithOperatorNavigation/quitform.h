#ifndef QUITFORM_H
#define QUITFORM_H

#include <QFrame>

namespace Ui {
class quitform;
}

class quitform : public QFrame
{
    Q_OBJECT

public:
    explicit quitform(QWidget *parent = 0);
    ~quitform();

private:
    Ui::quitform *ui;
};

#endif // QUITFORM_H
