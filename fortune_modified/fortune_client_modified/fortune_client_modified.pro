QT += core gui network widgets
requires(qtConfig(combobox))
requires(qtConfig(filedialog))

qtHaveModule(printsupport): QT += printsupport


HEADERS       = client.h \
    clickableframe.h \
    clickablelabel.h \
    doctitledialog.h \
    document.h \
    page.h \
    textedit.h \
    titleDocument.h \
    transmission.h
SOURCES       = client.cpp \
                clickableframe.cpp \
                clickablelabel.cpp \
                doctitledialog.cpp \
                document.cpp \
                main.cpp \
                page.cpp \
                textedit.cpp \
                transmission.cpp

RESOURCES += client.qrc

EXAMPLE_FILES = textedit.qdoc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

FORMS += \
    doctitledialog.ui \
    page.ui

