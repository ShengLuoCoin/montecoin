/*
 * Qt4 bitcoin GUI.
 *
 * W.J. van der Laan 2011-2012
 * The Bitcoin Developers 2011-2012
 */
#include "bitcoingui.h"
#include "transactiontablemodel.h"
#include "addressbookpage.h"
#include "sendcoinsdialog.h"
#include "signverifymessagedialog.h"
#include "optionsdialog.h"
#include "aboutdialog.h"
#include "clientmodel.h"
#include "walletmodel.h"
#include "editaddressdialog.h"
#include "optionsmodel.h"
#include "transactiondescdialog.h"
#include "addresstablemodel.h"
#include "transactionview.h"
#include "overviewpage.h"
#include "bitcoinunits.h"
#include "guiconstants.h"
#include "askpassphrasedialog.h"
#include "notificator.h"
#include "guiutil.h"
#include "rpcconsole.h"

//montecoin:gui
#include "welcomepage.h"
#include"bitcoinpage.h"
#include"chainstatisticspage.h"
#include"chainexplorerpage.h"
#include"consolepage.h"
#include"minerkingpage.h"
#include"lotterypage.h"
#include"mozzdicepage.h"
#include"mozzriaaboutpage.h"
#include"mozzriacontactpage.h"
#include"settingpage.h"
#include"helppage.h"
#include "main.h"
#include "init.h"


#ifdef Q_OS_MAC
#include "macdockiconhandler.h"
#endif

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLocale>
#include <QMessageBox>
#include <QProgressBar>
#include <QStackedWidget>
#include <QDateTime>
#include <QMovie>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTimer>
#include <QDragEnterEvent>
#include <QUrl>
#include <QStyle>
#include <QListWidget>
#include <QPushButton>
#include <QFrame>

#include <iostream>

extern bool fWalletUnlockMintOnly;

