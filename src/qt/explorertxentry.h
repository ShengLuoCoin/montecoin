#ifndef EXPLORERTXENTRY_H
#define EXPLORERTXENTRY_H

#include <QFrame>

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;

namespace Ui {
class ExplorerTxEntry;
}

class ExplorerTxEntry : public QFrame
{
    Q_OBJECT

public:
    explicit ExplorerTxEntry(QWidget *parent = 0);
    explicit ExplorerTxEntry(QString msg, QWidget *parent = 0);
    explicit ExplorerTxEntry(ptree pt, QWidget *parent = 0);
    ~ExplorerTxEntry();

private:
    Ui::ExplorerTxEntry *ui;
};

#endif // EXPLORERTXENTRY_H
