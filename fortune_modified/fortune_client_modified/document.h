#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <QObject>


class Document
{

public:
    Document(QString owner = "", QString title = "", QString rndTitle = "");
    const QString getOwner();
    const QString getTitle();
    const QString getRndTitle();

private:
    QString owner;
    QString title;
    QString rndTitle;

};

#endif // DOCUMENT_H
