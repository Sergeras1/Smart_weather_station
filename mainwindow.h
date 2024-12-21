#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "panelsettings.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class PanelSettings;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    PanelSettings *panelSet; // Объект для виджета
    QTcpSocket* socket;
    QString getIPServer() const;
private:
    Ui::MainWindow *ui;

    int temperature = 0;
    int humidity = 0;
    bool isOk;
    QString IPHost = "192.168.1.153";  // IP adress server
    double xBegin, xEnd, h, X;
    int N;
    QVector<double> x, y;
private slots:
    void readMessage();
    void on_btnSettings_clicked();

    void showMainMenu();
    void showWeatherStation();
    void showGraphs();
    void showLampaWidget();
    void showSettings();


public slots:
    void errorOccurred(QAbstractSocket::SocketError error);
    void connectToServer();
};
#endif // MAINWINDOW_H
