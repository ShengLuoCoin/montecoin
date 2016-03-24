#include "mozzriaaboutpage.h"
#include "ui_mozzriaaboutpage.h"
#include <QDesktopServices>
#include <QUrl>

MozzriaAboutPage::MozzriaAboutPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MozzriaAboutPage)
{
    ui->setupUi(this);

    connect(ui->label_connect,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
}

MozzriaAboutPage::~MozzriaAboutPage()
{
    delete ui;
}

void MozzriaAboutPage::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}
