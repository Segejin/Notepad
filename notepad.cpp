#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) : QMainWindow(parent), ui(new Ui::Notepad) {
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    this->currentFile = "New Document1";
    this->warning = "Do you want to save the changes made to the document \"" + currentFile + "\"?";
}

Notepad::~Notepad() {
    delete ui;
}

void Notepad::on_actionNew_triggered() {
    if(ui->textEdit->toPlainText() != "") {
        int selection = QMessageBox::warning(this,"..", warning, "Don't Save", "Cancel", "Save...");
        switch(selection) {
            case 0:
                ui->textEdit->setText("");
                break;
            case 1:
                break;
            case 2:
                if(Notepad::on_actionSave_As_triggered()) {
                    ui->textEdit->setText("");
                }
                break;
        }
    }
}

void Notepad::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "File not opened.");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::on_actionSave_triggered() {
    int digits;
    while(Notepad::fileExists(currentFile)) {
        QByteArray data = currentFile.toUtf8();
        int i = static_cast<int>(data.at(data.length() - 1)) - 48;
        digits = Notepad::numDigits(i);
        data.replace(data.length() - digits, digits, QByteArray::number(i + 1));
        currentFile = QString::fromUtf8(data.data());
    }
    QFile file(currentFile);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "File not saved");
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}

int Notepad::on_actionSave_As_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    if(!fileName.isEmpty()) {
        QFile file(fileName);
        currentFile = fileName;
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "File not saved");
            return 0;
        }
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.flush();
        file.close();
        return 1;
    }
    return 0;
}

void Notepad::on_actionPrint_triggered() {
    //QPrinter printer = QFileDialog::getPrinterName(this, "Save as");
}

void Notepad::on_actionExit_triggered() {
    if(ui->textEdit->toPlainText() != "") {
        int selection = QMessageBox::warning(this,"..", warning, "Don't Save", "Cancel", "Save...");
        switch(selection) {
            case 0:
                QCoreApplication::quit();
            case 1:
                break;
            case 2:
                if(Notepad::on_actionSave_As_triggered()) {
                    QCoreApplication::quit();
                }
                break;
        }
    }
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

bool Notepad::fileExists(QString path) {
    QFileInfo checkFile(path);
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    }
    return false;
}

int Notepad::numDigits(int x) {
    x = abs(x);
    return (x < 10 ? 1 :
        (x < 100 ? 2 :
        (x < 1000 ? 3 :
        (x < 10000 ? 4 :
        (x < 100000 ? 5 :
        (x < 1000000 ? 6 :
        (x < 10000000 ? 7 :
        (x < 100000000 ? 8 :
        (x < 1000000000 ? 9 :
        10)))))))));
}
