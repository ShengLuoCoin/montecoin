#include "welcomepage.h"
#include "ui_welcomepage.h"
#include <QtGui>

MyLabel::MyLabel(QWidget * parent)
    : QLabel(parent)
{

}

void MyLabel::mouseReleaseEvent(QMouseEvent * ev)
{
    Q_UNUSED(ev)
    emit clicked();
}

WelcomePage::WelcomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePage)
{
    ui->setupUi(this);

    imageBackground.load(":/images/homepage_bg");
    setAutoFillBackground(true);

    labelCoin = new MyLabel;
    labelRia = new MyLabel;
    labelGame = new MyLabel;
    labelLive = new MyLabel;

    labelWallet = new MyLabel;
    labelBlockchain = new MyLabel;
    labelHelp = new MyLabel;
    labelConsole = new MyLabel;
    labelSetting = new MyLabel;

    labelCoin->setStyleSheet("image:url(:icons/wel_montecoin)");
    labelRia->setStyleSheet("image:url(:icons/wel_mozzria)");
    labelGame->setStyleSheet("image:url(:icons/wel_mozzgame)");
    labelLive->setStyleSheet("image:url(:icons/wel_mozzlive)");

    labelWallet->setStyleSheet("image:url(:icons/wel_wallet)");
    labelBlockchain->setStyleSheet("image:url(:icons/wel_blockchain)");
    labelHelp->setStyleSheet("image:url(:icons/wel_help)");
    labelConsole->setStyleSheet("image:url(:icons/wel_console)");
    labelSetting->setStyleSheet("image:url(:icons/wel_setting)");

    //ui->verticalLayout_coin->addWidget(labelCoin);
    ui->verticalLayout_ria->addWidget(labelRia);
    ui->verticalLayout_game->addWidget(labelGame);
    ui->verticalLayout_live->addWidget(labelLive);

    ui->verticalLayout_wallet->addWidget(labelWallet);
    ui->verticalLayout_chain->addWidget(labelBlockchain);
    ui->verticalLayout_help->addWidget(labelHelp);
    ui->verticalLayout_console->addWidget(labelConsole);
    ui->verticalLayout_setting->addWidget(labelSetting);

    connect(labelCoin, SIGNAL(clicked()), this, SLOT(onClickedLabelCoin()));
    connect(labelRia, SIGNAL(clicked()), this, SLOT(onClickedLabelRia()));
    connect(labelGame, SIGNAL(clicked()), this, SLOT(onClickedLabelGame()));
    connect(labelLive, SIGNAL(clicked()), this, SLOT(onClickedLabelLive()));

    connect(labelWallet, SIGNAL(clicked()), this, SLOT(onClickedLabelWallet()));
    connect(labelBlockchain, SIGNAL(clicked()), this, SLOT(onClickedLabelChain()));
    connect(labelHelp, SIGNAL(clicked()), this, SLOT(onClickedLabelHelp()));
    connect(labelConsole, SIGNAL(clicked()), this, SLOT(onClickedLabelConsole()));
    connect(labelSetting, SIGNAL(clicked()), this, SLOT(onClickedLabelSetting()));
}

WelcomePage::~WelcomePage()
{
    delete ui;
}

void WelcomePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPalette pal(palette());   // 注意下面这一行，就是靠他实现大小拉伸
    pal.setBrush(QPalette::Window,
                 QBrush(imageBackground.scaled(event->size(),
                                      Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation))
                 );
    setPalette(pal);
}

//panelID = wallet:0 chain:1  console:2 setting:3
//panelID = game:4   live:5   mozzria:6 help:7    montecoin:8
void WelcomePage::onClickedLabelWallet()
{
        emit GoToPanelClicked(0);
}

void WelcomePage::onClickedLabelChain()
{
    emit GoToPanelClicked(1);
}

void WelcomePage::onClickedLabelConsole()
{
    emit GoToPanelClicked(2);
}

void WelcomePage::onClickedLabelSetting()
{
    emit GoToPanelClicked(3);
}

void WelcomePage::onClickedLabelGame()
{
    emit GoToPanelClicked(4);
}

void WelcomePage::onClickedLabelLive()
{
    emit GoToPanelClicked(5);
}

void WelcomePage::onClickedLabelRia()
{
    emit GoToPanelClicked(6);
}

void WelcomePage::onClickedLabelHelp()
{
    emit GoToPanelClicked(7);
}

void WelcomePage::onClickedLabelCoin()
{
    emit GoToPanelClicked(8);
}