BitcoinGUI::BitcoinGUI(QWidget *parent):
    QMainWindow(parent),
    clientModel(0),
    walletModel(0),
    encryptWalletAction(0),
    changePassphraseAction(0),
    snapshotWalletAction(0),
    lockMintingAction(0),
    aboutQtAction(0),
    trayIcon(0),
    notificator(0),
    rpcConsole(0)
{
    setFixedSize(850, 550);
   // resize(850, 550);
    //resize(1024, 660);
   // resize(1226, 750);
   // setFixedSize(1024,598);
   // setFixedSize(1142,668);

    setWindowTitle(tr("Montecoin") + " - " + tr("Wallet"));
  /*  qApp->setStyleSheet("QMainWindow { background-image:url(:images/background);border:none;font-family:'Open Sans,sans-serif'; }\
                        QToolBar { border:none; background-color: rgb(0,0,0); } \
                        QStatusBar { border:none; background-color: rgb(192,192,192); color:rgb(255,255,255) } \
                        QMenuBar { background: rgb(0,0,0); color:rgb(57,253,255);} \
                        QMenuBar::item { font-size:12px;padding-bottom:8px;padding-top:8px;padding-left:15px;padding-right:15px;color:rgb(57,253,255); background-color: transparent; } \
                        QMenuBar::item:selected { background-color:qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5,stop: 0 rgb(60,60,60), stop: 1 rgb(155,155,155)); } \
                        QMenu { background: black; color: rgb(57,253,255); } \
                        QMenu::item { color:rgb(57,253,255); background-color: transparent; } \
                        QMenu::item:selected { background-color:qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5,stop: 0 rgb(60,60,60), stop: 1 rgb(155,155,155)); }");

*/
#ifndef Q_OS_MAC
    qApp->setWindowIcon(QIcon(":icons/bitcoin"));
    setWindowIcon(QIcon(":icons/bitcoin"));
#else
    setUnifiedTitleAndToolBarOnMac(true);
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    // Accept D&D of URIs
    setAcceptDrops(true);

    // Create actions for the toolbar, menu bar and tray/dock icon
    createActions();

    // Create application menu bar
    createMenuBar();

    // Create the toolbars
    //createToolBars();

    // Create the tray icon (or setup the dock icon)
    createTrayIcon();

    // Create tabs
    overviewPage = new OverviewPage();

    transactionsPage = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout();
    transactionView = new TransactionView(this);
    vbox->addWidget(transactionView);
    transactionsPage->setLayout(vbox);

    addressBookPage = new AddressBookPage(AddressBookPage::ForEditing, AddressBookPage::SendingTab);

    receiveCoinsPage = new AddressBookPage(AddressBookPage::ForEditing, AddressBookPage::ReceivingTab);

    sendCoinsPage = new SendCoinsDialog(this);

    signVerifyMessageDialog = new SignVerifyMessageDialog(this);

    centralStackedWidget = new QStackedWidget(this);
    centralStackedWidget->addWidget(overviewPage);
    centralStackedWidget->addWidget(transactionsPage);
    centralStackedWidget->addWidget(addressBookPage);
    centralStackedWidget->addWidget(receiveCoinsPage);
    centralStackedWidget->addWidget(sendCoinsPage);
   // setCentralWidget(centralStackedWidget); //marktest

    //montecoin:gui
    welcomePage = new WelcomePage;
    centralStackedWidget->addWidget(welcomePage);

    bitcoinPage = new BitcoinPage;
    centralStackedWidget->addWidget(bitcoinPage);

    chainStatisticsPage = new ChainStatisticsPage;
    centralStackedWidget->addWidget(chainStatisticsPage);

    chainExplorerPage = new ChainExplorerPage;
    centralStackedWidget->addWidget(chainExplorerPage);

    consolePage = new ConsolePage;
    centralStackedWidget->addWidget(consolePage);

    minerkingPage = new MinerkingPage;
    centralStackedWidget->addWidget(minerkingPage);

    lotteryPage = new LotteryPage;
    centralStackedWidget->addWidget(lotteryPage);

    mozzdicePage = new MozzdicePage;
    centralStackedWidget->addWidget(mozzdicePage);

    mozzriaAboutPage = new MozzriaAboutPage;
    centralStackedWidget->addWidget(mozzriaAboutPage);

    mozzriaContactPage = new MozzriaContactPage;
    centralStackedWidget->addWidget(mozzriaContactPage);

    settingPage = new SettingPage;
    centralStackedWidget->addWidget(settingPage);

    helpPage = new HelpPage;
    centralStackedWidget->addWidget(helpPage);

    /****************************************************************/
    //montecoin:gui

    //create QListWidget and QStackedWidget items
    createListToolbarStaked();

    //LeftBottom Layout
    msgWidget = new QFrame;
    lb00 = new QLabel(QWidget::tr("Supply:"));
    supplyLabel = new QLabel(QWidget::tr(""));
    QLabel *lb10 = new QLabel(QWidget::tr("Nodes:"));
    connectionLabel = new QLabel(QWidget::tr(""));
    QLabel *lb20 = new QLabel(QWidget::tr("Height:"));
    heightLabel = new QLabel(QWidget::tr(""));
    QLabel *lb30 = new QLabel(QWidget::tr("Hash:"));
    nethashLabel = new QLabel(QWidget::tr(""));
    QLabel *lb40 = new QLabel(QWidget::tr("Version:"));
    versionLabel = new QLabel(QWidget::tr(""));

    lb00->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    lb10->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    lb20->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    lb30->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    lb40->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    supplyLabel->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    connectionLabel->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    heightLabel->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    nethashLabel->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");
    versionLabel->setStyleSheet("color: rgb(255, 0, 0);font: 75 12pt \"宋体\"");

    QGridLayout *msgGridLayout = new QGridLayout;
    msgGridLayout->addWidget(lb00, 0, 0);
    msgGridLayout->addWidget(supplyLabel, 0, 1);
    msgGridLayout->addWidget(lb10, 1, 0);
    msgGridLayout->addWidget(connectionLabel, 1, 1);
    msgGridLayout->addWidget(lb20, 2, 0);
    msgGridLayout->addWidget(heightLabel, 2, 1);
    msgGridLayout->addWidget(lb30, 3, 0);
    msgGridLayout->addWidget(nethashLabel, 3, 1);
    msgGridLayout->addWidget(lb40, 4, 0);
    msgGridLayout->addWidget(versionLabel, 4, 1);
    msgWidget->setLayout(msgGridLayout);
    msgWidget->setAutoFillBackground(true);
    msgWidget->setStyleSheet("background-image:url(:images/msg_bg)");
    msgWidget->setFixedHeight(115);

    //
    topWidget = new QWidget;
    topWidget->setAutoFillBackground(true);
    QHBoxLayout *topHLayout = new QHBoxLayout(this);
    QSpacerItem* hSpacerLeft = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topHLayout->addSpacerItem(hSpacerLeft);
    topHLayout->addWidget(msgWidget);
    topWidget->setLayout(topHLayout);
   // topWidget->setFixedHeight(179); //
  //  topWidget->setFixedHeight(115);  //

    //leftWidget Layout
    leftTopLabel = new QLabel();
    leftTopLabel->setFixedHeight(28);
    //leftTopLabel->setText("Home Page");

   // leftWidget = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setMargin(0);
    leftLayout->setSpacing(5);
    leftLayout->addWidget(leftTopLabel);

    btWelcome = new QPushButton();
    btWelcome->setText(tr("Home Page"));
    btWelcome->setFixedHeight(35);
    //leftLayout->addWidget(btWelcome);

   // leftLayout->addWidget(listWidget);

    //leftLayout->addWidget(toolbarsStack);

   // leftLayout->addWidget(msgWidget);
  //  leftWidget->setLayout(leftLayout);

  //  leftWidget->setStyleSheet("background-color:rgb(74,86,100);");

    //right-top Layout
    middleBarWidget = new QWidget;
    //middleBarWidget->setStyleSheet("background-color:rgb(255,255,255);"); //guitodo
    tbMintLock =  new QToolBar;
    tbMintLock->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbMintLock->setStyleSheet("background-color:rgb(189,252,201);color:rgb(63,139,147);font: 20 12pt \"黑体\"; text-align: left;");
   // tbMintLock->setFixedWidth(190);
    tbMintLock->addAction(lockMintingAction);

    tbMiningLock =  new QToolBar;
    tbMiningLock->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbMiningLock->setStyleSheet("background-color:rgb(0,0,0);color:rgb(63,139,147);font: 20 12pt \"黑体\"; text-align: left;");
   // tbMiningLock->setFixedWidth(190);
    tbMiningLock->addAction(genAction);

    balanceLabel = new QLabel(QWidget::tr(""));
    balanceLabel->setStyleSheet("font-size:20px; color:rgb(220,0,0);");
    middleBarWidget->setFixedHeight(35);

    tbHomePage =  new QToolBar;
    tbHomePage->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbHomePage->addAction(homepageAction);
    tbHomePage->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\"; border:none;height:100%;text-align: left;min-width:150px;max-width:150px;");

    QHBoxLayout *hMiddleBarLAyout=new QHBoxLayout(this);
    hMiddleBarLAyout->setMargin(0);
    hMiddleBarLAyout->setSpacing(3);
   // hMiddleBarLAyout->addWidget(btWelcome);
   // hMiddleBarLAyout->addWidget(tbHomePage);
    hMiddleBarLAyout->addWidget(toolbarsStack);
  //  hMiddleBarLAyout->addWidget(balanceLabel);
  //  hMiddleBarLAyout->addWidget(tbMiningLock);
   // hMiddleBarLAyout->addWidget(tbMintLock);
    //hMiddleBarLAyout->setStretchFactor(toolbarsStack,4);
    //hMiddleBarLAyout->setStretchFactor(tbMintLock,1);
    middleBarWidget->setLayout(hMiddleBarLAyout);



    //rightWidget Layout
    rightWidget = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setMargin(0);
    rightLayout->setSpacing(0);
   // rightLayout->addWidget(topWidget);
    rightLayout->addWidget(middleBarWidget);
    rightLayout->addWidget(centralStackedWidget);
    rightWidget->setLayout(rightLayout);
  //  toolbarsStack->setFixedHeight(35);

    //centerWindow Layout
    QHBoxLayout *centerLayout=new QHBoxLayout(this);
    centerLayout->setMargin(0);
    centerLayout->setSpacing(0);
   // centerLayout->addWidget(leftWidget);
    centerLayout->addWidget(rightWidget);
    //centerLayout->setStretchFactor(leftWidget,1);
    //centerLayout->setStretchFactor(rightWidget,5);
   // leftWidget->setFixedWidth(190);

    //setCentralWidget
    centerWindow = new QWidget;
    centerWindow->setLayout(centerLayout);
    setCentralWidget(centerWindow);

    // setCentralWidget(rightWidget);

    //新界面设计，不需要显示和使用这两个控件了
    //listWidget->setEnabled(false);
    //listWidget->setVisible(false);
   // connect(listWidget,SIGNAL(currentRowChanged(int)),toolbarsStack,SLOT(setCurrentIndex(int)));
   // connect(listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(ListWidgetItemSelectionChanged()));



    /****************************************************************/

    // Create status bar
    statusBar();

    // Status bar notification icons
    QFrame *frameBlocks = new QFrame();
    frameBlocks->setContentsMargins(0,0,0,0);
    frameBlocks->setFixedWidth(60);
    QHBoxLayout *frameBlocksLayout = new QHBoxLayout(frameBlocks);
    frameBlocksLayout->setContentsMargins(3,0,3,0);
    frameBlocksLayout->setSpacing(3);
    labelEncryptionIcon = new QLabel();
    labelConnectionsIcon = new QLabel();

    labelBlocksIcon = new QLabel();
    frameBlocksLayout->addStretch();
    frameBlocksLayout->addWidget(labelEncryptionIcon);
    frameBlocksLayout->addStretch();
    frameBlocksLayout->addWidget(labelConnectionsIcon);
    frameBlocksLayout->addStretch();
    frameBlocksLayout->addWidget(labelBlocksIcon);
    frameBlocksLayout->addStretch();

    // Progress bar and label for blocks download
    progressBarLabel = new QLabel();
    progressBarLabel->setVisible(false);
    progressBar = new QProgressBar();
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setVisible(false);

    // Override style sheet for progress bar for styles that have a segmented progress bar,
    // as they make the text unreadable (workaround for issue #1071)
    // See https://qt-project.org/doc/qt-4.8/gallery.html
    QString curStyle = qApp->style()->metaObject()->className();
    if(curStyle == "QWindowsStyle" || curStyle == "QWindowsXPStyle")
    {
        progressBar->setStyleSheet("QProgressBar { background-color: #e8e8e8; border: 1px solid grey; border-radius: 7px; padding: 1px; text-align: center; } QProgressBar::chunk { background: QLinearGradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #FF8000, stop: 1 orange); border-radius: 7px; margin: 0px; }");
    }

    QLabel  *developerLabel = new QLabel(QWidget::tr("Version")+ QString(": ")+QString::fromStdString(FormatFullVersion()));

    developerLabel->setMinimumWidth(400);
    statusBar()->addWidget(progressBarLabel);
    statusBar()->addWidget(progressBar);
   // statusBar()->addPermanentWidget(developerLabel);
    statusBar()->addPermanentWidget(frameBlocks);

    syncIconMovie = new QMovie(":/movies/update_spinner", "mng", this);
	// this->setStyleSheet("background-color: #ceffee;");

    // Clicking on a transaction on the overview page simply sends you to transaction history page
    connect(overviewPage, SIGNAL(transactionClicked(QModelIndex)), this, SLOT(gotoHistoryPage()));
    connect(overviewPage, SIGNAL(transactionClicked(QModelIndex)), transactionView, SLOT(focusTransaction(QModelIndex)));

    connect(sendCoinsPage, SIGNAL(sendCoinsClicked(QString)), overviewPage, SLOT(setButtonText(QString)));

    // Double-clicking on a transaction on the transaction history page shows details
    connect(transactionView, SIGNAL(doubleClicked(QModelIndex)), transactionView, SLOT(showDetails()));

    rpcConsole = new RPCConsole(this);
    connect(openRPCConsoleAction, SIGNAL(triggered()), rpcConsole, SLOT(show()));

    // Clicking on "Verify Message" in the address book sends you to the verify message tab
    connect(addressBookPage, SIGNAL(verifyMessage(QString)), this, SLOT(gotoVerifyMessageTab(QString)));
    // Clicking on "Sign Message" in the receive coins page sends you to the sign message tab
    connect(receiveCoinsPage, SIGNAL(signMessage(QString)), this, SLOT(gotoSignMessageTab(QString)));

   // connect(welcomePage->, SIGNAL(clicked())), this, SLOT(gotoMinerkingPage());
    connect(minerkingPage, SIGNAL(deposit_minerking()), this, SLOT(minerKingDeposit()));
    connect(welcomePage, SIGNAL(GoToPanelClicked(int)), this, SLOT(OnWelcomePageClicked(int)));

    connect(this, SIGNAL(autoUpdateHomePage(bool)), chainExplorerPage, SLOT(stopUpdateHomePage(bool)));

    connect(btWelcome, SIGNAL(clicked()), this, SLOT(gotoWelcomePage()));
    //gotoOverviewPage();
   // gotoWelcomePage();
  //  listWidget->setCurrentItem(listItemWelcome);
    OnWelcomePageClicked(0);
}
void BitcoinGUI::createListToolbarStaked()
{
    //Welcome ToolBar
    tbWelcome =  new QToolBar;
    tbWelcome->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //tbWelcome->addAction(homepageAction);
    tbWelcome->addAction(chainMozzAction);
    //tbWelcome->setStyleSheet("color: rgb(63, 139, 147)");
    //tbWelcome->setStyleSheet("background-color:rgb(150,150,150);color:red");

    //Wallet ToolBar
    tbWallet =  new QToolBar;
    tbWallet->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbWallet->addAction(overviewAction);
    tbWallet->addAction(sendCoinsAction);
    tbWallet->addAction(receiveCoinsAction);
    tbWallet->addAction(historyAction);
    tbWallet->addAction(addressBookAction);
    tbWallet->addAction(exportAction);

    //Blockchain ToolBar
    tbBlockchain =  new QToolBar;
    tbBlockchain->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbBlockchain->addAction(chainExplorerAction);
   // tbBlockchain->addAction(chainMozzAction);
    tbBlockchain->addAction(chainStaAction);

    //Console ToolBar
    tbConsole =  new QToolBar;
    tbConsole->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbConsole->addAction(consoleAction);

    //Mozzgame ToolBar
    tbMozzgame =  new QToolBar;
    tbMozzgame->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbMozzgame->addAction(gameMinerkingAction);

    //Mozzlive ToolBar
    tbMozzlive =  new QToolBar;
    tbMozzlive->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbMozzlive->addAction(liveLotteryAction);
    tbMozzlive->addAction(liveGamblingAction);

    //Mozzria ToolBar
    tbMozzria =  new QToolBar;
    tbMozzria->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbMozzria->addAction(riaAboutAction);
    //tbMozzria->addAction(riaContactAction);

    //Mozzgame ToolBar
    tbSetting =  new QToolBar;
    tbSetting->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbSetting->addAction(settingAction);

    //Mozzgame ToolBar
    tbHelp =  new QToolBar;
    tbHelp->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbHelp->addAction(helpAction);

    //create listWedgetItem
    listItemWelcome = new QListWidgetItem(QIcon(":/icons/list_welcome"),QObject::tr("Welcome"));
    listItemWallet = new QListWidgetItem(QIcon(":/icons/list_wallet"), QObject::tr("Wallet"));
    listItemBlockchain = new QListWidgetItem(QIcon(":/icons/list_blockchain"), QObject::tr("Block"));
    listItemConsole = new QListWidgetItem(QIcon(":/icons/list_console"), QObject::tr("Console"));
    listItemMozzgame = new QListWidgetItem(QIcon(":/icons/list_mozzgame"), QObject::tr("Mozzgame"));
    listItemMozzlive = new QListWidgetItem(QIcon(":/icons/list_mozzlive"), QObject::tr("Mozzlive"));
    listItemMozzria = new QListWidgetItem(QIcon(":/icons/list_mozzria"), QObject::tr("Mozzria"));
    listItemSetting = new QListWidgetItem(QIcon(":/icons/list_setting"), QObject::tr("Setting"));
    listItemHelp = new QListWidgetItem(QIcon(":/icons/list_help"), QObject::tr("Help"));

    //create ListWidget Items
   /* listWidget=new QListWidget(this);
    listWidget->insertItem(0,listItemWelcome);
    listWidget->insertItem(1,listItemWallet);
    listWidget->insertItem(2,listItemBlockchain);
    listWidget->insertItem(3,listItemConsole);
   // listWidget->insertItem(4,listItemMozzgame);
   // listWidget->insertItem(5,listItemMozzlive);
   // listWidget->insertItem(6,listItemMozzria);
    listWidget->insertItem(7,listItemSetting);
    listWidget->insertItem(8,listItemHelp);
    listWidget->setSpacing(5);
    listWidget->setStyleSheet("font-size:17px;");*/

   /* tbWelcome->setOrientation(Qt::Vertical);
    tbWallet->setOrientation(Qt::Vertical);
    tbBlockchain->setOrientation(Qt::Vertical);
    tbConsole->setOrientation(Qt::Vertical);
    tbMozzgame->setOrientation(Qt::Vertical);
    tbMozzlive->setOrientation(Qt::Vertical);
    tbMozzria->setOrientation(Qt::Vertical);
    tbSetting->setOrientation(Qt::Vertical);
    tbHelp->setOrientation(Qt::Vertical);*/

    tbBlockchain->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\";height:100%;text-align: left;min-width:100px;");
    tbWallet->setStyleSheet("color:rgb(255,128,0);font: 20 12pt \"黑体\";;height:100%;text-align: left;min-width:100px;"); //guitodo
    tbConsole->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\";height:100%;text-align: left;min-width:100px;");
    tbSetting->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\";;height:100%;text-align: left;min-width:100px;");
    tbMozzgame->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\";height:100%;text-align: left;min-width:100px;");
    tbMozzlive->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\";;height:100%;text-align: left;min-width:100px;");
    tbMozzria->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\";height:100%;text-align: left;min-width:100px;");
    tbHelp->setStyleSheet("color:rgb(63,139,147);font: 20 12pt \"黑体\";;height:100%;text-align: left;min-width:100px;");


    //
    toolbarsStack=new QStackedWidget(this);
    toolbarsStack->addWidget(tbWallet);
    toolbarsStack->addWidget(tbBlockchain);
    toolbarsStack->addWidget(tbConsole);
    toolbarsStack->addWidget(tbSetting);
    toolbarsStack->addWidget(tbMozzgame);
    toolbarsStack->addWidget(tbMozzlive);
    toolbarsStack->addWidget(tbMozzria);
    toolbarsStack->addWidget(tbHelp);
    toolbarsStack->addWidget(tbWelcome);
}

