#include "explorertxentry.h"
#include "ui_explorertxentry.h"

#include <QStringListModel>
#include <QStringList>

ExplorerTxEntry::ExplorerTxEntry(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ExplorerTxEntry)
{
    ui->setupUi(this);
}

ExplorerTxEntry::ExplorerTxEntry(QString msg,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ExplorerTxEntry)
{
    ui->setupUi(this);

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
      ui->lineEdit_blockhash->setText(QString::fromStdString(strValue));
    }
    catch (ptree_error & e)
    {
      return;
    }
}

ExplorerTxEntry::ExplorerTxEntry(ptree pt,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ExplorerTxEntry)
{
    ui->setupUi(this);

    try{
      ptree ptVin,p2,pinAddr,p4;
      std::string strValue;

      strValue = pt.get<std::string>("txid");
      ui->lineEdit_blockhash->setText(QString::fromStdString(strValue));

      //vin
      ptVin = pt.get_child("vin");
      QStringListModel* slm = new QStringListModel(this);
      QStringList* sl = new QStringList();
      for (ptree::iterator it = ptVin.begin(); it != ptVin.end(); ++it)
      {
          std::string inValue,inAddress;
          p2 = it->second; //first为空
          std::stringstream s;
          write_json(s, ptVin);
          std::string image_item = s.str();
          int pos = image_item.find("coinbase",0);

          if(pos >0)//pos ==25
          {
              strValue = p2.get<std::string>("coinbase");
              strValue = "coinbase:   " + strValue;
          }
          else
          {
              inValue = p2.get<std::string>("prevalue");
              pinAddr = p2.get_child("preaddress");
              for (ptree::iterator it = pinAddr.begin(); it != pinAddr.end(); ++it)
              {
                   p4 = it->second; //first为空
                   inAddress += p4.get<std::string>("") + "  ";
              }
              strValue = inAddress + inValue + " MTC"; //marktodo:rpc get pre address
          }

          sl->append(strValue.c_str());
      }

      //marktest
     /* std::stringstream s;
      write_json(s, pt);
      std::string image_item = s.str();
      sl->append(image_item.c_str());*/

      slm->setStringList(*sl);
      ui->listView_inouts->setModel(slm);
      delete sl;

      //vout
      ptree ptVout,p3,pScriptPubkey,pAddr;
      ptVout = pt.get_child("vout");
      QStringListModel* slm2 = new QStringListModel(this);
      QStringList* sl2 = new QStringList();

      for (ptree::iterator it = ptVout.begin(); it != ptVout.end(); ++it)
      {
          std::string voutValue,type,address;
          p3 = it->second; //first为空
          voutValue = p3.get<std::string>("value");
          pScriptPubkey = p3.get_child("scriptPubKey");
          type = pScriptPubkey.get<std::string>("type");
          if(type == "nonstandard")
          {
              strValue = "NULL   " +voutValue + " MTC";
          }
          else
          {
              pAddr = pScriptPubkey.get_child("addresses");
              for (ptree::iterator it = pAddr.begin(); it != pAddr.end(); ++it)
              {
                   p4 = it->second; //first为空
                   address += p4.get<std::string>("") + "  ";
              }
              strValue = address + voutValue + " MTC";
          }
          sl2->append(strValue.c_str());
      }
      slm2->setStringList(*sl2);
      ui->listView_outputs->setModel(slm2);
      delete sl2;

    }
    catch (ptree_error & e)
    {
      return;
    }
}


ExplorerTxEntry::~ExplorerTxEntry()
{
    delete ui;
}
