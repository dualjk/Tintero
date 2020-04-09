#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <QObject>


class Document
{

public:
    Document(QString owner = "", QString title = "");
    const QString getOwner();
    const QString getTitle();

private:
    QString owner;
    QString title;

};

#endif // DOCUMENT_H