BitcoinGUI::~BitcoinGUI()
{
    if(trayIcon) // Hide tray icon, as deleting will let it linger until quit (on Ubuntu)
        trayIcon->hide();
#ifdef Q_OS_MAC
    delete appMenuBar;
#endif
}

void BitcoinGUI::createActions()
{
    QActionGroup *tabGroup = new QActionGroup(this);

    overviewAction = new QAction(QIcon(":/icons/overview"), tr("&Overview"), this);
    overviewAction->setToolTip(tr("Show general overview of wallet"));
    overviewAction->setCheckable(true);
    overviewAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(overviewAction);

    sendCoinsAction = new QAction(QIcon(":/icons/send"), tr("&Send coins"), this);
    sendCoinsAction->setToolTip(tr("Send coins to a Montecoin address"));
    sendCoinsAction->setCheckable(true);
    sendCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
    tabGroup->addAction(sendCoinsAction);

    receiveCoinsAction = new QAction(QIcon(":/icons/receiving_addresses"), tr("&Receive coins"), this);
    receiveCoinsAction->setToolTip(tr("Show the list of addresses for receiving payments"));
    receiveCoinsAction->setCheckable(true);
    receiveCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_3));
    tabGroup->addAction(receiveCoinsAction);

    historyAction = new QAction(QIcon(":/icons/history"), tr("&Transactions"), this);
    historyAction->setToolTip(tr("Browse transaction history"));
    historyAction->setCheckable(true);
    historyAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_4));
    tabGroup->addAction(historyAction);

    addressBookAction = new QAction(QIcon(":/icons/address-book"), tr("&Address Book"), this);
    addressBookAction->setToolTip(tr("Edit the list of stored addresses and labels"));
    addressBookAction->setCheckable(true);
    addressBookAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_5));
    tabGroup->addAction(addressBookAction);

    connect(overviewAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(overviewAction, SIGNAL(triggered()), this, SLOT(gotoOverviewPage()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(gotoSendCoinsPage()));
    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(gotoReceiveCoinsPage()));
    connect(historyAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(historyAction, SIGNAL(triggered()), this, SLOT(gotoHistoryPage()));
    connect(addressBookAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(addressBookAction, SIGNAL(triggered()), this, SLOT(gotoAddressBookPage()));

    quitAction = new QAction(QIcon(":/icons/quit"), tr("E&xit"), this);
    quitAction->setToolTip(tr("Quit application"));
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAction->setMenuRole(QAction::QuitRole);
    aboutAction = new QAction(QIcon(":/icons/bitcoin"), tr("&About Montecoin"), this);
    aboutAction->setToolTip(tr("Show information about Montecoin"));
    aboutAction->setMenuRole(QAction::AboutRole);
    aboutQtAction = new QAction(QIcon(":/trolltech/qmessagebox/images/qtlogo-64.png"), tr("About &Qt"), this);
    aboutQtAction->setToolTip(tr("Show information about Qt"));
    aboutQtAction->setMenuRole(QAction::AboutQtRole);
    optionsAction = new QAction(QIcon(":/icons/options"), tr("&Options..."), this);
    optionsAction->setToolTip(tr("Modify configuration options for Montecoin"));
    optionsAction->setMenuRole(QAction::PreferencesRole);
    toggleHideAction = new QAction(QIcon(":/icons/bitcoin"), tr("&Show / Hide"), this);
    encryptWalletAction = new QAction(QIcon(":/icons/lock_closed"), tr("&Encrypt Wallet..."), this);
    encryptWalletAction->setToolTip(tr("Encrypt or decrypt wallet"));
    encryptWalletAction->setCheckable(true);
    backupWalletAction = new QAction(QIcon(":/icons/filesave"), tr("&Backup Wallet..."), this);
    backupWalletAction->setToolTip(tr("Backup wallet to another location"));
    changePassphraseAction = new QAction(QIcon(":/icons/key"), tr("&Change Passphrase..."), this);
    changePassphraseAction->setToolTip(tr("Change the passphrase used for wallet encryption"));
    signMessageAction = new QAction(QIcon(":/icons/edit"), tr("Sign &message..."), this);
    verifyMessageAction = new QAction(QIcon(":/icons/verification"), tr("&Verify message..."), this);

    snapshotWalletAction = new QAction(QIcon(":/icons/snapshot"), tr("&Import keys from WTOC wallet"), this);
    snapshotWalletAction->setToolTip(tr("WTOC transfer into MTC via snapshot"));
    snapshotWalletAction->setCheckable(true);

    lockMintingAction = new QAction(QIcon(":/icons/minting_closed"), tr("&Unlock Wallet For Minting Only"), this);
    lockMintingAction->setToolTip(tr("Unlock Wallet For Minting Only"));
    lockMintingAction->setCheckable(true);

    genAction = new QAction(QIcon(":/icons/mining_inactive"), tr("&Generate"), this);
    genAction->setStatusTip(tr("Start or Stop Generate Coins"));
    genAction->setToolTip(genAction->statusTip());
    genAction->setCheckable(true);

    exportAction = new QAction(QIcon(":/icons/export"), tr("&Export..."), this);
    exportAction->setToolTip(tr("Export the data in the current tab to a file"));
    openRPCConsoleAction = new QAction(QIcon(":/icons/debugwindow"), tr("&Debug window"), this);
    openRPCConsoleAction->setToolTip(tr("Open debugging and diagnostic console"));

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutClicked()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(optionsClicked()));
    connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
    connect(encryptWalletAction, SIGNAL(triggered(bool)), this, SLOT(encryptWallet(bool)));
    connect(snapshotWalletAction, SIGNAL(triggered(bool)), this, SLOT(wtocSnapshot(bool)));
    connect(lockMintingAction, SIGNAL(triggered(bool)), this, SLOT(lockWalletForMinting(bool)));
    connect(backupWalletAction, SIGNAL(triggered()), this, SLOT(backupWallet()));
    connect(changePassphraseAction, SIGNAL(triggered()), this, SLOT(changePassphrase()));
    connect(signMessageAction, SIGNAL(triggered()), this, SLOT(gotoSignMessageTab()));
    connect(verifyMessageAction, SIGNAL(triggered()), this, SLOT(gotoVerifyMessageTab()));
    connect(genAction, SIGNAL(triggered()), this, SLOT(StartOrStopGenerate()));

    //marktodo: settooltip icon
    //montecoin:gui
    homepageAction = new QAction(QIcon(":/icons/homepage"), tr("&Home Page"), this);
   // homepageAction->setToolTip(tr("Show general overview of wallet"));
    homepageAction->setCheckable(true);
    //homepageAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));

    tabGroup->addAction(homepageAction);

    chainStaAction = new QAction(QIcon(":/icons/tb_statistics"), tr("&Statistics"), this);
  //  chainStaAction->setToolTip(tr("Show general overview of wallet"));
    chainStaAction->setCheckable(true);
    //homepageAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(chainStaAction);

    chainMozzAction = new QAction(QIcon(":/icons/tb_montecoin"), tr("&Montecoin"), this);
  //  chainMozzAction->setToolTip(tr("Show general overview of wallet"));
    chainMozzAction->setCheckable(true);
    //chainMozzAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(chainMozzAction);

    chainExplorerAction = new QAction(QIcon(":/icons/tb_explorer"), tr("&Explorer"), this);
  //  chainExplorerAction->setToolTip(tr("Show general overview of wallet"));
    chainExplorerAction->setCheckable(true);
    //homepageAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(chainExplorerAction);

    consoleAction = new QAction(QIcon(":/icons/tb_console"), tr("&Console"), this);
  //  consoleAction->setToolTip(tr("Show general overview of wallet"));
    consoleAction->setCheckable(true);
    //consoleAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(consoleAction);

    gameMinerkingAction = new QAction(QIcon(":/icons/tb_minerking"), tr("&MinerKing"), this);
  //  gameMinerkingAction->setToolTip(tr("Show general overview of wallet"));
    gameMinerkingAction->setCheckable(true);
    //gameMinerkingAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(gameMinerkingAction);

    liveLotteryAction = new QAction(QIcon(":/icons/tb_lottery"), tr("&Mozz-Lottery"), this);
  //  liveLotteryAction->setToolTip(tr("Show general overview of wallet"));
    liveLotteryAction->setCheckable(true);
    //liveLotteryAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(liveLotteryAction);

    liveGamblingAction = new QAction(QIcon(":/icons/tb_gambling"), tr("&Mozz-Dice"), this);
  //  liveGamblingAction->setToolTip(tr("Show general overview of wallet"));
    liveGamblingAction->setCheckable(true);
    //gameMinerkingAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(liveGamblingAction);

    riaAboutAction = new QAction(QIcon(":/icons/tb_about"), tr("&About Us"), this);
 //   riaAboutAction->setToolTip(tr("Show general overview of wallet"));
    riaAboutAction->setCheckable(true);
    //liveLotteryAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(riaAboutAction);

    riaContactAction = new QAction(QIcon(":/icons/tb_contact"), tr("&Contact"), this);
  //  riaContactAction->setToolTip(tr("Show general overview of wallet"));
    riaContactAction->setCheckable(true);
    //gameMinerkingAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(riaContactAction);

    settingAction = new QAction(QIcon(":/icons/tb_setting"), tr("&Setting"), this);
 //   settingAction->setToolTip(tr("Show general overview of wallet"));
    settingAction->setCheckable(true);
    //settingAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(settingAction);

    helpAction = new QAction(QIcon(":/icons/tb_help"), tr("&Help"), this);
 //   helpAction->setToolTip(tr("Show general overview of wallet"));
    helpAction->setCheckable(true);
    //helpAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(helpAction);

    //montecoin:gui
    connect(homepageAction, SIGNAL(triggered()), this, SLOT(gotoWelcomePage()));
    connect(chainMozzAction, SIGNAL(triggered()), this, SLOT(gotoChainMozzPage()));
    connect(chainStaAction, SIGNAL(triggered()), this, SLOT(gotoChainStatisticsPage()));
    connect(chainExplorerAction, SIGNAL(triggered()), this, SLOT(gotoChainExplorerPage()));
    connect(consoleAction, SIGNAL(triggered()), this, SLOT(gotoConsolePage()));
    connect(gameMinerkingAction, SIGNAL(triggered()), this, SLOT(gotoMinerkingPage()));
    connect(liveLotteryAction, SIGNAL(triggered()), this, SLOT(gotoLotteryPage()));
    connect(liveGamblingAction, SIGNAL(triggered()), this, SLOT(gotoMozzdicePage()));
    connect(riaAboutAction, SIGNAL(triggered()), this, SLOT(gotoMozzriaAboutPage()));
    connect(riaContactAction, SIGNAL(triggered()), this, SLOT(gotoMozzriaContactPage()));
    connect(settingAction, SIGNAL(triggered()), this, SLOT(gotoSettingPage()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(gotoHelpPage()));

}

void BitcoinGUI::createMenuBar()
{
#ifdef Q_OS_MAC
    // Create a decoupled menu bar on Mac which stays even if the window is closed
    appMenuBar = new QMenuBar();
#else
    // Get the main window's menu bar on other platforms
    appMenuBar = menuBar();
#endif

    // Configure the menus
    QMenu *file = appMenuBar->addMenu(tr("&File"));
    file->addAction(backupWalletAction);
    file->addAction(exportAction);
    file->addAction(signMessageAction);
    file->addAction(verifyMessageAction);
   // file->addSeparator();
    file->addAction(quitAction);

    QMenu *settings = appMenuBar->addMenu(tr("&Setting"));
    settings->addAction(encryptWalletAction);
    settings->addAction(changePassphraseAction);
    settings->addAction(genAction);
    settings->addAction(lockMintingAction);
    //settings->addAction(snapshotWalletAction);
   // settings->addSeparator();
    settings->addAction(optionsAction);

    QMenu *help = appMenuBar->addMenu(tr("&About"));
    help->addAction(openRPCConsoleAction);
   // help->addSeparator();
    help->addAction(aboutAction);
    help->addAction(aboutQtAction);

    //QString ss("QMenuBar{ background: black; color: black }");
   // appMenuBar->setStyleSheet(ss);

    //upgradetodo

/*
    appMenuBar->addSeparator();
    appMenuBar->addAction(overviewAction);
    appMenuBar->addSeparator();
    appMenuBar->addAction(chainExplorerAction);
*/

   // appMenuBar->addSeparator();
    //appMenuBar->addAction(helpAction);
}

void BitcoinGUI::createToolBars()
{
    QToolBar *toolbar = addToolBar(tr("Tabs toolbar"));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addAction(overviewAction);
    toolbar->addAction(sendCoinsAction);
    toolbar->addAction(receiveCoinsAction);
    toolbar->addAction(historyAction);
    toolbar->addAction(addressBookAction);

    QToolBar *toolbar2 = addToolBar(tr("Actions toolbar"));
    toolbar2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar2->addAction(exportAction);
}

void BitcoinGUI::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;
    if(clientModel)
    {
        // Replace some strings and icons, when using the testnet
        if(clientModel->isTestNet())
        {
            setWindowTitle(windowTitle() + QString(" ") + tr("[testnet]"));
#ifndef Q_OS_MAC
            qApp->setWindowIcon(QIcon(":icons/bitcoin_testnet"));
            setWindowIcon(QIcon(":icons/bitcoin_testnet"));
#else
            MacDockIconHandler::instance()->setIcon(QIcon(":icons/bitcoin_testnet"));
#endif
            if(trayIcon)
            {
                trayIcon->setToolTip(tr("Montecoin client") + QString(" ") + tr("[testnet]"));
                trayIcon->setIcon(QIcon(":/icons/toolbar_testnet"));
                toggleHideAction->setIcon(QIcon(":/icons/toolbar_testnet"));
            }

            aboutAction->setIcon(QIcon(":/icons/toolbar_testnet"));
        }

        // Keep up to date with client
        setNumConnections(clientModel->getNumConnections());
        connect(clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(setNumConnections(int)));

        setNumBlocks(clientModel->getNumBlocks(), clientModel->getNumBlocksOfPeers());
        connect(clientModel, SIGNAL(numBlocksChanged(int,int)), this, SLOT(setNumBlocks(int,int)));

        // Report errors from network/worker thread
        connect(clientModel, SIGNAL(error(QString,QString,bool)), this, SLOT(error(QString,QString,bool)));

        connect(clientModel, SIGNAL(mainPanelParaChanged(QString,QString,QString,QString)), this, SLOT(setMainPanelPara(QString,QString,QString,QString)));

        chainStatisticsPage->setClientModel(clientModel);
        rpcConsole->setClientModel(clientModel);
        chainExplorerPage->setClientModel(clientModel);

        addressBookPage->setOptionsModel(clientModel->getOptionsModel());
        receiveCoinsPage->setOptionsModel(clientModel->getOptionsModel());
    }
}

void BitcoinGUI::setWalletModel(WalletModel *walletModel)
{
    this->walletModel = walletModel;
    if(walletModel)
    {
        // Report errors from wallet thread
        connect(walletModel, SIGNAL(error(QString,QString,bool)), this, SLOT(error(QString,QString,bool)));

        // Put transaction list in tabs
        transactionView->setModel(walletModel);

        overviewPage->setModel(walletModel);
        addressBookPage->setModel(walletModel->getAddressTableModel());
        receiveCoinsPage->setModel(walletModel->getAddressTableModel());
        sendCoinsPage->setModel(walletModel);
        signVerifyMessageDialog->setModel(walletModel);

        chainExplorerPage->setModel(walletModel);

        setEncryptionStatus(walletModel->getEncryptionStatus());
        connect(walletModel, SIGNAL(encryptionStatusChanged(int)), this, SLOT(setEncryptionStatus(int)));

        // Balloon pop-up for new transaction
        connect(walletModel->getTransactionTableModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
                this, SLOT(incomingTransaction(QModelIndex,int,int)));

        // Ask for passphrase if needed
        connect(walletModel, SIGNAL(requireUnlock()), this, SLOT(unlockWallet()));
    }
}

void BitcoinGUI::createTrayIcon()
{
    QMenu *trayIconMenu;
#ifndef Q_OS_MAC
    trayIcon = new QSystemTrayIcon(this);
    trayIconMenu = new QMenu(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("Montecoin client"));
    trayIcon->setIcon(QIcon(":/icons/toolbar"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
#else
    // Note: On Mac, the dock icon is used to provide the tray's functionality.
    MacDockIconHandler *dockIconHandler = MacDockIconHandler::instance();
    trayIconMenu = dockIconHandler->dockMenu();
    dockIconHandler->setMainWindow((QMainWindow *)this);
#endif

    // Configuration of the tray icon (or dock icon) icon menu
    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(sendCoinsAction);
    trayIconMenu->addAction(receiveCoinsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(signMessageAction);
    trayIconMenu->addAction(verifyMessageAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(optionsAction);
    trayIconMenu->addAction(openRPCConsoleAction);
#ifndef Q_OS_MAC // This is built-in on Mac
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
#endif

    notificator = new Notificator(qApp->applicationName(), trayIcon);
}

#ifndef Q_OS_MAC
void BitcoinGUI::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        // Click on system tray icon triggers show/hide of the main window
        toggleHideAction->trigger();
    }
}
#endif

void BitcoinGUI::optionsClicked()
{
    if(!clientModel || !clientModel->getOptionsModel())
        return;
    OptionsDialog dlg;
    dlg.setModel(clientModel->getOptionsModel());
    dlg.exec();
}

void BitcoinGUI::aboutClicked()
{
    AboutDialog dlg;
    dlg.setModel(clientModel);
    dlg.exec();
}


void BitcoinGUI::setMainPanelPara(QString strSupply,QString strBalance,QString strHashrate,QString strVersion)
{
    supplyLabel->setText(strSupply);
    balanceLabel->setText(strBalance);
    nethashLabel->setText(strHashrate);
    versionLabel->setText(strVersion);
}

void BitcoinGUI::setNumConnections(int count)
{
    QString icon;
    switch(count)
    {
    case 0: icon = ":/icons/connect_0"; break;
    case 1: case 2: case 3: icon = ":/icons/connect_1"; break;
    case 4: case 5: case 6: icon = ":/icons/connect_2"; break;
    case 7: case 8: case 9: icon = ":/icons/connect_3"; break;
    default: icon = ":/icons/connect_4"; break;
    }
    labelConnectionsIcon->setPixmap(QIcon(icon).pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
    labelConnectionsIcon->setToolTip(tr("%n active connection(s) to Montecoin network", "", count));
    connectionLabel->setText(tr("%n","",count));
}

void BitcoinGUI::setNumBlocks(int count, int nTotalBlocks)
{
    // don't show / hide progress bar and its label if we have no connection to the network
    if (!clientModel || clientModel->getNumConnections() == 0)
    {
        progressBarLabel->setVisible(false);
        progressBar->setVisible(false);

        return;
    }

    QString strStatusBarWarnings = clientModel->getStatusBarWarnings();
    QString tooltip;

    if(count < nTotalBlocks)
    {
        int nRemainingBlocks = nTotalBlocks - count;
        float nPercentageDone = count / (nTotalBlocks * 0.01f);

        if (strStatusBarWarnings.isEmpty())
        {
            progressBarLabel->setText(tr("Synchronizing with network..."));
            progressBarLabel->setVisible(true);
            progressBar->setFormat(tr("~%n block(s) remaining", "", nRemainingBlocks));
            progressBar->setMaximum(nTotalBlocks);
            progressBar->setValue(count);
            progressBar->setVisible(true);
        }

        tooltip = tr("Downloaded %1 of %2 blocks of transaction history (%3% done).").arg(count).arg(nTotalBlocks).arg(nPercentageDone, 0, 'f', 2);
    }
    else
    {
        if (strStatusBarWarnings.isEmpty())
            progressBarLabel->setVisible(false);

        progressBar->setVisible(false);
        tooltip = tr("Downloaded %1 blocks of transaction history.").arg(count);
    }

    // Override progressBarLabel text and hide progress bar, when we have warnings to display
    if (!strStatusBarWarnings.isEmpty())
    {
        QString s1 = "Info:Minting suspended due to locked wallet.";
        QString s2 = "Warning: This version is obsolete, upgrade required!";
        if (strStatusBarWarnings == s1)
        {
            progressBarLabel->setText(tr("Info:Minting suspended due to locked wallet."));
        }
        else
        if (strStatusBarWarnings == s2)
        {
            progressBarLabel->setText(tr("Warning: This version is obsolete, upgrade required!"));
        }
        else
            progressBarLabel->setText(strStatusBarWarnings);
        progressBarLabel->setVisible(true);
        progressBar->setVisible(false);
    }

	tooltip = tr("Current difficulty is %1.").arg(clientModel->GetDifficulty()) + QString("<br>") + tooltip;

    QDateTime lastBlockDate = clientModel->getLastBlockDate();
    int secs = lastBlockDate.secsTo(QDateTime::currentDateTime());
    QString text;

    // Represent time from last generated block in human readable text
    if(secs <= 0)
    {
        // Fully up to date. Leave text empty.
    }
    else if(secs < 60)
    {
        text = tr("%n second(s) ago","",secs);
    }
    else if(secs < 60*60)
    {
        text = tr("%n minute(s) ago","",secs/60);
    }
    else if(secs < 24*60*60)
    {
        text = tr("%n hour(s) ago","",secs/(60*60));
    }
    else
    {
        text = tr("%n day(s) ago","",secs/(60*60*24));
    }

    // Set icon state: spinning if catching up, tick otherwise
    if(secs < 90*60 && count >= nTotalBlocks)
    {
        tooltip = tr("Up to date") + QString(".<br>") + tooltip;
        labelBlocksIcon->setPixmap(QIcon(":/icons/synced").pixmap(STATUSBAR_ICONSIZE, STATUSBAR_ICONSIZE));

        overviewPage->showOutOfSyncWarning(false);
    }
    else
    {
        tooltip = tr("Catching up...") + QString("<br>") + tooltip;
        labelBlocksIcon->setMovie(syncIconMovie);
        syncIconMovie->start();

        overviewPage->showOutOfSyncWarning(true);
    }

    if(!text.isEmpty())
    {
        tooltip += QString("<br>");
        tooltip += tr("Last received block was generated %1.").arg(text);
    }

    // Don't word-wrap this (fixed-width) tooltip
    tooltip = QString("<nobr>") + tooltip + QString("</nobr>");

    labelBlocksIcon->setToolTip(tooltip);
    progressBarLabel->setToolTip(tooltip);
    progressBar->setToolTip(tooltip);
    heightLabel->setText(tr("%n","",count));
}

void BitcoinGUI::error(const QString &title, const QString &message, bool modal)
{
    // Report errors from network/worker thread
    if(modal)
    {
        QMessageBox::critical(this, title, message, QMessageBox::Ok, QMessageBox::Ok);
    } else {
        notificator->notify(Notificator::Critical, title, message);
    }
}

void BitcoinGUI::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
#ifndef Q_OS_MAC // Ignored on Mac
    if(e->type() == QEvent::WindowStateChange)
    {
        if(clientModel && clientModel->getOptionsModel()->getMinimizeToTray())
        {
            QWindowStateChangeEvent *wsevt = static_cast<QWindowStateChangeEvent*>(e);
            if(!(wsevt->oldState() & Qt::WindowMinimized) && isMinimized())
            {
                QTimer::singleShot(0, this, SLOT(hide()));
                e->ignore();
            }
        }
    }
#endif
}

void BitcoinGUI::closeEvent(QCloseEvent *event)
{
    if(clientModel)
    {
#ifndef Q_OS_MAC // Ignored on Mac
        if(!clientModel->getOptionsModel()->getMinimizeToTray() &&
           !clientModel->getOptionsModel()->getMinimizeOnClose())
        {
            qApp->quit();
        }
#endif
    }
    QMainWindow::closeEvent(event);
}

void BitcoinGUI::askFee(qint64 nFeeRequired, bool *payFee)
{
    QString strMessage =
        tr("This transaction is over the size limit.  You can still send it for a fee of %1, "
          "which goes to the nodes that process your transaction and helps to support the network.  "
          "Do you want to pay the fee?").arg(
                BitcoinUnits::formatWithUnit(BitcoinUnits::BTC, nFeeRequired));
    QMessageBox::StandardButton retval = QMessageBox::question(
          this, tr("Confirm transaction fee"), strMessage,
          QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes);
    *payFee = (retval == QMessageBox::Yes);
}

void BitcoinGUI::incomingTransaction(const QModelIndex & parent, int start, int end)
{
    if(!walletModel || !clientModel)
        return;
    TransactionTableModel *ttm = walletModel->getTransactionTableModel();
    qint64 amount = ttm->index(start, TransactionTableModel::Amount, parent)
                    .data(Qt::EditRole).toULongLong();
    if(!clientModel->inInitialBlockDownload())
    {
        // On new transaction, make an info balloon
        // Unless the initial block download is in progress, to prevent balloon-spam
        QString date = ttm->index(start, TransactionTableModel::Date, parent)
                        .data().toString();
        QString type = ttm->index(start, TransactionTableModel::Type, parent)
                        .data().toString();
        QString address = ttm->index(start, TransactionTableModel::ToAddress, parent)
                        .data().toString();
        QIcon icon = qvariant_cast<QIcon>(ttm->index(start,
                            TransactionTableModel::ToAddress, parent)
                        .data(Qt::DecorationRole));

        notificator->notify(Notificator::Information,
                            (amount)<0 ? tr("Sent transaction") :
                                         tr("Incoming transaction"),
                              tr("Date: %1\n"
                                 "Amount: %2\n"
                                 "Type: %3\n"
                                 "Address: %4\n")
                              .arg(date)
                              .arg(BitcoinUnits::formatWithUnit(walletModel->getOptionsModel()->getDisplayUnit(), amount, true))
                              .arg(type)
                              .arg(address), icon);
    }
}

void BitcoinGUI::gotoOverviewPage()
{
    overviewAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(overviewPage);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
}

void BitcoinGUI::gotoHistoryPage()
{
    historyAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(transactionsPage);

    exportAction->setEnabled(true);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), transactionView, SLOT(exportClicked()));
}

void BitcoinGUI::gotoAddressBookPage()
{
    addressBookAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(addressBookPage);

    exportAction->setEnabled(true);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), addressBookPage, SLOT(exportClicked()));
}

