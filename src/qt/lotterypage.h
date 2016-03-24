#ifndef LOTTERYPAGE_H
#define LOTTERYPAGE_H

#include <QWidget>

namespace Ui {
class LotteryPage;
}

class LotteryPage : public QWidget
{
    Q_OBJECT

public:
    explicit LotteryPage(QWidget *parent = 0);
    ~LotteryPage();

private:
    Ui::LotteryPage *ui;
};

#endif // LOTTERYPAGE_H
