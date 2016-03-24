#ifndef MINERKINGPAGE_H
#define MINERKINGPAGE_H

#include <QWidget>

namespace Ui {
class MinerkingPage;
}

class MinerkingPage : public QWidget
{
    Q_OBJECT

public:
    explicit MinerkingPage(QWidget *parent = 0);
    ~MinerkingPage();

private:
    Ui::MinerkingPage *ui;
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QImage imageBackground;

private slots:
    void openUrl(QString url);
    void on_click_button_deposit();

signals:
    void deposit_minerking();
};

#endif // MINERKINGPAGE_H
