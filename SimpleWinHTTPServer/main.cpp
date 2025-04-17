#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

MainWindow *g_mainWindow = nullptr;

void handleClient(SOCKET clientSocket);
void startServer();

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    g_mainWindow = &w;
    w.show();

    std::thread serverThread(startServer);
    serverThread.detach();

    return a.exec();
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';

        QString clientMsg = QString::fromLocal8Bit(buffer);
        QMetaObject::invokeMethod(g_mainWindow, [=]() {
            g_mainWindow->appendToConsole("Istemciden gelen veri:\n" + clientMsg);
        }, Qt::QueuedConnection);

        std::string httpResponse =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "Content-Length: 51\r\n"
            "\r\n"
            "<html><body><h1>Merhaba Dunya</h1></body></html>";

        send(clientSocket, httpResponse.c_str(), static_cast<int>(httpResponse.size()), 0);
    }
    closesocket(clientSocket);
}

void startServer() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) return;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return;
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) return;

    QMetaObject::invokeMethod(g_mainWindow, []() {
        g_mainWindow->appendToConsole("Sunucu 8080 portunda dinleniyor..");
    }, Qt::QueuedConnection);

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket != INVALID_SOCKET) {
            QMetaObject::invokeMethod(g_mainWindow, []() {
                g_mainWindow->appendToConsole("Bir istemci baglandi.");
            }, Qt::QueuedConnection);
            std::thread(handleClient, clientSocket).detach();
        }
    }
}
