
CONFIG (release, release|debug) {
    message( 'Enabling HifuCrashReport and including debug symbols' )

    DEFINES += HIFU_CRASH_REPORT

    win32-g++* {
        QMAKE_CFLAGS_RELEASE += -g
        QMAKE_CXXFLAGS_RELEASE += -g
        QMAKE_LFLAGS_RELEASE =
    }

#    win32-g++* {
#        QMAKE_CFLAGS_RELEASE -= -O2
#        QMAKE_CXXFLAGS_RELEASE -= -O2

#        QMAKE_CFLAGS_RELEASE += -g -O0
#        QMAKE_CXXFLAGS_RELEASE += -g -O0
#        QMAKE_LFLAGS_RELEASE =
#    }
}

CONFIG (debug, release|debug) {
    message( 'NOTE: CrashReport is only valid for release builds' )
}
