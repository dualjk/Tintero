/********************************************************************************
** Form generated from reading UI file 'mainpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets>
#include <QTranslator>
#include <QObject>
#include <QDialog>
#include "clickablelabel.h"
#include "titleDocument.h"
#include "clickableframe.h"




QT_BEGIN_NAMESPACE

class Ui_MainWindow : public QObject
{
    Q_OBJECT

private slots:
    void createDocument();

public:
    QAction *actionNew_File;
    QAction *actionOpen_File;
    QAction *actionLogout;
    QAction *actionTintero_0_1;
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *usernameLabel;
    ClickableLabel *avatarLabel;
    QListView *recentDocsLabel;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *docTableWdg;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QStatusBar *statusbar;

    void setUsernameLabel(QString username);
    void setAvatarLabel(QPixmap avatar);
    void setNewDocumentIcon();



    void setupUi(QMainWindow *MainWindow);

    void retranslateUi(QMainWindow *MainWindow);




};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui



QT_END_NAMESPACE

#endif // MAINPAGE_H
