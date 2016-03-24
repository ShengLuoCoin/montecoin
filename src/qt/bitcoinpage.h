#ifndef ALTCOINPAGE_H
#define ALTCOINPAGE_H

#include <QWidget>

namespace Ui {
class BitcoinPage;
}

class BitcoinPage : public QWidget
{
    Q_OBJECT

public:
    explicit BitcoinPage(QWidget *parent = 0);
    ~BitcoinPage();

private slots:
    void on_label_4_linkActivated(const QString &link);
    void openUrl(QString url);

private:
    Ui::BitcoinPage *ui;
};

#endif // ALTCOINPAGE_H
