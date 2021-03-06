#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QVector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTableView *mRsultTAbleView = NULL;
    QStandardItemModel *mcusterItem ;
    void processOneLineXinshiji(QString *line);
    void processOneLinebaoxuan(QString *line);
    void processOneLinenanshengbaoxuan(QString *line);
    void processOneLineali(QString *line);
    void processOneLinecalSum(QString *line);
    void processOneLinetianheguoji(QString *line);
    void processOneLinejingbaoli(QString *line);

    QString mTableTime;
    double mTotlePayback;

    typedef struct custerStruct{
        int index;
        QString name;
        QString type;
        double journal; //
        double winorlse;
        int paybackInAll;
        int playbackPaid;
        int playbackPaying;
    }T_custerStruct;

    QList<T_custerStruct> mCusterList;
    T_custerStruct mTotle;

    double journalpercet;

    //
    const double STATIC_JOURNAL_PERCET = 0.022;
    const char * STATIC_TYPE = "d";
    //

    int intFloor(double in);
    void initCusterStruct(T_custerStruct *cus);
    //platfrom.

    const int PLATFORM_UNKONW = 0X0;
    const int PLATFORM_XINSHIJI = 0x01;
    const int PLATFORM_BAOXUAN  = 0x02;
    const int PLATFORM_NANSHENGBAOXUAN = 0X03;
    const int PLATFORM_ALI = 0x04;
    const int PLATFORM_CALSUM = 0X5;
    const int PLATFORM_TIANHEGUOJI = 0x6;
    const int PLATFORM_JINGBAOLI = 0x7;

    //强制某平台
    const int FORCE_PLATFORM = PLATFORM_JINGBAOLI;

    int mCurrentPlatform;
    void detectPlatFrom(QString &str);
    double getLastNumFromString(QString str);
    bool mHasInited;
    void calSum();
private slots:
    void progress();
    void clear();
    void fastPayback022();
    void fastPayback024();
    void fastPayback026();
    void refreshData(QStandardItem*);
};



#endif // MAINWINDOW_H