void BitcoinGUI::gotoReceiveCoinsPage()
{
    receiveCoinsAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(receiveCoinsPage);

    exportAction->setEnabled(true);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), receiveCoinsPage, SLOT(exportClicked()));
}

void BitcoinGUI::gotoSendCoinsPage()
{
    sendCoinsAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(sendCoinsPage);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
}

void BitcoinGUI::gotoSignMessageTab(QString addr)
{
    // call show() in showTab_SM()
    signVerifyMessageDialog->showTab_SM(true);

    if(!addr.isEmpty())
        signVerifyMessageDialog->setAddress_SM(addr);
}

void BitcoinGUI::gotoVerifyMessageTab(QString addr)
{
    // call show() in showTab_VM()
    signVerifyMessageDialog->showTab_VM(true);

    if(!addr.isEmpty())
        signVerifyMessageDialog->setAddress_VM(addr);
}

//montecoin:gui
void BitcoinGUI::gotoWelcomePage()
{
   // appMenuBar->setVisible(false);
  //  statusBar()->setVisible(false);
  //  topWidget->setVisible(false);
  //  middleBarWidget->setVisible(false);
    homepageAction->setChecked(true);

    centralStackedWidget->setCurrentWidget(welcomePage);
}
void BitcoinGUI::gotoChainMozzPage()
{
    chainMozzAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(bitcoinPage);
}

