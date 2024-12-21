#include <QDebug>
#include <QPixmap>
#include <QTcpSocket>
#include <QHBoxLayout>
#include <qwidget.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readMessage);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::errorOccurred);

    connectToServer();

    connect(ui->PB_main_menu_main, &QPushButton::clicked, this, &MainWindow::showMainMenu);
    connect(ui->PB_weather_station_main, &QPushButton::clicked, this, &MainWindow::showWeatherStation);
    connect(ui->PB_graphic_main, &QPushButton::clicked, this, &MainWindow::showGraphs);
    connect(ui->PB_lampa_main, &QPushButton::clicked, this, &MainWindow::showLampaWidget);
    connect(ui->PB_settings_main, &QPushButton::clicked, this, &MainWindow::showSettings);

    ui->icon_main_widget->hide();

    //add icons in widget
    QPixmap imgTemp("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-temperature-3923489.png");
    QPixmap imgHum("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-humidity-15893504.png");
    QPixmap imgPres("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-thermometer-3525808.png");

    ui->btnSettings->setIcon(QIcon("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-settings-1082265.png"));
    ui->btnSettings_2->setIcon(QIcon("C:/Users/sergey/Desktop/Home_Weather_Station/layout/image/icons/free-icon-settings-1082265.png"));
    ui->labelImageTemp->setPixmap(imgTemp);
    ui->labelImageHum->setPixmap(imgHum);
    ui->labelImagePres->setPixmap(imgPres);
//---------------------------------------------------------------------------график---------------------------------------------------------------------//
    h = 0.1;
    xBegin = -3;
    xEnd = -2;
    ui->graphic->xAxis->setRange(0, 60); // Настройка разрешения графика по X
    ui->graphic->yAxis->setRange(0, 50); // Настройка разрешения графика по Y

    for(int i = 0; i <= 50; i++){
        x.push_back(i);
        for(int j = 0; j <= 50; j++) {
            y.push_back(j);
        }
    }

    ui->graphic->addGraph();
    ui->graphic->graph(0)->addData(x, y);
    ui->graphic->replot();
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::getIPServer() const
{
    return IPHost;
}

void MainWindow::connectToServer() {
    //IPHost = "192.168.1.153";
    socket->connectToHost(IPHost, 1111);
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

void MainWindow::errorOccurred(QAbstractSocket::SocketError error) {
    ui->infoMes->setText("Socket error: " + socket->errorString());
}


void MainWindow::on_btnSettings_clicked()
{
    /*panelSet = new PanelSettings(this);
    panelSet->show();*/
}

void MainWindow::showMainMenu()
{
    ui->stackedWidget->setCurrentWidget(ui->SW_Main_menu);
}

void MainWindow::showWeatherStation()
{
    ui->stackedWidget->setCurrentWidget(ui->SW_Weather_station);
}

void MainWindow::showGraphs()
{
    ui->stackedWidget->setCurrentWidget(ui->SW_Graphic);
}


void MainWindow::showLampaWidget()
{
    ui->stackedWidget->setCurrentWidget(ui->SW_Lampa);
}

void MainWindow::showSettings()
{
    ui->stackedWidget->setCurrentWidget(ui->SW_Settings);
}

