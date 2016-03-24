#include "explorerhome.h"
#include "ui_explorerhome.h"
#include <QTime>

#include "main.h"
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;


ExplorerHome::ExplorerHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerHome)
{
    ui->setupUi(this);
    ui->tableWidget_chain->setHorizontalHeaderLabels(QStringList() <<tr("Height")<<tr("Age")<<tr("Type")<<tr("Transactions")<<tr("Size(Byte)")<<tr("Difficulty")<<tr("Coins Out"));
    //ui->tableWidget_chain->setColumnWidth(4,150);
    ui->tableWidget_tx->setHorizontalHeaderLabels(QStringList() <<tr("Tx ID")<<tr("Age")<<tr("Coins Out"));
    ui->tableWidget_tx->setColumnWidth(0,500);

    for(int i=0;i<MAX_ROW_CHAINTABLE;i++)
    {
        height[i] = new QTableWidgetItem("0");
       // height[i]->setFont(new QFont);
       // height[i]->setBackgroundColor(QColor::fromRgb(173,216,230));
        height[i]->setTextAlignment(Qt::AlignCenter);
        height[i]->setForeground(QBrush(Qt::blue));
        ui->tableWidget_chain->setItem(i,0,height[i]);

        ageBlk[i] = new QTableWidgetItem("NULL");
        ageBlk[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_chain->setItem(i,1,ageBlk[i]);

        type[i] = new QTableWidgetItem();
        type[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_chain->setItem(i,2,type[i]);

        txs[i] = new QTableWidgetItem();
        txs[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_chain->setItem(i,3,txs[i]);

        size[i] = new QTableWidgetItem();
        size[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_chain->setItem(i,4,size[i]);

        difficulty[i] = new QTableWidgetItem();
        difficulty[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_chain->setItem(i,5,difficulty[i]);

        coinsoutBlk[i] = new QTableWidgetItem();
        coinsoutBlk[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_chain->setItem(i,6,coinsoutBlk[i]);

        timeBlk[i] = 0;
      //  ui->label_home->setVisible(false);
    }

    for(int i = 0 ; i < MAX_ROW_TXTABLE ; i++)
    {
        txid[i] = new QTableWidgetItem();
        txid[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_tx->setItem(i,0,txid[i]);
        txid[i]->setForeground(QBrush(Qt::blue));

        ageTx[i] = new QTableWidgetItem();
        ageTx[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_tx->setItem(i,1,ageTx[i]);

        coinsoutTx[i] = new QTableWidgetItem();
        coinsoutTx[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_tx->setItem(i,2,coinsoutTx[i]);

        timeTx[i] = 0;
    }
  //  connect(height[i], SIGNAL(clicked()), this,SLOT(serchBlock_HomePage(QString)));
}

ExplorerHome::~ExplorerHome()
{
    delete ui;
}

long ExplorerHome::getBestHeight()
{
    getRowHeight(0);
}


void ExplorerHome::rpcNewBlock(QString msg)
{
    long heightNew = getRpcBlockHeight(msg);
    rpcAssigRow(msg,pindexBest->nHeight - heightNew);
    //queryMempoolTXs();

   /* long heightNew = getRpcBlockHeight(msg);
    long heightCur = getRowHeight(0);

    if(heightNew > heightCur)
    {
        long h[MAX_ROW_CHAINTABLE];
        for(int k=0;k<MAX_ROW_CHAINTABLE;k++)
        {
            h[k] = 0;
        }
        //row=0 assig new block
        for(int i = MAX_ROW_CHAINTABLE-1 ; i>0 ; i--)
        {
            long height_i = heightNew - i;
            int j=0;
            for(j = i-1 ; j >= 0 ; j--)
            {
                if(getRowHeight(j) == height_i)
                {
                    chainTable_assigRow(j,i);
                    break;
                }
            }
            //not exsist,rpc request for this block
            if(j < 0)
            {
                clearRow(i);
                h[i] = height_i;
            }
        }
        rpcAssigRow(msg,0);

        for(int k = 0 ; k < MAX_ROW_CHAINTABLE ; k++)
        {
            if(h[k] != 0)
            {
                Sleep(100);
                emit rpcGetBlock(QString::number(h[k]));
            }
        }


    }
    else if(heightNew == heightCur)
    {
    }
    else
    {
        rpcAssigRow(msg,heightCur - heightNew);
    }*/
}


void ExplorerHome::chainTable_assigRow(int from,int to)
{
    if(!(from>=0 && from < MAX_ROW_CHAINTABLE && to>=0 && to < MAX_ROW_CHAINTABLE && from != to))
        return;
    height[to]->setText(height[from]->text());
    timeBlk[to] = timeBlk[from];

    /*long current_time = time(NULL);
    long minu = (current_time - timeBlk[to]) / 60;
    QString blkage;
    if(minu < 1)
        blkage = QString("< 1 min");
    else
        blkage = QString::number(minu) + QString(" min");
    ageBlk[to]->setText(blkage);
*/
    ageBlk[to]->setText(calculateAge(timeBlk[to]));
    type[to]->setText(type[from]->text());
    txs[to]->setText(txs[from]->text());
    size[to]->setText(size[from]->text());
    difficulty[to]->setText(difficulty[from]->text());
    coinsoutBlk[to]->setText(coinsoutBlk[from]->text());
}

long ExplorerHome::getRowHeight(int row)
{
    if(row <0 || row >= MAX_ROW_CHAINTABLE)
        return 0;
    return height[row]->text().toLong();
}

void ExplorerHome::rpcAssigRow(QString msg,unsigned int row)
{
    if(row <0 || row >= MAX_ROW_CHAINTABLE)
        return;

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
        strValue = pt.get<std::string>("height");
        height[row]->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("time");
        long block_time = QString::fromStdString(strValue).toLong();
        long current_time = time(NULL);
        timeBlk[row] = current_time;

        /*long minu = (current_time - block_time) / 60;
        QString blkage;
        if(minu < 1)
            blkage = QString("< 1 min");
        else
            blkage = QString::number(minu) + QString(" min");
        ageBlk[row]->setText(blkage);
        */
        ageBlk[row]->setText(calculateAge(block_time));

        strValue = pt.get<std::string>("flags");
        int pos = strValue.find("of-work",0);  //-1 when not found
        if(pos > 0)
            type[row]->setText("POW");
        else
            type[row]->setText("POS");
        strValue = pt.get<std::string>("transactions");
        txs[row]->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("size");
        size[row]->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("difficulty");
        difficulty[row]->setText(QString::fromStdString(strValue));

        strValue = pt.get<std::string>("coinsout");
        coinsoutBlk[row]->setText(QString::fromStdString(strValue) + " MTC");
    }
    catch (ptree_error & e)
    {
      return;
    }

    //update the latest txs
    if(row == 0)
    {
        updateLatestTxsAge();
        queryMempoolTXs();
    }
}

long ExplorerHome::getRpcBlockHeight(QString msg)
{
    std::string strPrint = msg.toStdString();
    std::stringstream ss(strPrint);
    std::string strValue;
    ptree pt;

    try{
      read_json(ss, pt);
    }
    catch(ptree_error & e) {
      return 0;
    }

    try{
        unsigned int row = 0;
        strValue = pt.get<std::string>("height");
        return QString::fromStdString(strValue).toLong();
    }
    catch (ptree_error & e)
    {
      return 0;
    }
}

void ExplorerHome::clearRow(unsigned int row)
{
    if(row <0 || row >= MAX_ROW_CHAINTABLE)
        return;

    height[row]->setText("");
    timeBlk[row] = 0;
    ageBlk[row]->setText("");

    type[row]->setText("");
    txs[row]->setText("");
    size[row]->setText("");
    difficulty[row]->setText("");
    coinsoutBlk[row]->setText("");
}


void ExplorerHome::on_tableWidget_chain_clicked(const QModelIndex &index)
{
    int col = index.column();
    int row  = index.row();

    if(col == 0)
    {
        emit rpcGetBlock(height[row]->text());
    }
}

static long nLastQueryTime = 0;
void ExplorerHome::queryMempoolTXs()
{
    long curTime = time(NULL);
    if( (curTime - nLastQueryTime) < 30 )
    {
        return;
    }
    nLastQueryTime = curTime;

    std::vector<uint256> vtxid;
    std::vector<long> vtxTime;
    std::vector<double> vValue;
    mempool.queryTxsMsg(vtxid,vtxTime,vValue);

    std::vector<long>::iterator itTime = vtxTime.begin();
    std::vector<double>::iterator itValue = vValue.begin();

    //the size of this 3 vector must be same
    BOOST_FOREACH(const uint256& hash, vtxid)
    {
        QString txhash = QString::fromStdString(hash.ToString());
        long txtime = *itTime;
        double fValue = *itValue;
        QString coinsout = QString::number(fValue,'f',8) + QString(" MTC");
        insertLatestTx(txhash,txtime,coinsout);
        itTime++;
        itValue++;
    }
}

void ExplorerHome::insertLatestTx(QString hash,long txtime,QString coinsout)
{

    for(int i=MAX_ROW_TXTABLE-1 ; i >= 0 ; i--)
    {
        //update age
        if(txid[i]->text() == hash)
        {
            return;
        }
    }

    for(int i=MAX_ROW_TXTABLE-1 ; i > 0 ; i--)
    {
        txid[i]->setText(txid[i-1]->text());
        ageTx[i]->setText(ageTx[i-1]->text());
        coinsoutTx[i]->setText(coinsoutTx[i-1]->text());
        timeTx[i] = timeTx[i-1];
    }
    txid[0]->setText(hash);
    ageTx[0]->setText(calculateAge(txtime));
    coinsoutTx[0]->setText(coinsout);
    timeTx[0] = txtime;
}

QString ExplorerHome::calculateAge(long t)
{
    long current_time = time(NULL);
    long minu = (current_time - t) / 60;
    QString age;
    if(minu < 1)
        age = QString("< 1 min");
    else if(minu > 1440)
        age = QString("> 1 day");
    else
    {
        int h = minu / 60;
        int m = minu % 60;
        if(h > 0)
            age = QString::number(h) + QString(" h ");
        age += QString::number(m) + QString(" min");
    }
    return age;
}

void ExplorerHome::updateLatestTxsAge()
{
    for(int i = 0; i< MAX_ROW_TXTABLE;i++)
    {
        if(ageTx[i]->text().isEmpty())
            continue;
        ageTx[i]->setText(calculateAge(timeTx[i]));
    }
}

void ExplorerHome::on_tableWidget_tx_clicked(const QModelIndex &index)
{
    int col = index.column();
    int row  = index.row();

    if(col == 0)
    {
        if(txid[row]->text().size() == 64)
            emit rpcGetTx(txid[row]->text());
    }
}
