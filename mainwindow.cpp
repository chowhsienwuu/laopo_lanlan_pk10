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
    mTotlePayback = 0;
    journalpercet = 0;
    mcusterItem = new QStandardItemModel();
    mTableTime = "";
    mCurrentPlatform = PLATFORM_UNKONW;
    mHasInited = false;
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->verticalLayout_3);

    connect(ui->plainsrcTextEdit, SIGNAL(textChanged()), this, SLOT(progress()));
    connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clear()));
    connect(ui->fastpayback_src, SIGNAL(textChanged(QString)), this, SLOT(fastPayback022()));
     connect(ui->fastpayback_src_2, SIGNAL(textChanged(QString)), this, SLOT(fastPayback024()));
    connect(mcusterItem, SIGNAL(itemChanged(QStandardItem*)), this,
            SLOT(refreshData(QStandardItem*)));
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
    mRsultTAbleView->setSortingEnabled(true);
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
    if (stringlist.length() > 10)
    {
        T_custerStruct t_custerStruct;
        initCusterStruct(&t_custerStruct);
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
    if (stringlist.length() > 10)
    {
        T_custerStruct t_custerStruct;
        initCusterStruct(&t_custerStruct);
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

            if (t_custerStruct.paybackInAll){
                mCusterList.append(t_custerStruct);
            }
        }
    }
}

