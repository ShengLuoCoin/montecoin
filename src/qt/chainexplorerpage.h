#ifndef CHAINEXPLORERPAGE_H
#define CHAINEXPLORERPAGE_H

#include <QWidget>
#include <QVector>

class ExplorerHome;
class ExplorerAddress;
class ExplorerBlock;
class ExplorerTransaction;
class ClientModel;

namespace Ui {
class ChainExplorerPage;
}

class WalletModel;

class ChainExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChainExplorerPage(QWidget *parent = 0);
    ~ChainExplorerPage();

    void setModel(WalletModel *model);

    enum RPCCommand {
        RPC_NULL,
        RPC_GETBLOCK_BLOCK,
        RPC_GETBLOCKHASH_BLOCK,
        RPC_GETBLOCK_HOME,
        RPC_GETBLOCKHASH_HOME,
        RPC_GETTRANSACTION,
        RPC_GETADDRESS
    };

    void setClientModel(ClientModel *model);

private:
    Ui::ChainExplorerPage *ui;
    ExplorerHome *expHome;
    ExplorerAddress *expAddress;
    ExplorerBlock *expBlock;
    ExplorerTransaction *expTx;

    WalletModel *model;

    RPCCommand curRPCCmd;

    int heightFlag;

    ClientModel *clientModel;
    bool fAutoUpdateHomePage;

    void startExecutor();

public slots:
    void changedExplorer(QString str);

    void gotoHomePage();
    void gotoTxPage();
    void gotoBlockPage();
    void gotoAddressPage();
    void stopUpdateHomePage(bool fUpdate);

    void message(int category, const QString &message, bool html = false);
private slots:
    void on_pushButton_search_clicked();
    void on_pushButton_clicked();

    void serchBlock_BlockPage(QString blockHash);
    void serchBlock_HomePage(QString height);
    void serchTx_HomePage(QString txid);

    void updateHomePage(int count, int countOfPeers);

signals:
    void rpcSetBlockPage(QString msg);
    void rpcSetTxPage(QString msg);
    void rpcSetHomePage(QString msg);

    // For RPC command executor
    void stopExecutor();
    void cmdRequest(const QString &command);
};

#endif // CHAINEXPLORERPAGE_H
