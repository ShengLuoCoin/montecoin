#ifndef EXPLORERBLOCK_H
#define EXPLORERBLOCK_H

#include <QWidget>

namespace Ui {
class ExplorerBlock;
}

class ExplorerBlock : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerBlock(QWidget *parent = 0);
    ~ExplorerBlock();

public slots:
    void rpcFoundBlock(QString msg);

private slots:
    void rpc_getblock(long height);

    void on_pb_preblock_clicked();

    void on_pb_nextBlock_clicked();

    void on_pushButton_prepage_clicked();

    void on_pushButton_nextpage_clicked();

    void on_pushButton_goto_clicked();

private:
    Ui::ExplorerBlock *ui;

    void clearTxEntries();

    QString msgBlock;

    unsigned int getCurPageNo();
    void setCurPageNo(unsigned int nPage);
    unsigned int getTotalPageCount();
    void setTotalPageNo(unsigned int nPage);
    void TxEntriesShowPage(unsigned int nPage);

signals:
    void searchBlockByHash(QString blockHash);
};

#endif // EXPLORERBLOCK_H
