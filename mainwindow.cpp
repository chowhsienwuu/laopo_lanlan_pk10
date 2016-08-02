#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "QDebug"
#include <QDoubleSpinBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    mTotlePayback = 0;
    mTableTime = "";
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

void MainWindow::processOneLine(QString *line)
{
    if (line->startsWith("会员"))
        //会员    n6888    c    40    908.0    908.0    170.7
    {
        QStringList stringlist =  line->split(QRegularExpression("\\s+"));
//        for (int j = 0; j < stringlist.length(); j++)
//        {
//            qDebug() << " " << j << " : " << stringlist.at(j);
//        }
        QString t_str;
        T_custerStruct t_custerStruct;

        t_custerStruct.name = stringlist.at(1); //n6888
        t_custerStruct.type = stringlist.at(2); //c

        t_str = stringlist.at(5);
        t_str = t_str.remove(QChar(','));
        t_custerStruct.journal = t_str.toDouble(); //908.0

        t_str = stringlist.at(6);
        t_str = t_str.remove(QChar(','));
        t_custerStruct.winorlse = t_str.toDouble(); //170.7

        if (t_custerStruct.type.startsWith("c"))
        {
            t_custerStruct.payback = t_custerStruct.journal * 0.022;
            mTotlePayback += t_custerStruct.payback;
        }else {
            t_custerStruct.payback = 0;
        }

        mCusterList.append(t_custerStruct);
    }

    if (line->contains("交收报表"))
    {
        mTableTime = *line;
        mTableTime.remove("返回");
    }
    if (line->startsWith("总计"))
    {
        QStringList stringlist =  line->split(QRegularExpression("\\s+"));
//        for (int j = 0; j < stringlist.length(); j++)
////        {
////            qDebug() << " " << j << " : " << stringlist.at(j);
////        }
        QString t_str;
        T_custerStruct t_custerStruct;

        t_custerStruct.name = "";
        t_custerStruct.type = "总和"; //c

        t_str = stringlist.at(4);
        t_str = t_str.remove(QChar(','));
        t_custerStruct.journal = t_str.toDouble();

        t_str = stringlist.at(5);
        t_str = t_str.remove(QChar(','));
        t_custerStruct.winorlse = t_str.toDouble();

        t_custerStruct.payback = mTotlePayback;

        mCusterList.append(t_custerStruct);
    }

}

void MainWindow::progress()
{
//    qDebug("..on progress");
//    mcusterItem->clear();
    mCusterList.clear();
    mTotlePayback = 0;
    QString text = ui->plainsrcTextEdit->toPlainText();

    QTextStream str(&text, QIODevice::ReadOnly);
    QString line;
    while(!str.atEnd())
    {
        line = str.readLine();
        //qDebug()<<"--" << line ;
        processOneLine(&line);
        //qDebug("-------");
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
    journal *= 0.022;
    QString show = "退水: " +   QString::number(journal);

    ui->fastPaybacklable->setText(show);
}