void BitcoinGUI::gotoChainStatisticsPage()
{
    chainStaAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(chainStatisticsPage);
}

void BitcoinGUI::gotoChainExplorerPage()
{
    chainExplorerAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(chainExplorerPage);
}

void BitcoinGUI::gotoConsolePage()
{
    consoleAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(consolePage);
}

void BitcoinGUI::gotoMinerkingPage()
{
    gameMinerkingAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(minerkingPage);
}

void BitcoinGUI::gotoLotteryPage()
{
    liveLotteryAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(lotteryPage);
}

void BitcoinGUI::gotoMozzdicePage()
{
    liveGamblingAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(mozzdicePage);
}

void BitcoinGUI::gotoMozzriaAboutPage()
{
    riaAboutAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(mozzriaAboutPage);
}

void BitcoinGUI::gotoMozzriaContactPage()
{
    riaContactAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(mozzriaContactPage);
}

void BitcoinGUI::gotoSettingPage()
{
    settingAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(settingPage);

    if(!clientModel || !clientModel->getOptionsModel())
        return;
    settingPage->setModel(clientModel->getOptionsModel());
}

void BitcoinGUI::gotoHelpPage()
{
    helpAction->setChecked(true);
    centralStackedWidget->setCurrentWidget(helpPage);
}

