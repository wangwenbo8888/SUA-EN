#ifndef TRAMENTMENTPARATABLES_H
#define TRAMENTMENTPARATABLES_H

#include <QStackedWidget>

namespace Ui {
class tramentmentparatables;
}

class tramentmentparatables : public QStackedWidget
{
    Q_OBJECT

public:
    explicit tramentmentparatables(QWidget *parent = 0);
    ~tramentmentparatables();

private:
    Ui::tramentmentparatables *ui;
};

#endif // TRAMENTMENTPARATABLES_H
