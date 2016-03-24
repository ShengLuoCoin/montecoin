#ifndef ALTCOINCONTACTPAGE_H
#define ALTCOINCONTACTPAGE_H

#include <QWidget>

namespace Ui {
class MozzriaContactPage;
}

class MozzriaContactPage : public QWidget
{
    Q_OBJECT

public:
    explicit MozzriaContactPage(QWidget *parent = 0);
    ~MozzriaContactPage();

private:
    Ui::MozzriaContactPage *ui;
};

#endif // ALTCOINCONTACTPAGE_H
