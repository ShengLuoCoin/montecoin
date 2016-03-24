#ifndef ALTCOINABOUTPAGE_H
#define ALTCOINABOUTPAGE_H

#include <QWidget>

namespace Ui {
class MozzriaAboutPage;
}

class MozzriaAboutPage : public QWidget
{
    Q_OBJECT

public:
    explicit MozzriaAboutPage(QWidget *parent = 0);
    ~MozzriaAboutPage();

private slots:
    void openUrl(QString url);

private:
    Ui::MozzriaAboutPage *ui;
};

#endif // ALTCOINABOUTPAGE_H
