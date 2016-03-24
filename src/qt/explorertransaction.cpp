#include "explorertransaction.h"
#include "ui_explorertransaction.h"
#include <QTime>
#include "explorertxentry.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;

ExplorerTransaction::ExplorerTransaction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerTransaction)
{
    ui->setupUi(this);
}

ExplorerTransaction::~ExplorerTransaction()
{
    delete ui;
}

void ExplorerTransaction::rpcFoundTx(QString msg)
{
    clear();

    std::string strPrint = msg.toStdString();
    std::stringstream ss(strPrint);
    std::string strValue;
    ptree pt,ptTxs,p2;

    try{
      read_json(ss, pt);
    }
    catch(ptree_error & e) {
      return;
    }

    try{
        strValue = pt.get<std::string>("txid");
        ui->label_hash->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("blockhash");
        ui->pb_includeInBlock->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("time");
        QString unixTime = QString::fromStdString(strValue);
        QString timet = QDateTime::fromTime_t(unixTime.toLongLong()).toString();
        unixTime = unixTime + QString(" (") + timet + QString(")");
        ui->label_block_time->setText(unixTime);

        strValue = pt.get<std::string>("confirmations");
        ui->label_confirmations->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("version");
        ui->label_version->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("locktime");
        ui->label_locktime->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("coinsout");
        ui->label_coinsout->setText(QString::fromStdString(strValue) + QString(" MTC"));

       /* int pos = msg.toStdString().find("generated",0);
        if(pos > 0)
        {
            ui->label_gen->setText(QString("True"));
        }
        else
        {
            ui->label_gen->setText(QString("False"));
        }*/

        ExplorerTxEntry *txEntry = new ExplorerTxEntry(pt);
        ui->entries->addWidget(txEntry);
    }
    catch (ptree_error & e)
    {
      return;
    }
}

void ExplorerTransaction::clear()
{
    ui->label_hash->setText("");
    ui->label_block_time->setText("");
    ui->label_coinsout->setText("");
    ui->label_confirmations->setText("");
    ui->label_locktime->setText("");
    ui->label_version->setText("");
    ui->pb_includeInBlock->setText("");

    while(ui->entries->count())
    {
        delete ui->entries->takeAt(0)->widget();
    }
}

void ExplorerTransaction::on_pb_includeInBlock_clicked()
{
    QString hash = ui->pb_includeInBlock->text();
    if(hash.size() == 64)
    {
        emit searchBlockByHash(hash);
    }
}
