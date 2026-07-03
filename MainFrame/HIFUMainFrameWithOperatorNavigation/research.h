#ifndef RESEARCH_H
#define RESEARCH_H

#include <QDialog>

namespace Ui {
class research;
}

class research : public QDialog
{
    Q_OBJECT

public:
    explicit research(QWidget *parent = 0);
    ~research();

private:
    Ui::research *ui;
};

#endif // RESEARCH_H
