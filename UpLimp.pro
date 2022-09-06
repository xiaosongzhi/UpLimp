QT += core gui sql serialport network printsupport charts xml  multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += ABSOLUTEPATH  #是否使用绝对路径
#DEFINES += LOG4QT #DATABASE
#关闭debug输出
#DEFINES += QT_NO_DEBUG_OUTPUT
INCLUDEPATH += $$PWD/src/ \
               $$PWD/src/rankingList \
               $$PWD/src/settingDialog \
               $$PWD/src/teachManager \
               $$PWD/src/tipDialog \
               $$PWD/src/trainHistory \
               $$PWD/src/trainingManager \
               $$PWD/src/userManager \
               $$PWD/src/log4j  \
               $$PWD/src/dataBase \
               $$PWD/src/communication \
               $$PWD/src/sharedModule \
               $$PWD/src/report \
               $$PWD/src/scopeManager \
               $$PWD/src/Mainwindow

INCLUDEPATH += $$PWD/src/log4j/include \
               $$PWD/src/log4j/src \
               $$PWD/src/log4j/src/log4qt


HEADERS += \
    src/Mainwindow/controlmaskdialog.h \
    src/Mainwindow/mainwindow.h \
    src/Mainwindow/movementrange.h \
    src/communication/ccommunicateapi.h \
    src/communication/ccommunicationinterface.h \
    src/communication/cserialportinterface.h \
    src/communication/ctcpsocketinterface.h \
    src/communication/cudpinterface.h \
    src/communication/dataFormate.h \
    src/communication/globalqueue.h \
    src/communication/readconfig.h \
    src/dataBase/cdatabaseinterface.h \
    src/dataBase/mysqldatabase.h \
    src/dataBase/userdataformate.h \
    src/log4j/clog4j.h \
    src/rankingList/rankinglist.h \
    src/report/cprinter.h \
    src/report/linechart.h \
    src/report/reportdialog.h \
    src/scopeManager/paintdevice.h \
    src/scopeManager/scopemanager.h \
    src/scopeManager/scopereport.h \
    src/scopeManager/scopetest.h \
    src/settingDialog/settingdialog.h \
    src/sharedModule/bindtipdialog.h \
    src/sharedModule/cbasedialog.h \
    src/sharedModule/cmaskdialog.h \
    src/sharedModule/controlmaskdialog.h \
    src/sharedModule/cpassworddialog.h \
    src/sharedModule/cpushbutton.h \
    src/sharedModule/cwidget.h \
    src/sharedModule/resethandledialog.h \
    src/sharedModule/warningdialog.h \
    src/teachManager/cgraphicsscene.h \
    src/teachManager/cgraphicsscene.h \
    src/teachManager/clistwidgetitem.h \
    src/teachManager/link.h \
    src/teachManager/node.h \
    src/teachManager/recipeselectdialog.h \
    src/teachManager/teachedit.h \
    src/teachManager/teachmanager.h \
    src/teachManager/teachmanagerdialog.h \
    src/teachManager/teachpaint.h \
    src/tipDialog/messagebox.h \
    src/tipDialog/waitingdialog.h \
    src/trainHistory/traininghistory.h \
    src/trainingManager/cactivetrain.h \
    src/trainingManager/cagainsttrain.h \
    src/trainingManager/cboosttrain.h \
    src/trainingManager/cpassivetrain.h \
    src/trainingManager/gamepaintdevice.h \
    src/trainingManager/gameparamcontrol.h \
    src/trainingManager/simulategame.h \
    src/trainingManager/trainingmanager.h \
    src/userManager/currentuserdata.h \
    src/userManager/edituser.h \
    src/userManager/usermanager.h

