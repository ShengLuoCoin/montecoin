#ifndef BITCOINGUI_H
#define BITCOINGUI_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class TransactionTableModel;
class ClientModel;
class WalletModel;
class TransactionView;
class OverviewPage;
class AddressBookPage;
class SendCoinsDialog;
class SignVerifyMessageDialog;
class Notificator;
class RPCConsole;

//montecoin:gui
class WelcomePage;
class BitcoinPage;
class ChainStatisticsPage;
class ChainExplorerPage;
class ConsolePage;
class MinerkingPage;
class LotteryPage;
class MozzdicePage;
class MozzriaAboutPage;
class MozzriaContactPage;
class SettingPage;
class HelpPage;

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTableView;
class QAbstractItemModel;
class QModelIndex;
class QProgressBar;
class QStackedWidget;
class QUrl;  
class QListWidget;
class QPushButton;
class QFrame;
class QListWidgetItem;

QT_END_NAMESPACE

/**
  Bitcoin GUI main class. This class represents the main window of the Bitcoin UI. It communicates with both the client and
  wallet models to give the user an up-to-date view of the current core state.
*/
class BitcoinGUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit BitcoinGUI(QWidget *parent = 0);
    ~BitcoinGUI();

    /** Set the client model.
        The client model represents the part of the core that communicates with the P2P network, and is wallet-agnostic.
    */
    void setClientModel(ClientModel *clientModel);
    /** Set the wallet model.
        The wallet model represents a bitcoin wallet, and offers access to the list of transactions, address book and sending
        functionality.
    */
    void setWalletModel(WalletModel *walletModel);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    ClientModel *clientModel;
    WalletModel *walletModel;

    QStackedWidget *centralStackedWidget;

    OverviewPage *overviewPage;
    QWidget *transactionsPage;
    AddressBookPage *addressBookPage;
    AddressBookPage *receiveCoinsPage;
    SendCoinsDialog *sendCoinsPage;
    SignVerifyMessageDialog *signVerifyMessageDialog;

    //montecoin:gui
    WelcomePage *welcomePage;
    BitcoinPage *bitcoinPage;
    ChainStatisticsPage *chainStatisticsPage;
    ChainExplorerPage *chainExplorerPage;
    ConsolePage *consolePage;
    MinerkingPage *minerkingPage;
    LotteryPage *lotteryPage;
    MozzdicePage *mozzdicePage;
    MozzriaAboutPage *mozzriaAboutPage;
    MozzriaContactPage *mozzriaContactPage;
    SettingPage *settingPage;
    HelpPage *helpPage;

    QLabel *labelEncryptionIcon;
    QLabel *labelConnectionsIcon;
    QLabel *labelBlocksIcon;
    QLabel *progressBarLabel;
    QProgressBar *progressBar;

    QMenuBar *appMenuBar;
    QAction *overviewAction;
    QAction *historyAction;
    QAction *genAction; //一件挖矿
    QAction *quitAction;
    QAction *sendCoinsAction;
    QAction *addressBookAction;
    QAction *signMessageAction;
    QAction *verifyMessageAction;
    QAction *aboutAction;
    QAction *receiveCoinsAction;
    QAction *optionsAction;
    QAction *toggleHideAction;
    QAction *exportAction;
    QAction *encryptWalletAction;
    QAction *backupWalletAction;
    QAction *changePassphraseAction;
    QAction *aboutQtAction;
    QAction *openRPCConsoleAction;
    QAction *snapshotWalletAction; //MOZZ快照
    QAction *lockMintingAction;

    //montecoin:gui
    QAction *homepageAction;
    QAction *chainStaAction;
    QAction *chainExplorerAction;
    QAction *chainMozzAction;
    QAction *consoleAction;
    QAction *gameMinerkingAction;
    QAction *liveLotteryAction;
    QAction *liveGamblingAction;
    QAction *riaAboutAction;
    QAction *riaContactAction;
    QAction *settingAction;
    QAction *helpAction;

    QSystemTrayIcon *trayIcon;
    Notificator *notificator;
    TransactionView *transactionView;
    RPCConsole *rpcConsole;

    QMovie *syncIconMovie;

    //montecoin:gui
    QListWidget *listWidget;
    QStackedWidget *toolbarsStack;
    QWidget *centerWindow;
    QLabel  *leftTopLabel;
    QLabel  *balanceLabel;
    QWidget *leftWidget;
    QPushButton *btWelcome;

    QListWidgetItem *listItemWelcome;
    QListWidgetItem *listItemWallet;
    QListWidgetItem *listItemBlockchain;
    QListWidgetItem *listItemConsole;
    QListWidgetItem *listItemMozzgame;
    QListWidgetItem *listItemMozzlive;
    QListWidgetItem *listItemMozzria;
    QListWidgetItem *listItemSetting;
    QListWidgetItem *listItemHelp;

    QFrame *msgWidget;
    QLabel  *supplyLabel;
    QLabel  *connectionLabel;
    QLabel  *heightLabel;
    QLabel  *nethashLabel;
    QLabel  *versionLabel;

    QLabel *lb00;

    QWidget *middleBarWidget; //右边页面的菜单条
    QWidget *rightWidget;
    QToolBar *tbHomePage;
    QToolBar *tbMintLock;
    QToolBar *tbMiningLock;//一键挖矿
    void createListToolbarStaked();

    QToolBar *tbWelcome,*tbWallet,*tbBlockchain,*tbConsole;
    QToolBar *tbMozzgame,*tbMozzlive,*tbMozzria,*tbSetting,*tbHelp;

    //新UI设计设计控件
    QWidget *topWidget; //最上面的图片区域

    /** Create the main UI actions. */
    void createActions();
    /** Create the menu bar and sub-menus. */
    void createMenuBar();
    /** Create the toolbars */
    void createToolBars();
    /** Create system tray (notification) icon */
    void createTrayIcon();

