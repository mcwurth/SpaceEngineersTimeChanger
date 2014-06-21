/*****************************************************************************
 * SpaceEngineersBuildTimeChanger                                            *
 *                                                                           *
 * Author  : CptRomain                                                       *
 * Contact : cptromain@gmail.com                                             *
 * Version : 1.0                                                             *
 *                                                                           *
 * Description : This program allow the players of Space Engineers to change *
 * the speed of the Welding by changing some values in a XML file in a       *
 * game's directory.                                                         *
 *                                                                           *
 * Copyright (C) SpaceEngineersBuildTimeChanger CptRomain                    *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * a long with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,*
 * USA.                                                                      *
 *****************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

const QString dir = "C:\\Program Files (x86)\\Steam\\SteamApps\\common\\SpaceEngineers\\Content\\Data\\";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connections
    QObject::connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(openDlg()));
    QObject::connect(ui->doButton, SIGNAL(clicked()), this, SLOT(doTheJob()));

    // Set clickable label
    ui->webLabel->setText("<a href='https://github.com/CptRomain/SpaceEngineersTimeChanger'>https://github.com/CptRomain/SpaceEngineersTimeChanger</a>");
    ui->webLabel->setOpenExternalLinks(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDlg()
{
    QString file = QFileDialog::getOpenFileName(this, "Choose the file",dir , "CubeBlocks (CubeBlocks.sbc)");

    ui->filePath->setText(file);
}

void MainWindow::doTheJob()
{
    bool errorDivider = false, errorPath = false;

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

        // Make a backup keeping all backup files
        QString fileBackup = filePath + ".bak";
        int j = 1;
        while (QFile::exists(fileBackup)) {
            fileBackup = filePath + ".bak" + QString::number(j);
            j++;
        }
        QFile::copy(filePath, fileBackup);

        // Start doing the job
        QFile inputFile(filePath);

        if(inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

            QTextStream stream(&inputFile);

            // Vars
            QString text, newFileContent, newLine, buildTimeStr;
            int i, spaces, positionStart, positionEnd;
            float newBuildTime;

            while(!stream.atEnd())
            {
                // Reset vars
                text = stream.readLine();
                newLine.clear();
                buildTimeStr.clear();

                // Check if it's the correct key
                if (text.contains("<BuildTimeSeconds>", Qt::CaseInsensitive)) {

                    spaces = text.indexOf("<");
                    positionStart = text.indexOf(">") + 1;
                    positionEnd = text.indexOf("</") - 1;

                    // Get the time writed in the file
                    for (i = positionStart; i <= positionEnd; i++) {
                        buildTimeStr += text[i];
                    }

                    // Do the division
                    newBuildTime = (buildTimeStr.toFloat()) / divider;

                    // Add spaces to the new line
                    for (i = 0; i < spaces; i++) {
                        newLine += " ";
                    }

                    // Construct new line
                    newLine += "<BuildTimeSeconds>" + QString::number(newBuildTime, 'f', 2) + "</BuildTimeSeconds>";

                    // Write the modified line
                    newFileContent += newLine + '\n';

                } else {
                    //If it's not the build time line, write it
                    newFileContent += text + '\n';
                }

            }
            // Close original file
            inputFile.close();

            // Save the new file
            QString savePath = QFileDialog::getSaveFileName(this, tr("Save File"), dir + "CubeBlocksNew.sbc", tr("(*.sbc)"));
            QFile outputFile(savePath);
            outputFile.open(QIODevice::ReadWrite | QIODevice::Text);
            QTextStream out(&outputFile);
            out.operator <<(newFileContent);
            outputFile.close();
        } else {
            QMessageBox::warning(this, "Reading error", "The file could not be opened.");
        }
    }
}
