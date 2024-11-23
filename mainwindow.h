#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QTcpSocket* socket ;

    int temperature = 0;
    int humidity = 0;
    bool isOk;

private slots:
    void connectToServer();
    void readMessage();

    void on_connectBtn_clicked();
    void on_pushButton_clicked();
public slots:
    void errorOccurred(QAbstractSocket::SocketError error);
};
#endif // MAINWINDOW_H
