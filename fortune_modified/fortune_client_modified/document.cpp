#include "document.h"

Document::Document(QString owner, QString title, QString rndTitle):
    owner(owner),
    title(title),
    rndTitle(rndTitle)
{

}


const QString Document::getOwner(){
    return owner;
}

const QString Document::getTitle(){
    return title;
}

const QString Document::getRndTitle(){
    return rndTitle;
}
