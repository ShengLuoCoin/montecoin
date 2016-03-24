#include "chainexplorerpage.h"
#include "ui_chainexplorerpage.h"

#include "explorerhome.h"
#include "exploreraddress.h"
#include "explorerblock.h"
#include "explorertransaction.h"

#include "walletmodel.h"
#include <QThread>
#include "main.h"
#include "clientmodel.h"
#include  "rpcconsole.h"

#include <QDesktopServices>
#include <QUrl>


bool IsDigitString(QString strSource)
{
    bool bDigit = false;

    if (strSource.isEmpty())
    {
        return bDigit;
    }

    QByteArray strByteArray = strSource.toLatin1();
    const char *chString = strByteArray.data();

    while (*chString && *chString>='0' && *chString<='9') chString++;

    if (*chString)
    {
        bDigit = false;
    }
    else
    {
        bDigit = true;
    }

    return bDigit;
}

ChainExplorerPage::ChainExplorerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChainExplorerPage)
{
    ui->setupUi(this);
    ui->lineEdit_search->setPlaceholderText(tr("Hash, Address, Block Height"));
    connect(ui->lineEdit_search, SIGNAL(textChanged(QString)), this, SLOT(changedExplorer(QString)));

    expHome = new ExplorerHome;
    expAddress = new ExplorerAddress;
    expBlock = new ExplorerBlock;
    expTx = new ExplorerTransaction;

    ui->stackedWidget->addWidget(expHome);
    ui->stackedWidget->addWidget(expAddress);
    ui->stackedWidget->addWidget(expBlock);
    ui->stackedWidget->addWidget(expTx);

    curRPCCmd = RPC_NULL;

    heightFlag = 0;
    fAutoUpdateHomePage = false;

    connect(this, SIGNAL(rpcSetBlockPage(QString)), expBlock, SLOT(rpcFoundBlock(QString)));
    connect(this, SIGNAL(rpcSetTxPage(QString)), expTx, SLOT(rpcFoundTx(QString)));
    connect(expBlock, SIGNAL(searchBlockByHash(QString)), this, SLOT(serchBlock_BlockPage(QString)));
    connect(expTx, SIGNAL(searchBlockByHash(QString)), this, SLOT(serchBlock_BlockPage(QString)));
    connect(this, SIGNAL(rpcSetHomePage(QString)), expHome, SLOT(rpcNewBlock(QString)));
    connect(expHome, SIGNAL(rpcGetBlock(QString)), this,SLOT(serchBlock_HomePage(QString)));
    connect(expHome, SIGNAL(rpcGetTx(QString)), this,SLOT(serchTx_HomePage(QString)));

    startExecutor();
    gotoHomePage();
}

ChainExplorerPage::~ChainExplorerPage()
{
    emit stopExecutor();
    delete ui;
}

void ChainExplorerPage::changedExplorer(QString str)
{
    QString editSearch = ui->lineEdit_search->text();
    if(model->validateAddress(editSearch))
    {
        ui->pushButton_search->setEnabled(true);
        ui->pushButton_search->setStyleSheet("border-image:url(:icons/exp_search)");
        return ;
    }
    else if(IsDigitString(editSearch))
    {
        long blkheight = editSearch.toLong();
        if(blkheight <= pindexBest->nHeight)
        {
            ui->pushButton_search->setEnabled(true);
            ui->pushButton_search->setStyleSheet("border-image:url(:icons/exp_search)");
            return ;
        }
    }
    else if(editSearch.size() == 64) //hash,block or tx
    {
        ui->pushButton_search->setEnabled(true);
        ui->pushButton_search->setStyleSheet("border-image:url(:icons/exp_search)");
        return ;
    }
    ui->pushButton_search->setEnabled(false);
    ui->pushButton_search->setStyleSheet("border-image:null");
}

void ChainExplorerPage::gotoHomePage()
{
     fAutoUpdateHomePage = true;
     ui->stackedWidget->setCurrentWidget(expHome);
}

void ChainExplorerPage::gotoBlockPage()
{
     fAutoUpdateHomePage = false;
     ui->stackedWidget->setCurrentWidget(expBlock);
}

void ChainExplorerPage::gotoAddressPage()
{

}

void ChainExplorerPage::gotoTxPage()
{
     fAutoUpdateHomePage = false;
     ui->stackedWidget->setCurrentWidget(expTx);
}

void ChainExplorerPage::on_pushButton_search_clicked()
{
    QString editSearch = ui->lineEdit_search->text();
    if(model->validateAddress(editSearch))
    {
        gotoAddressPage();
    }
    else if(IsDigitString(editSearch))
    {
        //marktodo:if height larger than long...
        long blkheight = editSearch.toLong();
        QString cmd = QString("getblockhash ") + QString::number(blkheight);
        curRPCCmd = RPC_GETBLOCKHASH_BLOCK;
        emit cmdRequest(cmd);

       // gotoBlockPage();
    }
    else if(editSearch.size() == 64)
    {

        QString cmdTx = QString("gettransaction ") + editSearch;
        curRPCCmd = RPC_GETTRANSACTION;
        emit cmdRequest(cmdTx);
    }
    else
    {
        gotoHomePage();
    }
}