void BitcoinGUI::ListWidgetItemSelectionChanged()
{
/*
    if(listWidget->currentItem() == listItemWelcome)
    {
        toolbarsStack->setCurrentWidget(tbWelcome);
       // gotoWelcomePage();
        gotoChainMozzPage();
    }
    else if(listWidget->currentItem() == listItemWallet)
    {
        toolbarsStack->setCurrentWidget(tbWallet);
        gotoOverviewPage();
    }
    else if(listWidget->currentItem() == listItemBlockchain)
    {
        toolbarsStack->setCurrentWidget(tbBlockchain);
        gotoChainExplorerPage();
    }
    else if(listWidget->currentItem() == listItemConsole)
    {
        toolbarsStack->setCurrentWidget(tbConsole);
        gotoConsolePage();
    }
    else if(listWidget->currentItem() == listItemMozzgame)
    {
        toolbarsStack->setCurrentWidget(tbMozzgame);
        gotoMinerkingPage();
    }
    else if(listWidget->currentItem() == listItemMozzlive)
    {
        toolbarsStack->setCurrentWidget(tbMozzlive);
        gotoLotteryPage();
    }
    else if(listWidget->currentItem() == listItemMozzria)
    {
        toolbarsStack->setCurrentWidget(tbMozzria);
        gotoMozzriaAboutPage();
    }
    else if(listWidget->currentItem() == listItemSetting)
    {
        toolbarsStack->setCurrentWidget(tbSetting);
        gotoSettingPage();
    }
    else if(listWidget->currentItem() == listItemHelp)
    {
        toolbarsStack->setCurrentWidget(tbHelp);
        gotoHelpPage();
    }

    if(listWidget->currentItem() == listItemBlockchain)
        emit autoUpdateHomePage(true);
    else
        emit autoUpdateHomePage(false);*/
}

