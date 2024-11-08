#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <iostream>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    if(connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readMEssage)) {
        ui->label->setText("Connect to server!" );
    }
    else {
        ui->label->setText("Error: not connect to server!" );
    }
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
    ui->messageLabel->setText(msg);
    qDebug() << msg;

}




void MainWindow::on_connectBtn_clicked()
{
    connectToServer();

}


void MainWindow::on_pushButton_clicked()
{
    socket->disconnectFromHost();
    ui->label->setText("Disconnect to server!");
}

