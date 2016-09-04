#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <iostream>
#include "QDebug"
#include <QDoubleSpinBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    journalpercet = 0.024;
    mTotlePayback = 0;
    mcusterItem = new QStandardItemModel();
    mTableTime = "";
    mCurrentPlatform = PLATFORM_UNKONW;
    //mCurrentPlatform = PLATFORM_XINSHIJI;
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(progress()));
    connect(ui->plainsrcTextEdit, SIGNAL(textChanged()), this, SLOT(progress()));
    connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clear()));
    connect(ui->fastpayback_src, SIGNAL(textChanged(QString)), this, SLOT(fastPayback()));

    //    mcusterItem->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("序号")));
    mcusterItem->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("帐号")));
    mcusterItem->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("名称")));
    mcusterItem->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("有效金额")));
    mcusterItem->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("输赢")));
    mcusterItem->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("退水")));


    mRsultTAbleView = ui->tableView;
    mRsultTAbleView->setShowGrid(true);
    mRsultTAbleView->setMouseTracking(true);
    mRsultTAbleView->setModel(mcusterItem);
    mRsultTAbleView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mRsultTAbleView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mRsultTAbleView->setSortingEnabled(true);
//    mRsultTAbleView->resizeRowsToContents();
//    mRsultTAbleView->resizeColumnsToContents();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processOneLineXinshiji(QString *line)
{
    if ( !line->startsWith("会员"))
    {
        return;
    }
    QStringList stringlist =  line->split(QRegularExpression("\\s+"));
    QString t_str;
    if (stringlist.length() == 18)
    {
        T_custerStruct t_custerStruct;
        t_custerStruct.type = stringlist.at(2);
        if (t_custerStruct.type.startsWith("d", Qt::CaseInsensitive)
                || t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
        {
            t_custerStruct.name = stringlist.at(1); //n6888

            t_str = stringlist.at(5);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.journal = t_str.toDouble(); //908.0

            t_str = stringlist.at(6);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.winorlse = t_str.toDouble(); //170.7

            if (t_custerStruct.type.startsWith("d", Qt::CaseInsensitive)
                    || t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
            {
                t_custerStruct.payback = intFloor(t_custerStruct.journal * journalpercet);
                mTotlePayback += t_custerStruct.payback;
            }else {
                t_custerStruct.payback = 0;
            }

            mCusterList.append(t_custerStruct);
        }
    }
}

void MainWindow::processOneLinebaoxuan(QString *line)
{
    // yaofa168	c	27	111.0	-91.6	111.0	91.6	0.000	 	0.00	3.1	3.1	111.0	88.5
    QStringList stringlist =  line->split(QRegularExpression("\\s+"));
    QString t_str;
    if (stringlist.length() == 13)
    {
        T_custerStruct t_custerStruct;
        t_custerStruct.type = stringlist.at(1);
        if (t_custerStruct.type.startsWith("d", Qt::CaseInsensitive)
                || t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
        {
            t_custerStruct.name = stringlist.at(0); //n6888

            t_str = stringlist.at(3);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.journal = t_str.toDouble(); //908.0

            t_str = stringlist.at(4);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.winorlse = t_str.toDouble(); //170.7

            if (t_custerStruct.type.startsWith("d", Qt::CaseInsensitive)
                    || t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
            {
                t_custerStruct.payback = intFloor(t_custerStruct.journal * journalpercet);
                mTotlePayback += t_custerStruct.payback;
            }else {
                t_custerStruct.payback = 0;
            }

            mCusterList.append(t_custerStruct);
        }
    }
}

int MainWindow::intFloor(double in)
{
    return (int)floor(in);
}

void MainWindow::detectPlatFrom(QString &text)
{
    QTextStream str(&text, QIODevice::ReadOnly);
    QString line;
    while(!str.atEnd())
    {
        line = str.readLine();
        QStringList stringlist =  line.split(QRegularExpression("\\s+"));
        QString t_str;
        if(stringlist.length() == 13){
            t_str = stringlist.at(1);
            if (t_str.startsWith("d", Qt::CaseInsensitive)
                    || t_str.startsWith("c", Qt::CaseInsensitive)
                    || t_str.startsWith("a", Qt::CaseInsensitive))
            {
                mCurrentPlatform = PLATFORM_BAOXUAN;
                break;
            }
        }
        if(stringlist.length() == 18){
            t_str = stringlist.at(2);
            if (t_str.startsWith("d", Qt::CaseInsensitive)
                    || t_str.startsWith("c", Qt::CaseInsensitive)
                    || t_str.startsWith("a", Qt::CaseInsensitive))
            {
                mCurrentPlatform = PLATFORM_XINSHIJI;
                break;
            }
        }
    }
    qDebug() << "detect platform : " << mCurrentPlatform ;
}

void MainWindow::progress()
{
//    qDebug("..on progress");
//    mcusterItem->clear();
    mCusterList.clear();
    mTotlePayback = 0;
    QString text = ui->plainsrcTextEdit->toPlainText();
   detectPlatFrom(text);

    QTextStream str(&text, QIODevice::ReadOnly);
    QString line;
    while(!str.atEnd())
    {
        line = str.readLine();
        if (mCurrentPlatform == PLATFORM_XINSHIJI){
            processOneLineXinshiji(&line);
        }else if (mCurrentPlatform == PLATFORM_BAOXUAN){
            processOneLinebaoxuan(&line);
        }
    }

    ui->label_time->setText(mTableTime);
    for (int i = 0; i < mCusterList.size(); i++)
    {
        mcusterItem->setItem(i, 0, new QStandardItem(mCusterList.at(i).name));
        mcusterItem->setItem(i, 1, new QStandardItem(mCusterList.at(i).type));
        mcusterItem->setItem(i, 2, new QStandardItem(QString::number(mCusterList.at(i).journal)));
        mcusterItem->setItem(i, 3, new QStandardItem(QString::number(mCusterList.at(i).winorlse)));
        mcusterItem->setItem(i, 4, new QStandardItem(QString::number(mCusterList.at(i).payback)));
    }
}

void MainWindow::clear()
{
    ui->plainsrcTextEdit->clear();
    mCusterList.clear();
}

void MainWindow::fastPayback()
{
    QString t_str = ui->fastpayback_src->text();

    t_str = t_str.remove(QChar(','));
    double journal = t_str.toDouble(); //
    journal *= journalpercet;
    int palybak = intFloor(journal);
    QString show = "退水: " +   QString::number(palybak);

    ui->fastPaybacklable->setText(show);
}


