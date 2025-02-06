#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startServer();
    void stopServer();

private:
    Ui::MainWindow *ui;
    SOCKET serverSocket;
    bool isRunning;
    std::vector<SOCKET> clients;
};

#endif // MAINWINDOW_H
