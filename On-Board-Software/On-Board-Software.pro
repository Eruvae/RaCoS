ENV_HOME = $$system(env | grep HOME)
ENV_HOME_DIR = $$replace(ENV_HOME, HOME=,)

TEMPLATE = app
CONFIG -= qt debug_and_release
INCLUDEPATH += $$ENV_HOME_DIR/rodos/rodos-core/api
OBJECTS_DIR = ./
DESTDIR = ./

HEADERS += \
    topics.h \
    telemetry.h \
    structs.h \
    sensorIMU.h \
    spihelper.h \
    comminterfaces.h \
    sensorhousekeeping.h \
    telecommand.h \
    actuatorhandler.h \
    storagecontroller.h \
    murmur.h \
    healthwatchdog.h

SOURCES += \
    topics.cpp \
    telemetry.cpp \
    sensorIMU.cpp \
    spihelper.cpp \
    comminterfaces.cpp \
    sensorhousekeeping.cpp \
    telecommand.cpp \
    actuatorhandler.cpp \
    storagecontroller.cpp \
    murmur.cpp \
    healthwatchdog.cpp

OTHER_FILES += \
    flash_script

rodos.commands = "/bin/bash -c \"cd $$ENV_HOME_DIR/rodos/rodos-core && source make/rodosenvs \
    && cd $$_PRO_FILE_PWD_ && stm32f4-executable $$HEADERS $$SOURCES && cp -f flash_script On-Board-Software \
    && chmod +x On-Board-Software\""

QMAKE_EXTRA_TARGETS += rodos
QMAKE_CLEAN += On-Board-Software stm32f4.hex stm32f4.elf

