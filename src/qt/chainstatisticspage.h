#ifndef CHAINSTATISTICSPAGE_H
#define CHAINSTATISTICSPAGE_H

#include <QWidget>

namespace Ui {
class ChainStatisticsPage;
}

class ClientModel;
class ChainStatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChainStatisticsPage(QWidget *parent = 0);
    ~ChainStatisticsPage();

    void setClientModel(ClientModel *model);

protected:

private slots:
    /** Set number of connections shown in the UI */
    void setNumConnections(int count);
    /** Set number of blocks shown in the UI */
    void setNumBlocks(int count, int countOfPeers);
    void statisticsChanged(QString strSupply,QString strHashrate,QString strDiffPOW,
                           QString strDiffPOS,QString strHeightPOW,QString strRewardPOW,QString strReward123);


private:
    Ui::ChainStatisticsPage *ui;
    ClientModel *clientModel;
};

#endif // CHAINSTATISTICSPAGE_H
