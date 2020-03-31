#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}


void ClickableLabel::setValue(int n){
    this->value=n;
}

int ClickableLabel::getValue(){
    return this->value;
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
