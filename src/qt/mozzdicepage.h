#ifndef ALTDICEPAGE_H
#define ALTDICEPAGE_H

#include <QWidget>

namespace Ui {
class MozzdicePage;
}

class MozzdicePage : public QWidget
{
    Q_OBJECT

public:
    explicit MozzdicePage(QWidget *parent = 0);
    ~MozzdicePage();

private:
    Ui::MozzdicePage *ui;
};

#endif // ALTDICEPAGE_H
