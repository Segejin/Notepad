#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) : QMainWindow(parent), ui(new Ui::Notepad) {
    ui->setupUi(this);
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
    QObject::connect(shortcut, &QShortcut::activated, this, &Notepad::on_actionFind_triggered);
    this->setCentralWidget(ui->textEdit);
    this->saved = false;
    this->currentFile = Notepad::setDefaultFile();
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    ui->textEdit->setFont(font);
    highlighter = new Highlighter(ui->textEdit->document());
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
                saved = false;
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
    out.flush();
    file.close();
}

int Notepad::on_actionSave_As_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), Notepad::setDefaultFile(), tr("Text files (*.txt)"));
    if(!fileName.isEmpty()) {
        QFile file(fileName);
        currentFile = fileName;
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "File not saved");
            return 0;
        }
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        out.flush();
        file.close();
        saved = true;
        return 1;
    }
    return 0;
}

void Notepad::on_actionPrint_triggered() {
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (ui->textEdit->textCursor().hasSelection()) {
        dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    }
    if (dialog->exec() != QDialog::Accepted) {
        return;
    }
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

void Notepad::on_actionFind_triggered() {
    bool ok;
    bool found = false;
    int count = 0;
    QTextDocument *document = ui->textEdit->document();
    QString regexp = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("Find: "), QLineEdit::Normal, "", &ok);
    QString notFound = "Cannot find \"" + regexp + "\"";
    if (ok && !regexp.isEmpty()) {
        QRegularExpression re(regexp);
        QString dataText = ui->textEdit->toPlainText();
        QTextCursor highlightCursor(document);
        QTextCursor cursor(document);
        cursor.beginEditBlock();
        QTextCharFormat plainFormat(highlightCursor.charFormat());
        QTextCharFormat colorFormat = plainFormat;
        colorFormat.setBackground(Qt::red);

        while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
            highlightCursor = document->find(regexp, highlightCursor, QTextDocument::FindWholeWords);
            if (!highlightCursor.isNull()) {
                found = true;
                ++count;
                highlightCursor.movePosition(QTextCursor::WordRight, QTextCursor::KeepAnchor);
                highlightCursor.mergeCharFormat(colorFormat);
            }
        }
        cursor.endEditBlock();

        if (!found) {
            QMessageBox::information(this, tr("No Matches"), tr((const char*)(QByteArray)notFound.toLatin1().data()));
        }
    }
}

void Notepad::on_actionReplace_triggered() {
    QDialog dialog(this);
    QFormLayout form(&dialog);
    QList<QLineEdit *> fields;
    QList<QString> terms;
    terms.append("Find");
    terms.append("Replace");
    for(int i = 0; i < terms.length(); ++i) {
        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString("%1").arg(terms.at(i));
        form.addRow(label, lineEdit);
        fields << lineEdit;
    }
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QDialogButtonBox navigationBox(&dialog);
    navigationBox.addButton("Find Previous", QDialogButtonBox::ApplyRole);
    navigationBox.addButton("Find Next", QDialogButtonBox::ApplyRole);
    form.addRow(&buttonBox);
    form.addRow(&navigationBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    if (dialog.exec() == QDialog::Accepted) {
        QString regexp = fields.at(0)->text();
        QString replacementText = fields.at(1)->text();
        if (!regexp.isEmpty()) {
            QRegularExpression re(regexp);
            QString dataText = ui->textEdit->toPlainText();
            QRegularExpressionMatchIterator itr = re.globalMatch(dataText);
            while(itr.hasNext()) {
                QRegularExpressionMatch match = itr.next();
                dataText.replace(match.capturedStart(0), match.capturedLength(0), replacementText);
            }
            ui->textEdit->setText(dataText);
        }
    }
}

QString Notepad::setWarning() {
    this->nameWarning = "Do you want to save \"" + currentFile + "\"?";
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
    QFileInfo checkFile(path + ".txt");
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
