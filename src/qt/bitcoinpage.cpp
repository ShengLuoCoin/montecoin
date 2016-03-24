#include "bitcoinpage.h"
#include "ui_bitcoinpage.h"
#include <QDesktopServices>
#include <QUrl>

BitcoinPage::BitcoinPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BitcoinPage)
{
    ui->setupUi(this);
    connect(ui->label_ex,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->label_explorer,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->label_forum,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->label_pool,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->label_web,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->label_soundcode,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
}

BitcoinPage::~BitcoinPage()
{
    delete ui;
}

void BitcoinPage::on_label_4_linkActivated(const QString &link)
{

}

void BitcoinPage::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}
