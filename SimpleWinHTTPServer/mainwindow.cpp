#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include <QListWidgetItem>
#include <QtConcurrent>


#pragma comment(lib, "ws2_32.lib")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serverSocket(INVALID_SOCKET), isRunning(false) {
    ui->setupUi(this);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::startServer);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::stopServer);

    ui->btnStop->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startServer() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        QMessageBox::critical(this, "Hata", "WSAStartup başarısız!");
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        QMessageBox::critical(this, "Hata", "Socket oluşturulamadı!");
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        QMessageBox::critical(this, "Hata", "Bind başarısız!");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        QMessageBox::critical(this, "Hata", "Dinleme başarısız!");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    isRunning = true;
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    ui->listClients->addItem("Sunucu başlatıldı: 8080");

    QtConcurrent::run([this]() {
        while (isRunning) {
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
            if (clientSocket == INVALID_SOCKET) {
                continue;
            }

            clients.push_back(clientSocket);
            QString clientInfo = "Yeni istemci: " + QString::number(clientSocket);
            ui->listClients->addItem(clientInfo);

            const char* httpResponse =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 13\r\n"
                "\r\n"
                "Merhaba Dunya";
            send(clientSocket, httpResponse, strlen(httpResponse), 0);
            closesocket(clientSocket);
        }
    });
}

void MainWindow::stopServer() {
    isRunning = false;
    closesocket(serverSocket);
    WSACleanup();

    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->listClients->addItem("Sunucu durduruldu.");
}
