#ifndef EXPLORERADDRESS_H
#define EXPLORERADDRESS_H

#include <QWidget>

namespace Ui {
class ExplorerAddress;
}

class ExplorerAddress : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerAddress(QWidget *parent = 0);
    ~ExplorerAddress();

private:
    Ui::ExplorerAddress *ui;
};

#endif // EXPLORERADDRESS_H
