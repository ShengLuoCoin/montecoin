#include "exploreraddress.h"
#include "ui_exploreraddress.h"

ExplorerAddress::ExplorerAddress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerAddress)
{
    ui->setupUi(this);
}

ExplorerAddress::~ExplorerAddress()
{
    delete ui;
}
