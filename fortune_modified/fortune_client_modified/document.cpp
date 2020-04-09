#include "document.h"

Document::Document(QString owner, QString title):
    owner(owner),
    title(title)
{

}


const QString Document::getOwner(){
    return owner;
}

const QString Document::getTitle(){
    return title;
}
