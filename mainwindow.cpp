#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connections
    QObject::connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(openDlg()));
    QObject::connect(ui->doButton, SIGNAL(clicked()), this, SLOT(doTheJob()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDlg()
{
    QString file = QFileDialog::getOpenFileName(this, "Choose the file", QString(), "CubeBlocks (CubeBlocks.sbc)");
    ui->filePath->setText(file);
}

void MainWindow::doTheJob()
{
    bool errorDivider, errorPath = false;

    // Check if the number in divider is correct
    foreach (QChar c, ui->divider->text()) {
        if (!c.isNumber()) {
            if (c != QChar('.')) {
                errorDivider = true;
            }
        }
    }
    if (ui->divider->text().isEmpty()) {
        errorDivider = true;
    }
    if (errorDivider) {
        QMessageBox::warning(this, "Divider error", "The number you entered is incorrect.");
    }

    // Check if file path is correct
    if (!QFile::exists(ui->filePath->text())) {
        errorPath = true;
    }
    if (errorPath) {
        QMessageBox::warning(this, "File path error", "The file path is incorrect.");
    }

    // If no error, do the job
    if (!errorDivider && !errorPath) {
        float divider = ui->divider->text().toFloat();
        QString filePath(ui->filePath->text());

        QFile inputFile(filePath);

        if(inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream flux(&inputFile);
            QString text;

            QString newFileContent;

            //int lineNumber = inputFile.readAll().count();

            while(!flux.atEnd())
            {
                text = flux.readLine();

                // Check if it's the correct key
                if (text.contains("<BuildTimeSeconds>", Qt::CaseInsensitive)) {
                    //int spaces = text.indexOf("<") - 1;
                    int positionStart = text.indexOf(">") + 1;
                    int positionEnd = text.indexOf("</") - 1;

                    QString buildTimeStr;

                    for (int i = positionStart; i <= positionEnd; i++) {
                        buildTimeStr += text[i];
                    }

                    float newBuildTime = (buildTimeStr.toFloat()) / divider;

                    QString newLine;

                    // Add spaces
                    for (int i = 0; i < positionStart; i++) {
                        newLine += " ";
                    }

                    // Construct new line
                    newLine += "<BuildTimeSeconds>" + QString::number(newBuildTime, 'f', 2) + "</BuildTimeSeconds>";

                    newFileContent += newLine + '\n';
                    //ui->resultText->appendPlainText(newLine);

                } else {
                    newFileContent += text + '\n';
                    //ui->resultText->appendPlainText(text);
                }

            }
            inputFile.close();

            QString savePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/tmp/text.sbc", tr("CubeBlocks (*.sbc)"));

            QFile outputFile(savePath);
            outputFile.open(QIODevice::ReadWrite | QIODevice::Text);

            QTextStream out(&outputFile);

            out.operator <<(newFileContent);

            //outputFile.write(ui->resultText->toPlainText());

            outputFile.close();

        }
    }
}
