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
    QStandardItemModel *mcusterItem = new QStandardItemModel();
    void processOneLine(QString *line);

    QString mTableTime;
    double mTotlePayback;

    typedef struct custerStruct{
        int index;
        QString name;
        QString type;
        double journal; //
        double winorlse;
        double payback;
    }T_custerStruct;

    QList<T_custerStruct> mCusterList;
    T_custerStruct mTotle;

    double journalpercet = 0.024;

private slots:
    void progress();
    void clear();
    void fastPayback();
};

#endif // MAINWINDOW_H
