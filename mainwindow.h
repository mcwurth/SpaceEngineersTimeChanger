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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openDlg();
    void doTheJob();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
