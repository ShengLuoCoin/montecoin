#include "explorerblock.h"
#include "ui_explorerblock.h"
#include <QTime>
#include "explorertxentry.h"

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;

static const unsigned int MAX_TX_PEERPAGE = 10;

ExplorerBlock::ExplorerBlock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerBlock)
{
    ui->setupUi(this);
    msgBlock = QString("");

    //input only number
    QRegExp regx("[1-9][0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit_gotopage );
    ui->lineEdit_gotopage->setValidator(validator);

    setTotalPageNo(1);
    setCurPageNo(1);
}

ExplorerBlock::~ExplorerBlock()
{
    delete ui;
}


void ExplorerBlock::rpcFoundBlock(QString msg)
{
    clearTxEntries();

    msgBlock = msg;
    std::string strPrint = msg.toStdString();
    std::stringstream ss(strPrint);
    std::string strValue;
    ptree pt;

    try{
      read_json(ss, pt);
    }
    catch(ptree_error & e) {
      return;
    }

    try{
      strValue = pt.get<std::string>("hash");
      ui->label_hash->setText(QString::fromStdString(strValue));
      int pos = msg.toStdString().find("previousblockhash",0);
      if(pos > 0)
      {
          strValue = pt.get<std::string>("previousblockhash");
          ui->pb_preblock->setText(QString::fromStdString(strValue));
      }
      else
      {
          ui->pb_preblock->setText(QString("NULL"));
      }

      pos = msg.toStdString().find("nextblockhash",0);
      if(pos > 0)
      {
          strValue = pt.get<std::string>("nextblockhash");
          ui->pb_nextBlock->setText(QString::fromStdString(strValue));
      }
      else
      {
          ui->pb_nextBlock->setText(QString("NULL"));
      }

      strValue = pt.get<std::string>("height");
      ui->label_height->setText(QString::fromStdString(strValue));
      strValue = pt.get<std::string>("difficulty");
      ui->label_difficulty->setText(QString::fromStdString(strValue));

      strValue = pt.get<std::string>("time");
      QString unixTime = QString::fromStdString(strValue);
      QString timet = QDateTime::fromTime_t(unixTime.toLongLong()).toString();
      unixTime = unixTime + QString(" (") + timet + QString(")");
      ui->label_timestamp->setText(unixTime);

      strValue = pt.get<std::string>("transactions");
      ui->label_tx->setText(QString::fromStdString(strValue));
      unsigned int txcount;
      txcount = ui->label_tx->text().toInt();
      setTotalPageNo((unsigned int)(txcount/MAX_TX_PEERPAGE + 1));

      strValue = pt.get<std::string>("confirmations");
      ui->label_confirmations->setText(QString::fromStdString(strValue));
      strValue = pt.get<std::string>("coinsout");
      ui->label_coinsout->setText(QString::fromStdString(strValue) + QString(" MTC"));

      strValue = pt.get<std::string>("flags");
      ui->label_type->setText(QString::fromStdString(strValue));
      strValue = pt.get<std::string>("size");
      ui->label_size->setText(QString::fromStdString(strValue));
      strValue = pt.get<std::string>("version");
      ui->label_version->setText(QString::fromStdString(strValue));
      strValue = pt.get<std::string>("bits");
      ui->label_bits->setText(QString::fromStdString(strValue));
      strValue = pt.get<std::string>("nonce");
      ui->label_nonce->setText(QString::fromStdString(strValue));
      strValue = pt.get<std::string>("pow");
      ui->label_wtoreward->setText(QString::fromStdString(strValue) + QString(" MTC"));

      setCurPageNo(1);
      TxEntriesShowPage(1);
    }
    catch (ptree_error & e)
    {
      return;
    }
}

void ExplorerBlock::TxEntriesShowPage(unsigned int nPage)
{
    if(msgBlock.isEmpty() || nPage < 1 || nPage > getTotalPageCount())
        return ;
    clearTxEntries();

    std::string strPrint = msgBlock.toStdString();
    std::stringstream ss(strPrint);
    ptree pt,ptTxs,p2;

    try{
      read_json(ss, pt);
    }
    catch(ptree_error & e) {
      return;
    }

    try{
        ptTxs = pt.get_child("tx");
        int b = (nPage-1) * MAX_TX_PEERPAGE;
        if(b >= ptTxs.size())
            return;
        int i = 0;
        ptree::iterator it = ptTxs.begin();
        while(it != ptTxs.end() && i < b )
        {
            it++;
            i++;
        }

        for (i=0; (it != ptTxs.end()) && (i < MAX_TX_PEERPAGE); ++it,i++)
          {
              p2 = it->second; //first为空
              ExplorerTxEntry *txEntry = new ExplorerTxEntry(p2);
              ui->entries->addWidget(txEntry);
          }
    }
    catch (ptree_error & e)
    {
      return;
    }

    setCurPageNo(nPage);

    if(nPage <= 1)
        ui->pushButton_prepage->setEnabled(false);
    else
        ui->pushButton_prepage->setEnabled(true);

    if(nPage >= getTotalPageCount())
        ui->pushButton_nextpage->setEnabled(false);
    else
        ui->pushButton_nextpage->setEnabled(true);
}

void ExplorerBlock::rpc_getblock(long height)
{
   // QString cmd = QString("getblockhash ") + QString::number(height);
   // emit cmdRequest(cmd);
}

void ExplorerBlock::clearTxEntries()
{
    // Remove entries
    while(ui->entries->count())
    {
        delete ui->entries->takeAt(0)->widget();
    }
}

void ExplorerBlock::on_pb_preblock_clicked()
{
    QString hash = ui->pb_preblock->text();
    if(hash.size() == 64)
    {
        emit searchBlockByHash(hash);
    }
}

void ExplorerBlock::on_pb_nextBlock_clicked()
{
    QString hash = ui->pb_nextBlock->text();
    if(hash.size() == 64)
    {
        emit searchBlockByHash(hash);
    }
}

unsigned int ExplorerBlock::getCurPageNo()
{
    return ui->label_curpage->text().toUInt();
}

unsigned int ExplorerBlock::getTotalPageCount()
{
    return ui->label_totalpage->text().toUInt();
}

void ExplorerBlock::setCurPageNo(unsigned int nPage)
{
    ui->label_curpage->setText(QString::number(nPage));
}

void ExplorerBlock::setTotalPageNo(unsigned int nPage)
{
    ui->label_totalpage->setText(QString::number(nPage));
}

void ExplorerBlock::on_pushButton_prepage_clicked()
{
    int nPage = getCurPageNo()-1;
    if(nPage >=1 && nPage <= getTotalPageCount())
    {
        TxEntriesShowPage(nPage);
    }
}

void ExplorerBlock::on_pushButton_nextpage_clicked()
{
    int nPage = getCurPageNo()+ 1;
    if(nPage >=1 && nPage <= getTotalPageCount())
    {
        TxEntriesShowPage(nPage);
    }
}

void ExplorerBlock::on_pushButton_goto_clicked()
{
    int nPage = ui->lineEdit_gotopage->text().toInt();
    if(nPage >=1 && nPage <= getTotalPageCount())
    {
        TxEntriesShowPage(nPage);
    }
}
