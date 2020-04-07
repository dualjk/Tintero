QT += core gui network widgets
requires(qtConfig(combobox))
requires(qtConfig(filedialog))

qtHaveModule(printsupport): QT += printsupport


HEADERS       = client.h \
    clickableframe.h \
    clickablelabel.h \
    doctitledialog.h \
    page.h \
    textedit.h \
    titleDocument.h
SOURCES       = client.cpp \
                clickableframe.cpp \
                clickablelabel.cpp \
                doctitledialog.cpp \
                main.cpp \
                page.cpp \
                textedit.cpp

RESOURCES += client.qrc

EXAMPLE_FILES = textedit.qdoc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

FORMS += \
    doctitledialog.ui \
    page.ui