void ChainExplorerPage::stopUpdateHomePage(bool fUpdate)
{
    fAutoUpdateHomePage = fUpdate;
    if(fAutoUpdateHomePage && (ui->stackedWidget->currentWidget() == expHome)) //homepage
    {
        on_pushButton_clicked();
    }
}


static long nLastUpdateBlockTime = 0;
void ChainExplorerPage::updateHomePage(int count, int countOfPeers)
{
    if(fAutoUpdateHomePage)
    {
        long curTime = time(NULL);
        if( (curTime - nLastUpdateBlockTime) < 10 )
        {
            return;
        }
        nLastUpdateBlockTime = curTime;
        on_pushButton_clicked();
    }
}

void ChainExplorerPage::on_pushButton_clicked()
{
    if(pindexBest->nHeight > expHome->getBestHeight())
    {
        heightFlag = pindexBest->nHeight;
        QString cmd = QString("getblockhash ") + QString::number(heightFlag--);
        curRPCCmd = RPC_GETBLOCKHASH_HOME;
        emit cmdRequest(cmd);
    }

    gotoHomePage();
}

void ChainExplorerPage::setModel(WalletModel *model)
{
    this->model = model;

    //marktodo,signals
}

void ChainExplorerPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        connect(model, SIGNAL(numBlocksChanged(int,int)), this, SLOT(updateHomePage(int,int)));
    }
}

void ChainExplorerPage::startExecutor()
{
    QThread* thread = new QThread;
    RPCExecutor *executor = new RPCExecutor();
    executor->moveToThread(thread);

    // Notify executor when thread started (in executor thread)
    connect(thread, SIGNAL(started()), executor, SLOT(start()));
    // Replies from executor object must go to this object
    connect(executor, SIGNAL(reply(int,QString)), this, SLOT(message(int,QString)));
    // Requests from this object must go to executor
    connect(this, SIGNAL(cmdRequest(QString)), executor, SLOT(request(QString)));
    // On stopExecutor signal
    // - queue executor for deletion (in execution thread)
    // - quit the Qt event loop in the execution thread
    connect(this, SIGNAL(stopExecutor()), executor, SLOT(deleteLater()));
    connect(this, SIGNAL(stopExecutor()), thread, SLOT(quit()));
    // Queue the thread for deletion (in this thread) when it is finished
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // Default implementation of QThread::run() simply spins up an event loop in the thread,
    // which is what we want.
    thread->start();
}


void ChainExplorerPage::message(int category, const QString &message, bool html)
{
    if(category != RPCConsole::CMD_REPLY)
    {
        if(curRPCCmd == RPC_GETTRANSACTION)
        {
            QString editSearch = ui->lineEdit_search->text();
            QString cmdBlo = QString("getblock ") + editSearch + QString(" true");
            curRPCCmd = RPC_GETBLOCK_BLOCK;
            emit cmdRequest(cmdBlo);
        }
        else
        {
            ui->lineEdit_search->clear();
            gotoHomePage();
        }
        return ;
    }

    switch (curRPCCmd) {
    case RPC_GETBLOCKHASH_HOME:
    {
        QString cmd = QString("getblock ") + message;
        curRPCCmd = RPC_GETBLOCK_HOME;
        emit cmdRequest(cmd);

        break;
    }
    case RPC_GETBLOCK_HOME:
    {
        gotoHomePage();
        emit rpcSetHomePage(message);
        if( (pindexBest->nHeight - heightFlag ) < 7)
        {
            QString cmd = QString("getblockhash ") + QString::number(heightFlag--);
            curRPCCmd = RPC_GETBLOCKHASH_HOME;
            emit cmdRequest(cmd);
        }
        break;
    }
    case RPC_GETBLOCKHASH_BLOCK:
    {
        QString cmd = QString("getblock ") + message + QString(" true");
        curRPCCmd = RPC_GETBLOCK_BLOCK;
        emit cmdRequest(cmd);

        break;
    }
    case RPC_GETBLOCK_BLOCK:
    {
        gotoBlockPage();
        emit rpcSetBlockPage(message);
        break;
    }
    case RPC_GETTRANSACTION:
    {
        gotoTxPage();
        emit rpcSetTxPage(message);
        break;
    }
    default:
        break;
    }
}

void ChainExplorerPage::serchBlock_BlockPage(QString blockHash)
{
    ui->lineEdit_search->setText(blockHash);
    QString cmdBlo = QString("getblock ") + blockHash + QString(" true");
    curRPCCmd = RPC_GETBLOCK_BLOCK;
    emit cmdRequest(cmdBlo);
}

void ChainExplorerPage::serchBlock_HomePage(QString height)
{
    ui->lineEdit_search->setText(height);
    QString cmdBlo = QString("getblockhash ") + height;
    curRPCCmd = RPC_GETBLOCKHASH_BLOCK;
    emit cmdRequest(cmdBlo);
}

void ChainExplorerPage::serchTx_HomePage(QString txid)
{
    ui->lineEdit_search->setText(txid);
    QString cmd = QString("gettransaction ") + txid;
    curRPCCmd = RPC_GETTRANSACTION;
    emit cmdRequest(cmd);
}
