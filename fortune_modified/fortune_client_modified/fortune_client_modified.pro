QT += network widgets
requires(qtConfig(combobox))

HEADERS       = client.h \
    clickablelabel.h
SOURCES       = client.cpp \
                clickablelabel.cpp \
                main.cpp

RESOURCES += client.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

