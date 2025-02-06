#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qDebug() << "Hello, World from Qt Console!";

    return a.exec();
}
