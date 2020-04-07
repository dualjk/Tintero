QT += core gui network widgets
requires(qtConfig(combobox))

HEADERS       = client.h \
    clickableframe.h \
    clickablelabel.h \
    doctitledialog.h \
    page.h \
    titleDocument.h
SOURCES       = client.cpp \
                clickableframe.cpp \
                clickablelabel.cpp \
                doctitledialog.cpp \
                main.cpp \
                page.cpp

RESOURCES += client.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

FORMS += \
    doctitledialog.ui \
    page.ui

