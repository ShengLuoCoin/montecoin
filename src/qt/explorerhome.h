#ifndef EXPLORERHOME_H
#define EXPLORERHOME_H

#include <QWidget>
#include <QDateTime>
#include <QTableWidgetItem>

namespace Ui {
class ExplorerHome;
}

static const unsigned int MAX_ROW_CHAINTABLE = 6;
static const unsigned int MAX_ROW_TXTABLE = 4;

class ExplorerHome : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerHome(QWidget *parent = 0);
    ~ExplorerHome();

    long getBestHeight();

public slots:
    void rpcNewBlock(QString msg);

private:
    Ui::ExplorerHome *ui;

    //blockchain tablewidget
    QTableWidgetItem *height[MAX_ROW_CHAINTABLE];
    QTableWidgetItem *ageBlk[MAX_ROW_CHAINTABLE];
    QTableWidgetItem *type[MAX_ROW_CHAINTABLE];
    QTableWidgetItem *txs[MAX_ROW_CHAINTABLE];
    QTableWidgetItem *size[MAX_ROW_CHAINTABLE];
    QTableWidgetItem *difficulty[MAX_ROW_CHAINTABLE];
    QTableWidgetItem *coinsoutBlk[MAX_ROW_CHAINTABLE];
    long timeBlk[MAX_ROW_CHAINTABLE];

    //tx tablewidget
    QTableWidgetItem *txid[MAX_ROW_TXTABLE];
    QTableWidgetItem *ageTx[MAX_ROW_TXTABLE];
    QTableWidgetItem *coinsoutTx[MAX_ROW_TXTABLE];
    void insertLatestTx(QString hash,long txtime,QString coinsout);
    void updateLatestTxsAge();
    void queryMempoolTXs();
    long timeTx[MAX_ROW_TXTABLE];


    void chainTable_assigRow(int from,int to);
    long getRowHeight(int row);
    void rpcAssigRow(QString msg,unsigned int row);
    long getRpcBlockHeight(QString msg);
    void clearRow(unsigned int row);
    QString calculateAge(long t);

signals:
    void rpcGetBlock(QString height);
    void rpcGetTx(QString txid);
private slots:
    void on_tableWidget_chain_clicked(const QModelIndex &index);
    void on_tableWidget_tx_clicked(const QModelIndex &index);
};

#endif // EXPLORERHOME_H
