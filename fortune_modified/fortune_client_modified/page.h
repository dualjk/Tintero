#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include <QtWidgets>

#include "textedit.h"
#include <doctitledialog.h>

namespace Ui {
class page;
}

class page : public QMainWindow
{
    Q_OBJECT

public:
    explicit page(QWidget *parent = nullptr, QString username = nullptr);
    ~page();

    void setAvatar(QPixmap p);
    void setUsernameLabel();
    void setGridLayout();
    void newDocumentSetup();
    void newDocumentCreate();

    TextEdit* textEditStart();

private:
    Ui::page *ui;
    QString username;
};

#endif // PAGE_H
