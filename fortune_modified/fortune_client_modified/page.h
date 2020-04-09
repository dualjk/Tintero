#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMessageBox>



#include "textedit.h"
#include "transmission.h"
#include "doctitledialog.h"
#include "document.h"

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
    bool readJsonNewDocument();

    TextEdit* textEditStart();


private:
    Ui::page *ui;
    QString username;
    Transmission* t;
    QString titleDocument;
    QVector<Document>* documentVector;
};

#endif // PAGE_H
