#include "mainpage.h"

void Ui_MainWindow::setupUi(QMainWindow *MainWindow)
{
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(800, 600);
    actionNew_File = new QAction(MainWindow);
    actionNew_File->setObjectName(QString::fromUtf8("actionNew_File"));
    actionOpen_File = new QAction(MainWindow);
    actionOpen_File->setObjectName(QString::fromUtf8("actionOpen_File"));
    actionLogout = new QAction(MainWindow);
    actionLogout->setObjectName(QString::fromUtf8("actionLogout"));
    actionTintero_0_1 = new QAction(MainWindow);
    actionTintero_0_1->setObjectName(QString::fromUtf8("actionTintero_0_1"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayoutWidget = new QWidget(centralwidget);
    verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
    verticalLayoutWidget->setGeometry(QRect(10, 30, 191, 501));
    verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);


    avatarLabel = new ClickableLabel(verticalLayoutWidget);
    avatarLabel->setObjectName(QString::fromUtf8("avatarLabel"));
    avatarLabel->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(avatarLabel);

    usernameLabel = new QLabel(verticalLayoutWidget);
    usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));
    usernameLabel->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(usernameLabel);

    recentDocsLabel = new QListView(verticalLayoutWidget);
    recentDocsLabel->setObjectName(QString::fromUtf8("recentDocsLabel"));

    verticalLayout->addWidget(recentDocsLabel);


    scrollArea = new QScrollArea(centralwidget);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setGeometry(QRect(210, 30, 581, 501));
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 579, 499));
    docTableWdg = new QGridLayout(scrollAreaWidgetContents);
    docTableWdg->setObjectName(QString::fromUtf8("docTableWdg"));
    docTableWdg->setGeometry(QRect(10, 10, 561, 481));
    scrollArea->setWidget(scrollAreaWidgetContents);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 800, 22));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuAbout = new QMenu(menubar);
    menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuAbout->menuAction());
    menuFile->addAction(actionNew_File);
    menuFile->addSeparator();
    menuFile->addAction(actionLogout);
    menuAbout->addAction(actionTintero_0_1);


    retranslateUi(MainWindow);
    setNewDocumentIcon();

    QMetaObject::connectSlotsByName(MainWindow);
} // setupUi

void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow)
{
    MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Tintero Client", nullptr));
    actionNew_File->setText(QCoreApplication::translate("MainWindow", "New File", nullptr));
    actionOpen_File->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
    actionLogout->setText(QCoreApplication::translate("MainWindow", "Logout", nullptr));
    actionTintero_0_1->setText(QCoreApplication::translate("MainWindow", "Tintero 0.1", nullptr));
    usernameLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    avatarLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    menuAbout->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
} // retranslateUi

void Ui_MainWindow::setUsernameLabel(QString username){

    this->usernameLabel->setText("<b>"+username+"</b>");


}

void Ui_MainWindow::setAvatarLabel(QPixmap avatar){
    this->avatarLabel->setPixmap(avatar);
    connect(avatarLabel, &ClickableLabel::clicked, this, &Ui_MainWindow::createDocument);

}


void Ui_MainWindow::setNewDocumentIcon() {

    ClickableLabel *newDoc = new ClickableLabel();
    newDoc->setPixmap(QPixmap(":/img/icons/new-file.png").scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    newDoc->setAlignment(Qt::AlignCenter);

    //ClickableLabel *newDocLabel = new ClickableLabel();
    auto createDocButton = new QPushButton("New document");


    ClickableFrame *container = new ClickableFrame();
    container->setLayout(new QVBoxLayout);
    container->layout()->addWidget(newDoc);
    container->layout()->addWidget(createDocButton);
    container->setStyleSheet("background-color: green");
    connect(container, &ClickableFrame::clicked, this, &Ui_MainWindow::createDocument);



    this->docTableWdg->addWidget(container, 0, 0);
    connect(newDoc, &ClickableLabel::clicked, this, &Ui_MainWindow::createDocument);
    connect(createDocButton, &QPushButton::clicked, this, &Ui_MainWindow::createDocument);
    connect(container, &ClickableFrame::clicked, this, &Ui_MainWindow::createDocument);


    QVector<QLabel*> labels;
    labels.append(new QLabel);

    QVector<QLabel*> docLabels;
    docLabels.append(new QLabel);

    QVector<QFrame*> containerVector;
    containerVector.append(new QFrame);

    for(int i = 1; i < 50; i++)
    {

        containerVector.append(new QFrame);

        labels.append(new QLabel);
        labels[i]->setPixmap(QPixmap(":/img/icons/blank.png").scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        labels[i]->setAlignment(Qt::AlignCenter);

        docLabels.append(new QLabel);
        docLabels[i]->setText("Doc"+QString::number(i));
        docLabels[i]->setAlignment(Qt::AlignCenter);

        containerVector[i]->setLayout(new QVBoxLayout);
        containerVector[i]->layout()->addWidget(labels[i]);
        containerVector[i]->layout()->addWidget(docLabels[i]);
        containerVector[i]->setStyleSheet("background-color: yellow");
        this->docTableWdg->addWidget(containerVector[i], i/3, i%3);
    }



}


void Ui_MainWindow::createDocument(){

    qDebug()<<"arrivo qui";
    QDialog *titleDoc = new QDialog();
    Ui_Dialog title;
    title.setupUi(titleDoc);
    titleDoc->show();


}
