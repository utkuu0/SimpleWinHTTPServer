#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("TCP Sunucu - HTTP Yanıt Uygulaması");
    ui->textBrowser->append("🟢 Uygulama başlatıldı...");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendToConsole(const QString &text) {
    ui->textBrowser->append(text);
}
