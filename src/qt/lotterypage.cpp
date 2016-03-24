#include "lotterypage.h"
#include "ui_lotterypage.h"

LotteryPage::LotteryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LotteryPage)
{
    ui->setupUi(this);
}

LotteryPage::~LotteryPage()
{
    delete ui;
}
