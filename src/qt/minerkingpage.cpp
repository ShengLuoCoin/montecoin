#include "minerkingpage.h"
#include "ui_minerkingpage.h"

#include <QtGui>

MinerkingPage::MinerkingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinerkingPage)
{
    ui->setupUi(this);

   // imageBackground.load(":/icons/minerking");
   // setAutoFillBackground(true);

    connect(ui->lb_download_andr,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->lb_download_ios,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->lb_officeweb_mk,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    connect(ui->Button_deposit,SIGNAL(clicked()),this,SLOT(on_click_button_deposit()));
}

MinerkingPage::~MinerkingPage()
{
    delete ui;
}

void MinerkingPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPalette pal(palette());   // 注意下面这一行，就是靠他实现大小拉伸
    pal.setBrush(QPalette::Window,
                 QBrush(imageBackground.scaled(event->size(),
                                      Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation))
                 );
    setPalette(pal);
}

void MinerkingPage::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void MinerkingPage::on_click_button_deposit()
{
    emit deposit_minerking();
}
