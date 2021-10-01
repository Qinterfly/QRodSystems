
lessThan(QT_MAJOR_VERSION, 6) {
    CONFIG(debug, debug|release) {
        win32 {
            LIBS += -L$${ADS_PATH} -lqtadvanceddockingd
        }
        else:mac {
            LIBS += -L$${ADS_PATH} -lqtadvanceddocking_debug
        }
        else {
            LIBS += -L$${ADS_PATH} -lqtadvanceddocking
        }
    }
    else {
        LIBS += -L$${ADS_PATH} -lqtadvanceddocking
    }
}
else {
    LIBS += -L$${ADS_PATH} -lqtadvanceddocking
}

unix:!macx {
    LIBS += -L$${ADS_PATH} -lxcb
}

INCLUDEPATH += $${ADS_PATH}/src
DEPENDPATH  += $${ADS_PATH}/src

adsBuildStatic {
    DEFINES += ADS_STATIC
}

