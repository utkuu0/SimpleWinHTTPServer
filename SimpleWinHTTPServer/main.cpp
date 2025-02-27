#include <iostream>
#include <winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};

    // İstemciden veri al
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        closesocket(clientSocket);
        return;
    }

    std::cout << "Istemciden gelen istek:\n" << buffer << std::endl;

    // HTTP Yanıtı
    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "Content-Length: 51\r\n"
        "\r\n"
        "<html><body><h1>Merhaba Dunya!</h1></body></html>";

    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
    closesocket(clientSocket);
}

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Winsock başlat
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup basarisiz! Hata Kodu: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Sunucu soketi oluştur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Soket olusturulamadi! Hata Kodu: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Sunucu adres bilgilerini ayarla
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Soketi belirtilen porta bağla
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Baglama basarisiz! Hata Kodu: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Dinlemeye başla
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Dinleme basarisiz! Hata Kodu: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Sunucu " << PORT << " portunda dinleniyor...\n";

    while (true) {
        // Yeni istemci bağlantısını kabul et
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Baglanti kabul edilemedi! Hata Kodu: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Bir istemci baglandi!\n";

        // Yeni istemciyi bir thread'e ata
        std::thread(handleClient, clientSocket).detach();
    }

    // Kaynakları temizle
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
