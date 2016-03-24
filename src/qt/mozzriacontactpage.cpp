#include "mozzriacontactpage.h"
#include "ui_mozzriacontactpage.h"

MozzriaContactPage::MozzriaContactPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MozzriaContactPage)
{
    ui->setupUi(this);
}

MozzriaContactPage::~MozzriaContactPage()
{
    delete ui;
}
