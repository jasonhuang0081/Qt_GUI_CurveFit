/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_gui
{
public:
    QWidget *centralwidget;
    QCustomPlot *myPlot;
    QPushButton *bnt_gen;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *gui)
    {
        if (gui->objectName().isEmpty())
            gui->setObjectName(QString::fromUtf8("gui"));
        gui->resize(522, 538);
        centralwidget = new QWidget(gui);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        myPlot = new QCustomPlot(centralwidget);
        myPlot->setObjectName(QString::fromUtf8("myPlot"));
        myPlot->setGeometry(QRect(40, 10, 441, 401));
        bnt_gen = new QPushButton(centralwidget);
        bnt_gen->setObjectName(QString::fromUtf8("bnt_gen"));
        bnt_gen->setGeometry(QRect(200, 440, 75, 23));
        gui->setCentralWidget(centralwidget);
        menubar = new QMenuBar(gui);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 522, 21));
        gui->setMenuBar(menubar);
        statusbar = new QStatusBar(gui);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        gui->setStatusBar(statusbar);

        retranslateUi(gui);

        QMetaObject::connectSlotsByName(gui);
    } // setupUi

    void retranslateUi(QMainWindow *gui)
    {
        gui->setWindowTitle(QCoreApplication::translate("gui", "gui", nullptr));
        bnt_gen->setText(QCoreApplication::translate("gui", "Generate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gui: public Ui_gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
