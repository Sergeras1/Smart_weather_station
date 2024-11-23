#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readMessage);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::errorOccurred);

    connectToServer();

    QPixmap imgTemp("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-temperature-3923489.png");
    QPixmap imgHum("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-humidity-15893504.png");

    ui->labelImageTemp->setPixmap(imgTemp);
    ui->labelImageHum->setPixmap(imgHum);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectToServer() {
    socket->connectToHost("192.168.1.153", 1111);
    if (socket->waitForConnected(3000)) {
        ui->infoMes->setText("Connected to server");
    } else {
        ui->infoMes->setText("Connection failed: " + socket->errorString());
    }
}

void MainWindow::readMessage() {
    while (socket->canReadLine()) {
        QString message = QString::fromUtf8(socket->readLine().trimmed());
        qDebug() << "Received message:" << message;

        if (message.startsWith("T:")) {
            bool isOk;
            temperature = message.mid(2).toFloat(&isOk);
            if (isOk) {
                ui->labelTemp->setText(QString::number(temperature, 'f', 1) + " °C");
            }
        } else if (message.startsWith("H:")) {
            bool isOk;
            humidity = message.mid(2).toFloat(&isOk);
            if (isOk) {
                ui->labelHum->setText(QString::number(humidity, 'f', 1) + " %");
            }
        }
    }
}

void MainWindow::on_connectBtn_clicked() {
    connectToServer();
}

void MainWindow::on_pushButton_clicked() {
    socket->disconnectFromHost();
    ui->infoMes->setText("Disconnected from server");
}

void MainWindow::errorOccurred(QAbstractSocket::SocketError error) {
    ui->infoMes->setText("Socket error: " + socket->errorString());
}