void BitcoinGUI::minerKingDeposit()
{
    listWidget->setCurrentRow(1);
    toolbarsStack->setCurrentWidget(tbWallet);
    gotoSendCoinsPage();
}

void BitcoinGUI::OnWelcomePageClicked(int panelID )
{
    //panelID = wallet:0 chain:1  console:2 setting:3
    //panelID = game:4   live:5   mozzria:6 help:7    montecoin:8
    switch (panelID) {
    case 0:
    {
        //listWidget->setCurrentRow(6);
        gotoOverviewPage();
        toolbarsStack->setCurrentWidget(tbWallet);
        topWidget->setStyleSheet("background-image:url(:images/wallet_bg)");

        break;
    }
    case 1:
    {
        //listWidget->setCurrentRow(2);
        gotoChainExplorerPage();
        toolbarsStack->setCurrentWidget(tbBlockchain);
        topWidget->setStyleSheet("background-image:url(:images/chain_bg)");
        break;
    }
    case 2:
    {
        //listWidget->setCurrentRow(4);
        gotoConsolePage();
        toolbarsStack->setCurrentWidget(tbConsole);
        topWidget->setStyleSheet("background-image:url(:images/console_bg)");
        break;
    }
    case 3:
    {
        //listWidget->setCurrentRow(5);
        gotoSettingPage();
        toolbarsStack->setCurrentWidget(tbSetting);
        topWidget->setStyleSheet("background-image:url(:images/setting_bg)");
        break;
    }
    case 4:
    {
       // listWidget->setCurrentRow(4);
        gotoMinerkingPage();
        toolbarsStack->setCurrentWidget(tbMozzgame);
        topWidget->setStyleSheet("background-image:url(:images/game_bg)");
        break;
    }
    case 5:
    {
        //marktodo :
        return;

        //  listWidget->setCurrentRow(5);
        gotoLotteryPage();
        toolbarsStack->setCurrentWidget(tbMozzlive);
        topWidget->setStyleSheet("background-image:url(:images/live_bg)");
        break;
    }
    case 6:
    {
      //  listWidget->setCurrentRow(6);
        gotoMozzriaAboutPage();
        toolbarsStack->setCurrentWidget(tbMozzria);
        topWidget->setStyleSheet("background-image:url(:images/mozzria_bg)");
        break;
    }
    case 7:
    {
      //  listWidget->setCurrentRow(5);
        gotoHelpPage();
        toolbarsStack->setCurrentWidget(tbHelp);
         topWidget->setStyleSheet("background-image:url(:images/help_bg)");
        break;
    }
    case 8://
    {
      //  listWidget->setCurrentRow(2);
        gotoChainMozzPage();
        toolbarsStack->setCurrentWidget(tbWelcome);
        topWidget->setStyleSheet("background-image:url(:images/wallet_bg)");
        break;
    }
    default:
        break;
    }

    appMenuBar->setVisible(true);
    statusBar()->setVisible(true);
    //toolbarsStack->setCurrentIndex(panelID);
    //topWidget->setVisible(true);
    middleBarWidget->setVisible(true);
}

void BitcoinGUI::dragEnterEvent(QDragEnterEvent *event)
{
    // Accept only URIs
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void BitcoinGUI::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        int nValidUrisFound = 0;
        QList<QUrl> uris = event->mimeData()->urls();
        foreach(const QUrl &uri, uris)
        {
            if (sendCoinsPage->handleURI(uri.toString()))
                nValidUrisFound++;
        }

        // if valid URIs were found
        if (nValidUrisFound)
            gotoSendCoinsPage();
        else
            notificator->notify(Notificator::Warning, tr("URI handling"), tr("URI can not be parsed! This can be caused by an invalid Montecoin address or malformed URI parameters."));
    }

    event->acceptProposedAction();
}

void BitcoinGUI::handleURI(QString strURI)
{
    // URI has to be valid
    if (sendCoinsPage->handleURI(strURI))
    {
        showNormalIfMinimized();
        gotoSendCoinsPage();
    }
    else
        notificator->notify(Notificator::Warning, tr("URI handling"), tr("URI can not be parsed! This can be caused by an invalid Montecoin address or malformed URI parameters."));
}

