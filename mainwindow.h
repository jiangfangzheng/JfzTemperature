#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QLabel  *QLabelAll[64];
    void createFolder();
    //串口相关
    QSerialPort serial;   //串口及其操作函数
    void initSeialPort(); //用于初始化
    //TCP服务器相关
    QTcpServer *server;
    QTcpSocket *clientConnection;
    QString TCPReceiveData;
private slots:
    void serialRead();    //用于接收串口过来的数据
    void timerUpDate();   //定时器时间更新
    void JfzSaveCSV();    //定时保存文件到csv

    void on_pushButton_clicked();
    void on_pushButton_TCPS_clicked();
    void acceptConnection();//TCP
    void readClient();//TCP
};

#endif // MAINWINDOW_H
