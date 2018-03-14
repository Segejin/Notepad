#include "notepad.h"
#include "highlighter.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Notepad notepad;
    notepad.show();

    return application.exec();
}
