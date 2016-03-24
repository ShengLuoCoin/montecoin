#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class WelcomePage;
}


class MyLabel: public QLabel
{
    Q_OBJECT

public:
    MyLabel(QWidget * parent = 0);

private:

protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev);

signals:
    void clicked(void);
};

class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = 0);
    ~WelcomePage();

private:
    Ui::WelcomePage *ui;
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QImage imageBackground;
    MyLabel *labelCoin,*labelRia,*labelGame,*labelLive;
    MyLabel *labelWallet,*labelBlockchain,*labelHelp,*labelConsole,*labelSetting;

private slots:
    void onClickedLabelCoin();
    void onClickedLabelRia();
    void onClickedLabelGame();
    void onClickedLabelLive();

    void onClickedLabelWallet();
    void onClickedLabelChain();
    void onClickedLabelHelp();
    void onClickedLabelConsole();
    void onClickedLabelSetting();

signals:
    //panelID = mozzria:0 montecoin:1  mozzgame:2 mozzlive:3
    void GoToPanelClicked(int panelID);
};


#endif // WELCOMEPAGE_H
