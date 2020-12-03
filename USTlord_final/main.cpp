#include "mainwindow.h"

#include <QApplication>

const QString DataPackage::Content::REQUEST = "request";
const QString DataPackage::Content::ACCEPT="accept";
const QString DataPackage::Content::REJECT="reject";
const QString DataPackage::Content::BE_LANDLORD="be";
const QString DataPackage::Content::DO_NOT_PLAY="noplay";
const QString DataPackage::Content::WIN_GAME="win";
const QString DataPackage::Content::LOSE_GAME="lose";
const QString DataPackage::Content::END_GAME="end";
const QString DataPackage::Content::QUIT="quit";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
