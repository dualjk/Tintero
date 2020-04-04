QT += network widgets
requires(qtConfig(combobox))

HEADERS       = client.h \
    clickableframe.h \
    clickablelabel.h \
    mainpage.h \
    titleDocument.h
SOURCES       = client.cpp \
                clickableframe.cpp \
                clickablelabel.cpp \
                main.cpp \
                mainpage.cpp

RESOURCES += client.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

FORMS +=

