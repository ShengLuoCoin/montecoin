#include "mozzdicepage.h"
#include "ui_mozzdicepage.h"

MozzdicePage::MozzdicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MozzdicePage)
{
    ui->setupUi(this);
}

MozzdicePage::~MozzdicePage()
{
    delete ui;
}
