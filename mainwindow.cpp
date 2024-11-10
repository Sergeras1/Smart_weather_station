#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>
#include <QPixmap>
#include <iostream>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readMEssage);
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this, SLOT(errorOccurred(QAbstractSocket::SocketError)));

    connectToServer();

    QPixmap imgTemp("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-temperature-3923489.png");
    QPixmap imgHum("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-humidity-15893504.png");
    QPixmap imgPres("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-thermometer-3525808.png");

    ui->labelImageTemp->setPixmap(imgTemp);
    ui->labelImageHum->setPixmap(imgHum);
    ui->labelImagePres->setPixmap(imgPres);

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::connectToServer()
{
    socket->connectToHost(QHostAddress("192.168.1.153"), 1111);

}

void MainWindow::readMEssage()
{
    if (socket->bytesAvailable() < sizeof(int)) {
        return;
    }

    int msg_size;
    socket->read(reinterpret_cast<char*>(&msg_size), sizeof(int));

    if (socket->bytesAvailable() < msg_size) {
        return;
    }

    // Читаем само сообщение
    QByteArray msg = socket->read(msg_size);
    ui->labelTemp->setText(msg);
    ui->infoMes->setText("Connect to server!");
    qDebug() << msg;

}




void MainWindow::on_connectBtn_clicked()
{

    ui->infoMes->setText("Connect to server!" );

}


void MainWindow::on_pushButton_clicked()
{
    socket->disconnectFromHost();
    ui->infoMes->setText("Disconnect to server!");
}

void MainWindow::errorOccurred(QAbstractSocket::SocketError error)
{
    QString errMsg = socket->errorString();
    ui->infoMes->setText(errMsg);
}

