QT += network widgets
requires(qtConfig(combobox))

HEADERS       = client.h \
    borderlayout.h \
    clickablelabel.h
SOURCES       = client.cpp \
                borderlayout.cpp \
                clickablelabel.cpp \
                main.cpp

RESOURCES += client.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

