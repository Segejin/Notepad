#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow {
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    int on_actionSave_As_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    bool fileExists(QString path);

private:
    Ui::Notepad *ui;
    QString currentFile;
    QString warning;
};

#endif // NOTEPAD_H
