#ifndef EXPLORERTRANSACTION_H
#define EXPLORERTRANSACTION_H

#include <QWidget>

namespace Ui {
class ExplorerTransaction;
}

class ExplorerTransaction : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerTransaction(QWidget *parent = 0);
    ~ExplorerTransaction();

public slots:
    void rpcFoundTx(QString msg);

private slots:
    void on_pb_includeInBlock_clicked();

private:
    Ui::ExplorerTransaction *ui;

    void clear();

signals:
    void searchBlockByHash(QString blockHash);
};

#endif // EXPLORERTRANSACTION_H
