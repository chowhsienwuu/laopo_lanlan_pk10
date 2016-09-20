#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <iostream>
#include "QDebug"
#include <QDoubleSpinBox>
#include <QItemDelegate>

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
    connect(ui->plainsrcTextEdit, SIGNAL(textChanged()), this, SLOT(progress()));
    connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clear()));
    connect(ui->fastpayback_src, SIGNAL(textChanged(QString)), this, SLOT(fastPayback()));

    //    mcusterItem->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("序号")));
    mcusterItem->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("帐号")));
    mcusterItem->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("名称")));
    mcusterItem->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("流水")));
    mcusterItem->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("总退水")));
    mcusterItem->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("已退水")));
    mcusterItem->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("应退水")));

    mRsultTAbleView = ui->tableView;
    mRsultTAbleView->setShowGrid(true);
    mRsultTAbleView->setMouseTracking(true);
    mRsultTAbleView->setModel(mcusterItem);
    mRsultTAbleView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    mRsultTAbleView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mRsultTAbleView->setSortingEnabled(true);

    //setCentralWidget(ui->grid_root);
//    setCentralWidget(ui->centralWidget);
//    ui->centralWidget->setLayout(ui->gridLayout_3);
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
                t_custerStruct.paybackInAll = intFloor(t_custerStruct.journal * journalpercet);
                mTotlePayback += t_custerStruct.paybackInAll;
            }else {
                t_custerStruct.paybackInAll = 0;
            }

            t_custerStruct.playbackPaid = getLastNumFromString(t_custerStruct.type);
            t_custerStruct.playbackPaying = t_custerStruct.paybackInAll - t_custerStruct.playbackPaid;

            //if (t_custerStruct.paybackInAll){
                mCusterList.append(t_custerStruct);
            //}
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
                t_custerStruct.paybackInAll = intFloor(t_custerStruct.journal * journalpercet);
                mTotlePayback += t_custerStruct.paybackInAll;
            }else {
                t_custerStruct.paybackInAll = 0;
            }

            t_custerStruct.playbackPaid = getLastNumFromString(t_custerStruct.type);
            t_custerStruct.playbackPaying = t_custerStruct.paybackInAll - t_custerStruct.playbackPaid;

           // if (t_custerStruct.paybackInAll){
                mCusterList.append(t_custerStruct);
          //  }
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
    qDebug() << "detect platform : " << mCurrentPlatform <<endl;
}

double MainWindow::getLastNumFromString(QString rawString)
{
    //qDebug() << "rawString: " << rawString;
    if (!rawString.startsWith("d", Qt::CaseInsensitive)
            && !rawString.startsWith("c", Qt::CaseInsensitive)
            && !rawString.startsWith("a", Qt::CaseInsensitive)) {
      //  qDebug() << "error rawString" ;
        return 0;
    }

    int startPos = 0;
    for(int i = rawString.size() - 1; i >= 0; i--){
        QChar t_char = rawString.at(i);
        if(!t_char.isDigit()){
            startPos = i;
            break;
        }
    };
    startPos++;
    QString t_value = rawString.remove(0, startPos);

    //qDebug() <<  "." << t_value.toDouble();
    return t_value.toDouble();
}

void MainWindow::progress()
{
//    qDebug("..on progress");
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

    for (int i = 0; i < mCusterList.size(); i++)
    {
        mcusterItem->setItem(i, 0, new QStandardItem(mCusterList.at(i).name));
        mcusterItem->setItem(i, 1, new QStandardItem(mCusterList.at(i).type));
        mcusterItem->setItem(i, 2, new QStandardItem(QString::number(mCusterList.at(i).journal)));
        mcusterItem->setItem(i, 3, new QStandardItem(QString::number(mCusterList.at(i).paybackInAll)));
        mcusterItem->setItem(i, 4, new QStandardItem(QString::number(mCusterList.at(i).playbackPaid)));
        mcusterItem->setItem(i, 5, new QStandardItem(QString::number(mCusterList.at(i).playbackPaying)));
    }

    //title
    mTableTime.clear();
    mTableTime.append("|||||||平台: ");

    if (mCurrentPlatform == PLATFORM_XINSHIJI){
        mTableTime.append("新世纪");
    }else if (mCurrentPlatform == PLATFORM_BAOXUAN){
         mTableTime.append("宝轩");
    }
    mTableTime.append("   总退水: ");
    //(intFloor(mTotlePayback)), 10
    QString allPackback  = QString::number(intFloor(mTotlePayback), 10);
    mTableTime.append(allPackback);
    ui->label_info->setText(mTableTime);
}

void MainWindow::clear()
{
    for (int i = 0; i < mCusterList.length(); i++)
    {
        mcusterItem->removeRow(0);
    }

    ui->plainsrcTextEdit->clear();
    mTableTime.clear();
    ui->label_info->setText(mTableTime);

    mCusterList.clear();
    mTotlePayback = 0;
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


