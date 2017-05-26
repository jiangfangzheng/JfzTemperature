#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QtCore>
#include <QtNetwork>

QString FileDirName;//文件夹名，全局
int SaveDataFlag=0;

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSeialPort(); //串口初始化
    //标签地址初始化，方便后续操作
    QLabelAll[ 0] = ui->label_2;
    QLabelAll[ 1] = ui->label_3;
    QLabelAll[ 2] = ui->label_4;
    QLabelAll[ 3] = ui->label_5;
    QLabelAll[ 4] = ui->label_6;
    QLabelAll[ 5] = ui->label_7;
    QLabelAll[ 6] = ui->label_8;
    QLabelAll[ 7] = ui->label_9;
    QLabelAll[ 8] = ui->label_10;
    QLabelAll[ 9] = ui->label_11;
    QLabelAll[10] = ui->label_12;
    QLabelAll[11] = ui->label_13;
    QLabelAll[12] = ui->label_14;
    QLabelAll[13] = ui->label_15;
    QLabelAll[14] = ui->label_16;
    QLabelAll[15] = ui->label_17;
    QLabelAll[16] = ui->label_18;
    QLabelAll[17] = ui->label_19;
    QLabelAll[18] = ui->label_20;
    QLabelAll[19] = ui->label_21;
    QLabelAll[20] = ui->label_22;
    QLabelAll[21] = ui->label_23;
    QLabelAll[22] = ui->label_24;
    QLabelAll[23] = ui->label_25;
    QLabelAll[24] = ui->label_26;
    QLabelAll[25] = ui->label_27;
    QLabelAll[26] = ui->label_28;
    QLabelAll[27] = ui->label_29;
    QLabelAll[28] = ui->label_30;
    QLabelAll[29] = ui->label_31;
    QLabelAll[30] = ui->label_32;
    QLabelAll[31] = ui->label_33;
    QLabelAll[32] = ui->label_34;
    QLabelAll[33] = ui->label_35;
    QLabelAll[34] = ui->label_36;
    QLabelAll[35] = ui->label_37;
    QLabelAll[36] = ui->label_38;
    QLabelAll[37] = ui->label_39;
    QLabelAll[38] = ui->label_40;
    QLabelAll[39] = ui->label_41;
    QLabelAll[40] = ui->label_42;
    QLabelAll[41] = ui->label_43;
    QLabelAll[42] = ui->label_44;
    QLabelAll[43] = ui->label_45;
    QLabelAll[44] = ui->label_46;
    QLabelAll[45] = ui->label_47;
    QLabelAll[46] = ui->label_48;
    QLabelAll[47] = ui->label_49;
    QLabelAll[48] = ui->label_50;
    QLabelAll[49] = ui->label_51;
    QLabelAll[50] = ui->label_52;
    QLabelAll[51] = ui->label_53;
    QLabelAll[52] = ui->label_54;
    QLabelAll[53] = ui->label_55;
    QLabelAll[54] = ui->label_56;
    QLabelAll[55] = ui->label_57;
    QLabelAll[56] = ui->label_58;
    QLabelAll[57] = ui->label_59;
    QLabelAll[58] = ui->label_60;
    QLabelAll[59] = ui->label_61;
    QLabelAll[60] = ui->label_62;
    QLabelAll[61] = ui->label_63;
    QLabelAll[62] = ui->label_64;
    QLabelAll[63] = ui->label_65;
    for(int i=0;i<64;i++)
    {
        QLabelAll[i]->setText("");
    }

    //定时器相关
    ui->label_time->setText("poi蒋");
    QTimer *timer = new QTimer(this);//新建定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));//关联定时器计满信号和相应的槽函数。
    timer->start(1000); //定时器开始计时，其中1000表示1000ms即1s,每1000ms就会触发一次槽函数。

    //定时器相关
    QTimer *timeSaveCSV = new QTimer(this);
    connect(timeSaveCSV,SIGNAL(timeout()),this,SLOT(JfzSaveCSV()));
    timeSaveCSV->start(3000);
}

MainWindow::~MainWindow()
{
    delete ui;
    serial.close();  //串口关闭
}

void MainWindow::initSeialPort()
{
    connect(&serial,SIGNAL(readyRead()),this,SLOT(serialRead()));   //连接槽
    //get name for choose
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        ui->comboBox->addItem("无效");
        return;
    }
    ui->comboBox->addItem("关闭");
    foreach (QSerialPortInfo info, infos)
    {
        ui->comboBox->addItem(info.portName());
    }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSerialPortInfo info;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    int i = 0;
    foreach (info, infos) {
        if(info.portName() == arg1) break;
        i++;
    }
    if(i != infos.size ()){//can find
        ui->label->setText("[已开启]");
        serial.close();
        serial.setPort(info);
        serial.open(QIODevice::ReadWrite);         //读写打开
    }
    else
    {
        serial.close();
        ui->label->setText("[关闭]");
    }
}