void BitcoinGUI::setEncryptionStatus(int status)
{
    switch(status)
    {
    case WalletModel::Unencrypted:
        labelEncryptionIcon->hide();
        encryptWalletAction->setChecked(false);
        changePassphraseAction->setEnabled(false);
        encryptWalletAction->setEnabled(true);
        //lockMintingAction->setEnabled(false); //
        //lockMintingAction->setText(tr("&Wallet Unencrypted"));
        if(GetBoolArg("-mint",false))
        {
            lockMintingAction->setText(tr("&Lock Wallet To Stop Minting"));
            lockMintingAction->setIcon(QIcon(":/icons/minting_open"));
        }
        else
        {
            lockMintingAction->setText(tr("&Unlock Wallet For Minting Only"));
            lockMintingAction->setIcon(QIcon(":/icons/minting_closed"));
        }
        break;
    case WalletModel::Unlocked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_open").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(tr("Wallet is <b>encrypted</b> and currently <b>unlocked</b>"));
        encryptWalletAction->setChecked(true);
        changePassphraseAction->setEnabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        lockMintingAction->setEnabled(true);
        break;
    case WalletModel::Locked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_closed").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(tr("Wallet is <b>encrypted</b> and currently <b>locked</b>"));
        encryptWalletAction->setChecked(true);
        changePassphraseAction->setEnabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        lockMintingAction->setEnabled(true);
        break;
    }
}

void BitcoinGUI::encryptWallet(bool status)
{
    if(!walletModel)
        return;
    AskPassphraseDialog dlg(status ? AskPassphraseDialog::Encrypt:
                                     AskPassphraseDialog::Decrypt, this);
    dlg.setModel(walletModel);
    dlg.exec();

    setEncryptionStatus(walletModel->getEncryptionStatus());
}

//marktodo
void BitcoinGUI::wtocSnapshot(bool status)
{
    if(!walletModel)
        return;
    QString saveDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QString filename = QFileDialog::getOpenFileName(this, tr("Import WTOC Wallet"), saveDir, tr("Wallet Data (*.dat)"));

    if(!filename.isEmpty()) {

        //load wallet
        if(!walletModel->LoadSnapshotWallet(filename.toStdString())) {
            QMessageBox::warning(this, tr("Load WTOC Wallet File Failed"), tr("Can not load keys from WTOC wallet"));
        }

        //unlock
        if(walletModel->IsSnapshotWalletCrypto(filename.toStdString()))
        {
            AskPassphraseDialog dlg(AskPassphraseDialog::SnapshotUnlock, this);
            dlg.setModel(walletModel);
            if(dlg.exec() != QDialog::Accepted)
            {
                return;
            }
        }

        //import keys
        unsigned int nFlags = 0;
        if(!walletModel->ImportSnapshotWalletKeys(nFlags)) {
            if(nFlags == 10000)
                QMessageBox::warning(this, tr("Snapshot Failed"), tr("Wallet is locked,please open console then unlock with command: walletpassphrase"));

            if(nFlags == 10001)
                QMessageBox::warning(this, tr("Snapshot Failed"), tr("Wallet is unlocked for minting only,please stop minting, open console and then unlock with command: walletpassphrase"));
        }
        else
        {
            QString strNokeys = tr("Detection of WTOC wallet finished,no keys with coins generated by genesis block");
            QMessageBox::warning(this, tr("Snapshot Finished"), nFlags == 0 ? strNokeys :tr("Import private keys succeeded,If you need to encrypt, please restart your wallet first"));
        }
    }
}

static bool fMining = false;//
static bool fMinting = false;

void BitcoinGUI::lockWalletForMinting(bool status)
{
    if(!walletModel)
        return;

    if (vNodes.empty() || IsInitialBlockDownload())
    {
        QMessageBox::warning(this, tr("Synchronizing"),
            tr("Please wait until finish of synchronize"),
            QMessageBox::Ok, QMessageBox::Ok);
        return ;
    }

    if(fMinting)
    {

        if(walletModel->getEncryptionStatus() != WalletModel::Unencrypted)
        {

            if(!fMining)
            {
              walletModel->setWalletLocked(true);
              fWalletUnlockMintOnly = false;
            }
        }

        fMinting = false;
        lockMintingAction->setText(tr("&Unlock Wallet For Minting Only"));
        lockMintingAction->setIcon(QIcon(":/icons/minting_closed"));
        mapArgs["-mint"] = "0";
    }

    else
    {

        if(walletModel->getEncryptionStatus() != WalletModel::Unencrypted)
        {
            if(walletModel->getEncryptionStatus() == WalletModel::Locked)
            {
                AskPassphraseDialog dlg(AskPassphraseDialog::Unlock, this);
                dlg.setModel(walletModel);
                if(dlg.exec() == QDialog::Accepted)
                {
                    fWalletUnlockMintOnly = true;
                }
                else
                {
                    return;
                }
            }
        }

        fMinting = true;
        lockMintingAction->setText(tr("&Lock Wallet To Stop Minting"));
        lockMintingAction->setIcon(QIcon(":/icons/minting_open"));
        mapArgs["-mint"] = "1";
    }
}


void BitcoinGUI::StartOrStopGenerate()
{
    if(!walletModel)
        return;

    if (vNodes.empty() || IsInitialBlockDownload())
    {
        QMessageBox::warning(this, tr("Synchronizing"),
            tr("Please wait until finish of synchronize"),
            QMessageBox::Ok, QMessageBox::Ok);
        return ;
    }

    if(fMining)
    {

        if(walletModel->getEncryptionStatus() != WalletModel::Unencrypted)
        {

            if(!fMinting)
            {
              walletModel->setWalletLocked(true);
              fWalletUnlockMintOnly = false;
            }
        }
        fMining = false;
        mapArgs["-gen"] = "0";
        genAction->setText(tr("&Generate"));
        genAction->setIcon(QIcon(":/icons/mining_inactive"));
        GenerateBitcoins(false, pwalletMain);
    }

    else
    {

        if(walletModel->getEncryptionStatus() != WalletModel::Unencrypted)
        {
            if(walletModel->getEncryptionStatus() == WalletModel::Locked)
            {
                AskPassphraseDialog dlg(AskPassphraseDialog::Unlock, this);
                dlg.setModel(walletModel);
                if(dlg.exec() == QDialog::Accepted)
                {
                    fWalletUnlockMintOnly = true;
                }
                else
                {
                    return;
                }
            }
        }
        fMining = true;
        mapArgs["-gen"] = "1";
        genAction->setText(tr("&DisGenerate"));
        genAction->setIcon(QIcon(":/icons/mining_active"));
        GenerateBitcoins(true, pwalletMain);
    }
}

void BitcoinGUI::backupWallet()
{
    QString saveDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QString filename = QFileDialog::getSaveFileName(this, tr("Backup Wallet"), saveDir, tr("Wallet Data (*.dat)"));
    if(!filename.isEmpty()) {
        if(!walletModel->backupWallet(filename)) {
            QMessageBox::warning(this, tr("Backup Failed"), tr("There was an error trying to save the wallet data to the new location."));
        }
    }
}

void BitcoinGUI::changePassphrase()
{
    AskPassphraseDialog dlg(AskPassphraseDialog::ChangePass, this);
    dlg.setModel(walletModel);
    dlg.exec();
}

void BitcoinGUI::unlockWallet()
{
    if(!walletModel)
        return;
    // Unlock wallet when requested by wallet model
    if(walletModel->getEncryptionStatus() == WalletModel::Locked)
    {
        AskPassphraseDialog dlg(AskPassphraseDialog::Unlock, this);
        dlg.setModel(walletModel);
        dlg.exec();
    }
}

void BitcoinGUI::showNormalIfMinimized(bool fToggleHidden)
{
    // activateWindow() (sometimes) helps with keyboard focus on Windows
    if (isHidden())
    {
        show();
        activateWindow();
    }
    else if (isMinimized())
    {
        showNormal();
        activateWindow();
    }
    else if (GUIUtil::isObscured(this))
    {
        raise();
        activateWindow();
    }
    else if(fToggleHidden)
        hide();
}

void BitcoinGUI::toggleHidden()
{
    showNormalIfMinimized(true);
}