public slots:
    /** Set number of connections shown in the UI */
    void setNumConnections(int count);
    void setMainPanelPara(QString strSupply,QString strBalance,QString strHashrate,QString strVersion);
    /** Set number of blocks shown in the UI */
    void setNumBlocks(int count, int nTotalBlocks);
    /** Set the encryption status as shown in the UI.
       @param[in] status            current encryption status
       @see WalletModel::EncryptionStatus
    */
    void setEncryptionStatus(int status);

    /** Notify the user of an error in the network or transaction handling code. */
    void error(const QString &title, const QString &message, bool modal);
    /** Asks the user whether to pay the transaction fee or to cancel the transaction.
       It is currently not possible to pass a return value to another thread through
       BlockingQueuedConnection, so an indirected pointer is used.
       https://bugreports.qt-project.org/browse/QTBUG-10440

      @param[in] nFeeRequired       the required fee
      @param[out] payFee            true to pay the fee, false to not pay the fee
    */
    void askFee(qint64 nFeeRequired, bool *payFee);
    void handleURI(QString strURI);

private slots:
    /** Switch to overview (home) page */
    void gotoOverviewPage();
    /** Switch to history (transactions) page */
    void gotoHistoryPage();
    /** Switch to address book page */
    void gotoAddressBookPage();
    /** Switch to receive coins page */
    void gotoReceiveCoinsPage();
    /** Switch to send coins page */
    void gotoSendCoinsPage();
    void minerKingDeposit();

    //montecoin:gui
    /** Switch to Welcome page */
    void gotoWelcomePage();
    void gotoChainMozzPage();
    void gotoChainStatisticsPage();
    void gotoChainExplorerPage();
    void gotoConsolePage();
    void gotoMinerkingPage();
    void gotoLotteryPage();
    void gotoMozzdicePage();
    void gotoMozzriaAboutPage();
    void gotoMozzriaContactPage();
    void gotoSettingPage();
    void gotoHelpPage();
    void ListWidgetItemSelectionChanged();
    void OnWelcomePageClicked(int panelID );

    /** Show Sign/Verify Message dialog and switch to sign message tab */
    void gotoSignMessageTab(QString addr = "");
    /** Show Sign/Verify Message dialog and switch to verify message tab */
    void gotoVerifyMessageTab(QString addr = "");

    /** Show configuration dialog */
    void optionsClicked();
    /** Show about dialog */
    void aboutClicked();
#ifndef Q_OS_MAC
    /** Handle tray icon clicked */
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
#endif
    /** Show incoming transaction notification for new transactions.

        The new items are those between start and end inclusive, under the given parent item.
    */
    void incomingTransaction(const QModelIndex & parent, int start, int end);
    /** Encrypt the wallet */
    void encryptWallet(bool status);
    void wtocSnapshot(bool status);
    /** lock/unlock for minting */
    void lockWalletForMinting(bool status);

    void StartOrStopGenerate();

    /** Backup the wallet */
    void backupWallet();
    /** Change encrypted wallet passphrase */
    void changePassphrase();
    /** Ask for passphrase to unlock wallet temporarily */
    void unlockWallet();

    /** Show window if hidden, unminimize when minimized, rise when obscured or show if hidden and fToggleHidden is true */
    void showNormalIfMinimized(bool fToggleHidden = false);
    /** simply calls showNormalIfMinimized(true) for use in SLOT() macro */
    void toggleHidden();

signals:
    void autoUpdateHomePage(bool fUpdate);

};

#endif
