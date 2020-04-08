#include "page.h"
#include "ui_page.h"


page::page(QWidget *parent, Transmission* t, QString username) :
    QMainWindow(parent),
    ui(new Ui::page),
    username(username),
    t(t)
{
    ui->setupUi(this);
    setUsernameLabel();
}

page::~page()
{
    delete ui;
}

void page::setAvatar(QPixmap p) {

    ui->avatarLabel->setPixmap(p);
    ui->avatarLabel->setAlignment(Qt::AlignCenter);
}

void page::setUsernameLabel() {
    ui->usernameLabel->setText(username);
    ui->usernameLabel->setAlignment(Qt::AlignCenter);
}

void page::setGridLayout(){
    QLabel *newDoc = new QLabel();
    newDoc->setPixmap(QPixmap(":/img/icons/new-file.png").scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    newDoc->setAlignment(Qt::AlignCenter);

    auto createDocButton = new QPushButton();
    createDocButton->setEnabled(true);
    createDocButton->setText("new document");

    QFrame *container = new QFrame();
    container->setLayout(new QVBoxLayout);
    container->layout()->addWidget(newDoc);
    container->layout()->addWidget(createDocButton);

    ui->docsGridLayout->addWidget(container, 0, 0);

    QVector<QLabel*> labels;
    labels.append(new QLabel);

    QVector<QLabel*> docLabels;
    docLabels.append(new QLabel);

    QVector<QFrame*> containerVector;
    containerVector.append(new QFrame);
    ui->scrollArea->setEnabled(true);

    for(int i = 1; i < 30; i++)
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
        ui->docsGridLayout->addWidget(containerVector[i], i/3, i%3);
    }



    connect(createDocButton, &QAbstractButton::clicked, this, &page::newDocumentSetup);


}

void page::newDocumentSetup(){
    DocTitleDialog *d = new DocTitleDialog(this);
    if(d->exec() !=  QDialog::Accepted)
        return;

    QJsonObject title{
        {"action", 2},
        {"user", username},
        {"docTitle", d->getDocumentTitle()}
    };

    this->t->sendJson(title, "", -1); //questo e' molto brutto ma dovrebbe funzionare
    TextEdit *te=textEditStart();
    te->fileNew();
    te->setCurrentFileName(d->getDocumentTitle());
    te->show();


    this->hide();

    qDebug() << d->getDocumentTitle();


}

TextEdit* page::textEditStart(){
    TextEdit *te = new TextEdit(this);

    const QRect availableGeometry = te->screen()->availableGeometry();
    te->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    te->move((availableGeometry.width() - te->width()) / 2,
            (availableGeometry.height() - te->height()) / 2);




    //t->load(QLatin1String(":/example.html"));
    //t->fileNew();
    return te;

}
