#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
}

Notepad::~Notepad() {
    delete ui;
}

void Notepad::on_actionNew_triggered() {
    currentFile = "New Document";
    QString warning = "Do you want to save the changes made to the document \"" + currentFile + "\"?";
    if(ui->textEdit->toPlainText() != "") {
        int selection = QMessageBox::warning(this,"..", warning, "Don't Save", "Cancel", "Save...");
        switch(selection) {
            case 0: ui->textEdit->setText("");
                break;
            case 1:
                break;
            case 2: Notepad::on_actionSave_As_triggered();
                break;
        }
    }
}

void Notepad::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"..","File not opened.");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::on_actionSave_triggered() {
    QFile file(currentFile);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        Notepad::on_actionSave_As_triggered();
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}

void Notepad::on_actionSave_As_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,"..","File not saved");
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}

void Notepad::on_actionPrint_triggered() {
    //QPrinter printer = QFileDialog::getPrinterName(this, "Save as");
}

void Notepad::on_actionExit_triggered() {
    this->~Notepad();
}

void Notepad::on_actionCut_triggered() {
    ui->textEdit->cut();
}

void Notepad::on_actionCopy_triggered() {
    ui->textEdit->copy();
}

void Notepad::on_actionPaste_triggered() {
    ui->textEdit->paste();
}

void Notepad::on_actionUndo_triggered() {
    ui->textEdit->undo();
}

void Notepad::on_actionRedo_triggered() {
    ui->textEdit->redo();
}