void MainWindow::processOneLinenanshengbaoxuan(QString *line)
{
    // ya22266	A六六六	36	10089.0	1052.6	1052.6	0.000	0	0.00	0	0	0	-60.5	-60.5	10089.0	1113.2
    QStringList stringlist =  line->split(QRegularExpression("\\s+"));
    QString t_str;
//    qDebug() << line << stringlist.length()  << " nanshengbaoxuan" ;
    if (stringlist.length() > 10)
    {
        T_custerStruct t_custerStruct;
        initCusterStruct(&t_custerStruct);
        t_custerStruct.type = stringlist.at(1);
        if (t_custerStruct.type.startsWith("d", Qt::CaseInsensitive)
                || t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
        {
            t_custerStruct.name = stringlist.at(0);

            t_str = stringlist.at(3);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.journal = t_str.toDouble();

            t_str = stringlist.at(4);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.winorlse = t_str.toDouble();

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

            if (t_custerStruct.paybackInAll){
                mCusterList.append(t_custerStruct);
            }
        }
    }
}

void MainWindow::processOneLineali(QString *line)
{
    QStringList stringlist =  line->split(QRegularExpression("\\s+"));
    QString t_str;
    //qDebug() << "ali " << stringlist.length() ;
    if (stringlist.length() > 10)
    {
        T_custerStruct t_custerStruct;
        initCusterStruct(&t_custerStruct);
        t_custerStruct.type = stringlist.at(1);
        if (t_custerStruct.type.startsWith("d", Qt::CaseInsensitive)
                || t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
        {
            t_custerStruct.name = stringlist.at(0);

            t_str = stringlist.at(3);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.journal = t_str.toDouble();

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

            if (t_custerStruct.paybackInAll){
                mCusterList.append(t_custerStruct);
            }
        }
    }
}

void MainWindow::initCusterStruct(T_custerStruct *cus)
{
    if (cus == NULL){
        return;
    }
    cus->index = 0;
    cus->journal = 0;
    cus->name = "";
    cus->paybackInAll = 0;
    cus->playbackPaid = 0;
    cus->playbackPaying = 0;
    cus->type = "";
    cus->winorlse = 0;
}

void MainWindow::processOneLinetianheguoji(QString *line)
{
    QStringList stringlist =  line->split(QRegularExpression("\\s+"));
    QString t_str;
    //qDebug() << "ali " << stringlist.length() ;
    if (stringlist.length() > 10)
    {
        T_custerStruct t_custerStruct;
        initCusterStruct(&t_custerStruct);
        t_custerStruct.type = stringlist.at(2);
        if (t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
        {
            t_custerStruct.name = stringlist.at(0);
            t_str = stringlist.at(4);
            t_str = t_str.remove(QChar(','));
            t_custerStruct.journal = t_str.toDouble();

            if (t_custerStruct.type.startsWith("c", Qt::CaseInsensitive))
            {
                t_custerStruct.paybackInAll = intFloor(t_custerStruct.journal * journalpercet);
                mTotlePayback += t_custerStruct.paybackInAll;
            }else {
                t_custerStruct.paybackInAll = 0;
            }

            if (t_custerStruct.paybackInAll){
                mCusterList.append(t_custerStruct);
            }
        }
    }
}
void MainWindow::processOneLinecalSum(QString *line)
{

    QStringList stringlist =  line->split(QRegularExpression("\\s+"));
    QString t_str;
   // qDebug() << " processOneLinecalSum" << *line << stringlist.length();
    if (stringlist.length() < 10 && stringlist.length() > 7)
    {
        T_custerStruct t_custerStruct;
        initCusterStruct(&t_custerStruct);
        t_custerStruct.paybackInAll = 0;
        t_custerStruct.playbackPaid = 0;
        t_custerStruct.playbackPaying = 0;
        t_custerStruct.name = stringlist.at(2);

        t_str = stringlist.at(4);
        t_str = t_str.remove(QChar(','));
        t_custerStruct.journal = t_str.toDouble();

        t_custerStruct.journal = intFloor(t_custerStruct.journal);
        mTotlePayback += t_custerStruct.journal;
        if (t_custerStruct.journal){
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
     //   qDebug() << line << " " << line.isEmpty();
        if (line.length() > 10){
            while (line.at(0).isSpace() && line.length() > 2){
                line.remove(0, 1);
            }
        }
        QStringList stringlist =  line.split(QRegularExpression("\\s+"));
       // qDebug() << "detectPlatFrom line" << stringlist.length();
        QString t_str;

        //tianheguoji
        if(stringlist.length() > 15){
            t_str = stringlist.at(2);
            QString temp = stringlist.at(1);
            if ((t_str.startsWith("d", Qt::CaseInsensitive)
                    || t_str.startsWith("c", Qt::CaseInsensitive)
                    || t_str.startsWith("a", Qt::CaseInsensitive))
                    && temp.contains("直属会员"))
            {
                mCurrentPlatform = PLATFORM_TIANHEGUOJI;
                break;
            }
        }

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
        //nanshen baoxuan ali
        QString t_str06;
        if(stringlist.length() == 16 || stringlist.length() == 17){
            t_str = stringlist.at(1);
            t_str06 = stringlist.at(6);
            if (t_str.startsWith("d", Qt::CaseInsensitive)
                    || t_str.startsWith("c", Qt::CaseInsensitive)
                    || t_str.startsWith("a", Qt::CaseInsensitive))
            {
              // qDebug()<< "the t_str06 " << t_str06 ;
                if (t_str06.endsWith("%", Qt::CaseInsensitive)){
                    mCurrentPlatform = PLATFORM_ALI;
                    break;
                }else{
                    mCurrentPlatform = PLATFORM_NANSHENGBAOXUAN;
                    break;
                }
            }
        }
        //CAL SUM ONLY.
        if(stringlist.length() == 10 || stringlist.length() == 9){
            t_str = stringlist.at(3);
            if (t_str.startsWith("d", Qt::CaseInsensitive)
                    || t_str.startsWith("c", Qt::CaseInsensitive)
                    || t_str.startsWith("a", Qt::CaseInsensitive))
            {
                mCurrentPlatform = PLATFORM_CALSUM;
                break;
            }
        }

    }
    if (mCurrentPlatform == PLATFORM_NANSHENGBAOXUAN || mCurrentPlatform == PLATFORM_XINSHIJI ||
            mCurrentPlatform == PLATFORM_BAOXUAN ){
        journalpercet = 0.024;
    }else if (mCurrentPlatform == PLATFORM_ALI || mCurrentPlatform == PLATFORM_TIANHEGUOJI){
        journalpercet = 0.022;
    }else if (mCurrentPlatform == PLATFORM_CALSUM){
        journalpercet = 0.;
    }

    qDebug() << "detect platform : " << mCurrentPlatform << " journalpercet : " << journalpercet << endl;
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

void MainWindow::calSum()
{
    mTableTime.clear();
    mTableTime.append(" *平台: ");

    if (mCurrentPlatform == PLATFORM_XINSHIJI){
        mTableTime.append("新世纪");
    }else if (mCurrentPlatform == PLATFORM_BAOXUAN){
        mTableTime.append("宝轩");
    }else if (mCurrentPlatform == PLATFORM_NANSHENGBAOXUAN){
         mTableTime.append("宝轩/南升");
    }else if (mCurrentPlatform == PLATFORM_ALI){
        mTableTime.append("阿里");
    }else if (mCurrentPlatform == PLATFORM_TIANHEGUOJI){
        mTableTime.append("天和国际");
    }

    mTableTime.append(" 退水比例: ");
    mTableTime.append(QString::number(journalpercet));
    mTableTime.append(" 总退水: ");
    QString allPackback  = QString::number(intFloor(mTotlePayback), 10);
    mTableTime.append(allPackback);
    mTableTime.append("  昨天退水: ");
    int t_paed = 0;
    int t_paying = 0;
    for (int i = 0; i < mCusterList.size(); i++){
        QStandardItem *t_itempaed = mcusterItem->item(i, 4);
        t_paed += t_itempaed->data(Qt::EditRole).toString().toInt();

        QStandardItem *t_itempaying = mcusterItem->item(i, 5);
        t_paying += t_itempaying->data(Qt::EditRole).toString().toInt();
    }
    mTableTime.append(QString::number(t_paed, 10));
            mTableTime.append("  今天退水: ");
    mTableTime.append(QString::number(t_paying, 10));

    if (mCurrentPlatform == PLATFORM_CALSUM){
        mTableTime.clear();
        mTableTime.append("总数  :");
        mTableTime.append(QString::number(intFloor(mTotlePayback), 10));
    }

    ui->label_info->setText(mTableTime);
}

void MainWindow::progress()
{
//    qDebug("..on progress");  
    //
    for (int i = 0; i < mCusterList.length(); i++)
    {
        mcusterItem->removeRow(0);
    }
    mTableTime.clear();
    ui->label_info->setText(mTableTime);

    mCusterList.clear();
    mTotlePayback = 0;
    mHasInited = false;
    journalpercet = 0;
    mCurrentPlatform = PLATFORM_UNKONW;
    //
    QString text = ui->plainsrcTextEdit->toPlainText();
    detectPlatFrom(text);

    QTextStream str(&text, QIODevice::ReadOnly);
    QString line;
    while(!str.atEnd())
    {
        line = str.readLine();
        //qDebug() << line << "befor" ;
        if (line.length() > 10){
            while (line.at(0).isSpace() && line.length() > 2){
                line.remove(0, 1);
            }
        }
        line = line.trimmed();
         // qDebug() << line << "end" ;
        if (mCurrentPlatform == PLATFORM_XINSHIJI){
            processOneLineXinshiji(&line);
        }else if (mCurrentPlatform == PLATFORM_BAOXUAN){
            processOneLinebaoxuan(&line);
        }else if (mCurrentPlatform == PLATFORM_NANSHENGBAOXUAN){
            processOneLinenanshengbaoxuan(&line);
        }else if (mCurrentPlatform == PLATFORM_ALI){
          //   qDebug() << line << "in" ;
            processOneLineali(&line);
        }else if (mCurrentPlatform == PLATFORM_CALSUM){
            processOneLinecalSum(&line);
        }else if (mCurrentPlatform == PLATFORM_TIANHEGUOJI){
            processOneLinetianheguoji(&line);
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

        mcusterItem->item(i, 0)->setEditable(false);
//         mcusterItem->item(i, 0)->setEditable(false);
          mcusterItem->item(i, 2)->setEditable(false);
    }
    mHasInited = true;
    calSum();
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
    mHasInited = false;
    journalpercet = 0;
    mCurrentPlatform = PLATFORM_UNKONW;
}

void MainWindow::fastPayback022()
{
    QString t_str = ui->fastpayback_src->text();

    t_str = t_str.remove(QChar(','));
    double journal = t_str.toDouble(); //
    journal *= 0.022;
    int palybak = intFloor(journal);
    QString show = "退水: " +   QString::number(palybak);

    ui->fastPaybacklable->setText(show);
}

void MainWindow::fastPayback024()
{
    QString t_str = ui->fastpayback_src_2->text();

    t_str = t_str.remove(QChar(','));
    double journal = t_str.toDouble(); //
    journal *= 0.024;
    int palybak = intFloor(journal);
    QString show = "退水: " +   QString::number(palybak);

    ui->fastPaybacklable_2->setText(show);
}


void MainWindow::refreshData(QStandardItem* item)
{
    if (!mHasInited){
        return; //system init table view just return;
    }

    if (item->column() != 4 && item->column() != 1){
        return; //just payed..
    }

    QStandardItem *t_item1 = mcusterItem->item(item->row(),  item->column() - 1);

    double t_payInall = t_item1->data(Qt::EditRole).toString().toDouble();

    double t_changePayed = item->data(Qt::EditRole).toString().toDouble();
    double t_changePaying = t_payInall - t_changePayed;
    t_changePaying = t_changePaying > 0L ? t_changePaying : 0L;
   // qDebug() << "..chage to " << t_changePaying;
    mcusterItem->setItem(item->row(), item->column() + 1, new QStandardItem(QString::number(intFloor(t_changePaying))));
    calSum();
}


