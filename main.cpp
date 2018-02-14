#include "notepad.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Notepad notepad;
    notepad.show();

    return application.exec();
}
