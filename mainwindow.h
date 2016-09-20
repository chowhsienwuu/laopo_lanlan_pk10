#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
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

    int intFloor(double in);

    //platfrom.

    const int PLATFORM_UNKONW = 0X0;
    const int PLATFORM_XINSHIJI = 0x01;
    const int PLATFORM_BAOXUAN  = 0x02;
    int mCurrentPlatform;
    void detectPlatFrom(QString &str);
    double getLastNumFromString(QString str);
private slots:
    void progress();
    void clear();
    void fastPayback();
};

#endif // MAINWINDOW_H
