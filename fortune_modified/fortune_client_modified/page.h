#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMessageBox>



#include "textedit.h"
#include "transmission.h"
#include "doctitledialog.h"
#include "document.h"
#include "clickableframe.h"

namespace Ui {
class page;
}

class page : public QMainWindow
{
    Q_OBJECT

public:
    explicit page(QWidget *parent = nullptr, Transmission* t=nullptr, QString username = nullptr, QVector<Document>* documentVector = nullptr);
    ~page();

    void setAvatar(QPixmap p);
    void setUsernameLabel();
    void setGridLayout();
    void newDocumentSetup();
    void newDocumentCreate();
    QString readJsonNewDocument();
    void documentButtonClicked();

    TextEdit* textEditStart();
    void readFile();


private:
    Ui::page *ui;
    QString username;
    Transmission* t;
    QString titleDocumentOriginal;
    QString titleDocumentRnd;
    QVector<Document>* documentVector;
    int indexDocument=-1;

};

#endif // PAGE_H
