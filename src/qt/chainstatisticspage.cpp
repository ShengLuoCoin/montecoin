#include "chainstatisticspage.h"
#include "ui_chainstatisticspage.h"

#include "clientmodel.h"
#include "bitcoinrpc.h"
#include "guiutil.h"

#include <QTime>
#include <QTimer>
#include <QThread>
#include <QTextEdit>
#include <QKeyEvent>
#include <QUrl>
#include <QScrollBar>

#include <openssl/crypto.h>

ChainStatisticsPage::ChainStatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChainStatisticsPage)
{
    ui->setupUi(this);

    // set OpenSSL version label
    ui->openSSLVersion->setText(SSLeay_version(SSLEAY_VERSION));
}

ChainStatisticsPage::~ChainStatisticsPage()
{
    delete ui;
}


void ChainStatisticsPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        // Subscribe to information, replies, messages, errors
        connect(model, SIGNAL(numConnectionsChanged(int)), this, SLOT(setNumConnections(int)));
        connect(model, SIGNAL(numBlocksChanged(int,int)), this, SLOT(setNumBlocks(int,int)));
        connect(model, SIGNAL(statisticsChanged(QString,QString,QString,QString,QString,QString,QString)), this, SLOT(statisticsChanged(QString,QString,QString,QString,QString,QString,QString)));

        // Provide initial values
        ui->clientVersion->setText(model->formatFullVersion());
        ui->clientName->setText(model->clientName());
        ui->buildDate->setText(model->formatBuildDate());
        ui->startupTime->setText(model->formatClientStartupTime());

        setNumConnections(model->getNumConnections());
        ui->isTestNet->setChecked(model->isTestNet());

        setNumBlocks(model->getNumBlocks(), model->getNumBlocksOfPeers());
    }
}

void ChainStatisticsPage::statisticsChanged(QString strSupply,QString strHashrate,QString strDiffPOW,
                       QString strDiffPOS,QString strHeightPOW,QString strRewardPOW,QString strReward123)
{
    ui->moneySupply->setText(strSupply);
    ui->nethashrate->setText(strHashrate);
    ui->difficulty_pow->setText(strDiffPOW);
    ui->difficulty_pos->setText(strDiffPOS);
    ui->numberOfBlocks_pow->setText(strHeightPOW);
    //ui->numberOfBlocks_pos->setText(); //marktodo
    ui->blockreawrd_pow->setText(strRewardPOW);
}


void ChainStatisticsPage::setNumConnections(int count)
{
    ui->numberOfConnections->setText(QString::number(count));
}

void ChainStatisticsPage::setNumBlocks(int count, int countOfPeers)
{
    ui->numberOfBlocks->setText(QString::number(count));
    ui->totalBlocks->setText(QString::number(countOfPeers));
    if(clientModel)
    {
        // If there is no current number available display N/A instead of 0, which can't ever be true
        ui->totalBlocks->setText(clientModel->getNumBlocksOfPeers() == 0 ? tr("N/A") : QString::number(clientModel->getNumBlocksOfPeers()));
        ui->lastBlockTime->setText(clientModel->getLastBlockDate().toString());
    }
}