//格式解析写在此函数中
void MainWindow::serialRead()
{
    QString JfzSerialData;
    JfzSerialData=serial.readAll();
    //收到一大堆数据后分开处理
    QStringList AllDataSplit = JfzSerialData.split(QRegExp("[S*/^]"));//逗号分开存数组
    for(int i=1;i<AllDataSplit.size();i++)
    {
        QString SerialData_one=AllDataSplit.at(i);
        //对每一串数据处理
        SerialData_one.remove("S");//删掉S符号
        SerialData_one.replace(QString("\n"), QString(""));//将所有\n去掉
        QStringList DataSections = SerialData_one.split(QRegExp("[,*/^]"));//逗号分开存数组
        bool OK;
        QString LabelShow_pre_temp;
        switch(DataSections.at(0).toInt(&OK,10))
        {
        case 0:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)]->setText(LabelShow_pre_temp);
            break;
        case 1:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)+8]->setText(LabelShow_pre_temp);
            break;
        case 2:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)+16]->setText(LabelShow_pre_temp);
            break;
        case 3:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)+24]->setText(LabelShow_pre_temp);
            break;
        case 4:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)+32]->setText(LabelShow_pre_temp);
            break;
        case 5:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)+40]->setText(LabelShow_pre_temp);
            break;
        case 6:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)+48]->setText(LabelShow_pre_temp);
            break;
        case 7:
            LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
            QLabelAll[DataSections.at(1).toInt(&OK,10)+56]->setText(LabelShow_pre_temp);
            break;
        }
    }
}

void MainWindow::createFolder()
{
    //获取系统时间
    QDateTime current_time = QDateTime::currentDateTime();//获取系统现在的时间
    QString time_str = current_time.toString("yyyy_MM_dd_hh_mm_ss");//设置系统时间显示格式

    QDir *folder = new QDir;    // 实例 QDir
    FileDirName = time_str;// 文件夹名,全局变量

    //判断创建文件夹是否存在
    bool exist = folder->exists(FileDirName);
    if(exist)
    {
        //什么都不提示
    }else
    {
        //创建文件夹
        folder->mkdir(FileDirName);
    }
}

void MainWindow::timerUpDate()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");//设置系统时间显示格式
    ui->label_time->setText(str);//在标签上显示时间
}

void MainWindow::JfzSaveCSV()
{
    QString FirstReceive=TCPReceiveData;
    TCPReceiveData="";
    if(FirstReceive.size()>25 && "S"==FirstReceive.at(0))
       {
           QString JfzTCPData = FirstReceive;
           //收到一大堆数据后分开处理
           QStringList AllDataSplit = JfzTCPData.split(QRegExp("[S*/^]"));//逗号分开存数组
           for(int i=1;i<AllDataSplit.size();i++)
           {
               QString SerialData_one=AllDataSplit.at(i);
               //对每一串数据处理
               SerialData_one.remove("S");//删掉S符号
               SerialData_one.replace(QString("\n"), QString(""));//将所有\n去掉
               QStringList DataSections = SerialData_one.split(QRegExp("[,*/^]"));//逗号分开存数组
               bool OK;
               QString LabelShow_pre_temp;
               switch(DataSections.at(0).toInt(&OK,10))
               {
               case 0:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)]->setText(LabelShow_pre_temp);
                   break;
               case 1:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)+8]->setText(LabelShow_pre_temp);
                   break;
               case 2:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)+16]->setText(LabelShow_pre_temp);
                   break;
               case 3:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)+24]->setText(LabelShow_pre_temp);
                   break;
               case 4:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)+32]->setText(LabelShow_pre_temp);
                   break;
               case 5:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)+40]->setText(LabelShow_pre_temp);
                   break;
               case 6:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)+48]->setText(LabelShow_pre_temp);
                   break;
               case 7:
                   LabelShow_pre_temp=DataSections.at(2)+"  "+DataSections.at(4);
                   QLabelAll[DataSections.at(1).toInt(&OK,10)+56]->setText(LabelShow_pre_temp);
                   break;
               }
           }
       }

    if(1 == SaveDataFlag)
    {
        QFile file(FileDirName+"\\JfzSaveData.csv");//实例 QFile
        file.open(QIODevice::WriteOnly | QIODevice::Append);//存在打开，不存在创建
        QByteArray str;
        //获取系统时间
        QDateTime current_time = QDateTime::currentDateTime();//获取系统现在的时间
        QString time_str = current_time.toString("yyyy-MM-dd hh:mm:ss")+",";//设置系统时间显示格式
        //先保存时间
        str=time_str.toUtf8();
        for(int i=0;i<64;i++)
        {
            QString temp= QLabelAll[i]->text().right(6);
            temp.replace(QString("\r"), QString(""));//将所有\r去掉
            temp.replace(QString("\n"), QString(""));//将所有\n去掉
            str += temp.toUtf8()+","; //写入内容,这里需要转码，否则报错。
        }
        str+="\r\n";
        file.write(str); //写入 QByteArray 格式字符串
        file.close();    //关闭文件
    }
}

void MainWindow::on_pushButton_clicked()
{
    //创建文件夹，存数据
    createFolder();

    //开启保存数据标志位，保存数据
    SaveDataFlag=1;
    ui->pushButton->setEnabled(false);

    QFile file(FileDirName+"\\JfzDataIndex.csv");//实例 QFile
    file.open(QIODevice::WriteOnly | QIODevice::Append);//存在打开，不存在创建
    QByteArray str;
    for(int i=0;i<64;i++)
    {
        QString temp= QLabelAll[i]->text().left(16);
        str += QString::number(i/8, 10)+ ","+ QString::number(i%8, 10) + "," + temp.toUtf8() + "\r\n"; //写入内容,这里需要转码，否则报错。
    }
    file.write(str); //写入 QByteArray 格式字符串
    file.close();    //关闭文件
}

void MainWindow::on_pushButton_TCPS_clicked()
{
    server = new QTcpServer();
    bool ok;
    int port=ui->lineEdit_port->text().toInt(&ok,10);
    server->listen(QHostAddress::Any, port);
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

//TCP接受连接
void MainWindow::acceptConnection()
{
    clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readClient()));
}

//TCP收
void MainWindow::readClient()
{
    TCPReceiveData += clientConnection->readAll();
}
