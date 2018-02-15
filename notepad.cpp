#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) : QMainWindow(parent), ui(new Ui::Notepad) {
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    this->saved = false;
    this->currentFile = Notepad::setDefaultFile();
}

Notepad::~Notepad() {
    delete ui;
}

void Notepad::on_actionNew_triggered() {
    if(ui->textEdit->toPlainText() != "") {
        int selection = QMessageBox::warning(this,"..", Notepad::setWarning(), "Don't Save", "Cancel", "Save...");
        switch(selection) {
            case 0:
                ui->textEdit->setText("");
                Notepad::setDefaultFile();
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
    if(!fileName.isEmpty()) {
        QFile file(fileName);
        currentFile = fileName;
        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "File not opened.");
            return;
        }
        QTextStream in(&file);
        ui->textEdit->setText(in.readAll());
        file.close();
        return;
    }
    return;
}

void Notepad::on_actionSave_triggered() {
    if(!saved) {
        Notepad::on_actionSave_As_triggered();
        return;
    }
    QFile file(currentFile);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "File not saved");
        return;
    }
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
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
        out << ui->textEdit->toPlainText();
        file.flush();
        file.close();
        saved = true;
        return 1;
    }
    return 0;
}

void Notepad::on_actionPrint_triggered() {
    //QPrinter printer = QFileDialog::getPrinterName(this, "Save as");
}

void Notepad::on_actionExit_triggered() {
    if(ui->textEdit->toPlainText() != "") {
        int selection = QMessageBox::warning(this,"..", setWarning(), "Don't Save", "Cancel", "Save...");
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

QString Notepad::setWarning() {
    this->nameWarning = "Do you want to save the changes made to the document \"" + currentFile + "\"?";
    return(nameWarning);
}

QString Notepad::setDefaultFile() {
    QString defaultName = "Untitled";
    int digits;
    while(Notepad::fileExists(defaultName)) {
        QByteArray data = defaultName.toUtf8();
        if(QChar::isDigit(data.at(data.length() - 1))) {
            int i = static_cast<int>(data.at(data.length() - 1)) - 48;
            digits = Notepad::numDigits(i);
            data.replace(data.length() - digits, digits, QByteArray::number(i + 1));
            defaultName = QString::fromUtf8(data.data());
        } else {
            defaultName.append('1');
        }
    }
    return defaultName;
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