SOURCES += \
    src/Mainwindow/controlmaskdialog.cpp \
    src/Mainwindow/main.cpp \
    src/Mainwindow/mainwindow.cpp \
    src/Mainwindow/movementrange.cpp \
    src/communication/ccommunicateapi.cpp \
    src/communication/ccommunicationinterface.cpp \
    src/communication/cserialportinterface.cpp \
    src/communication/ctcpsocketinterface.cpp \
    src/communication/cudpinterface.cpp \
    src/communication/dataFormate.cpp \
    src/communication/globalqueue.cpp \
    src/communication/readconfig.cpp \
    src/dataBase/cdatabaseinterface.cpp \
    src/dataBase/mysqldatabase.cpp \
    src/dataBase/userdataformate.cpp \
    src/log4j/clog4j.cpp \
    src/rankingList/rankinglist.cpp \
    src/report/cprinter.cpp \
    src/report/linechart.cpp \
    src/report/reportdialog.cpp \
    src/scopeManager/paintdevice.cpp \
    src/scopeManager/scopemanager.cpp \
    src/scopeManager/scopereport.cpp \
    src/scopeManager/scopetest.cpp \
    src/settingDialog/settingdialog.cpp \
    src/sharedModule/bindtipdialog.cpp \
    src/sharedModule/cbasedialog.cpp \
    src/sharedModule/cmaskdialog.cpp \
    src/sharedModule/controlmaskdialog.cpp \
    src/sharedModule/cpassworddialog.cpp \
    src/sharedModule/cpushbutton.cpp \
    src/sharedModule/cwidget.cpp \
    src/sharedModule/resethandledialog.cpp \
    src/sharedModule/warningdialog.cpp \
    src/teachManager/cgraphicsscene.cpp \
    src/teachManager/cgraphicsscene.cpp \
    src/teachManager/clistwidgetitem.cpp \
    src/teachManager/link.cpp \
    src/teachManager/node.cpp \
    src/teachManager/recipeselectdialog.cpp \
    src/teachManager/teachedit.cpp \
    src/teachManager/teachmanager.cpp \
    src/teachManager/teachmanagerdialog.cpp \
    src/teachManager/teachpaint.cpp \
    src/tipDialog/messagebox.cpp \
    src/tipDialog/waitingdialog.cpp \
    src/trainHistory/traininghistory.cpp \
    src/trainingManager/cactivetrain.cpp \
    src/trainingManager/cagainsttrain.cpp \
    src/trainingManager/cboosttrain.cpp \
    src/trainingManager/cpassivetrain.cpp \
    src/trainingManager/gamepaintdevice.cpp \
    src/trainingManager/gameparamcontrol.cpp \
    src/trainingManager/simulategame.cpp \
    src/trainingManager/trainingmanager.cpp \
    src/userManager/currentuserdata.cpp \
    src/userManager/edituser.cpp \
    src/userManager/usermanager.cpp

FORMS += \
    src/Mainwindow/mainwindow.ui \
    src/Mainwindow/movementrange.ui \
    src/rankingList/rankinglist.ui \
    src/report/cprinter.ui \
    src/report/linechart.ui \
    src/report/reportdialog.ui \
    src/scopeManager/scopemanager.ui \
    src/scopeManager/scopereport.ui \
    src/scopeManager/scopetest.ui \
    src/settingDialog/settingdialog.ui \
    src/sharedModule/bindtipdialog.ui \
    src/sharedModule/cbasedialog.ui \
    src/sharedModule/cpassworddialog.ui \
    src/sharedModule/resethandledialog.ui \
    src/sharedModule/warningdialog.ui \
    src/teachManager/clistwidgetitem.ui \
    src/teachManager/recipeselectdialog.ui \
    src/teachManager/teachedit.ui \
    src/teachManager/teachmanager.ui \
    src/teachManager/teachmanagerdialog.ui \
    src/tipDialog/messagebox.ui \
    src/tipDialog/waitingdialog.ui \
    src/trainHistory/traininghistory.ui \
    src/trainingManager/cactivetrain.ui \
    src/trainingManager/cagainsttrain.ui \
    src/trainingManager/cboosttrain.ui \
    src/trainingManager/cpassivetrain.ui \
    src/trainingManager/simulategame.ui \
    src/trainingManager/trainingmanager.ui \
    src/userManager/edituser.ui \
    src/userManager/usermanager.ui


LIBS += -L$$PWD/src/log4j/bin -llog4qt
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
#使用中文的编码配置
QMAKE_CXXFLAGS += /execution-charset:utf-8   /source-charset:utf-8

RESOURCES += \
    image.qrc

RC_ICONS = logo_ICO.ico



