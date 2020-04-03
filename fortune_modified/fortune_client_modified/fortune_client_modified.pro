QT += network widgets
requires(qtConfig(combobox))

HEADERS       = client.h \
    clickablelabel.h \
    mainpage.h
SOURCES       = client.cpp \
                clickablelabel.cpp \
                main.cpp \
                mainpage.cpp

RESOURCES += client.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

FORMS +=

