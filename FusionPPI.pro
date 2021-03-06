#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T11:23:11
#
#-------------------------------------------------

QT       += core gui

qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FusionPPI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += ./src/main.cpp\
    ./src/mainwindow.cpp \
    ./src/view.cpp \
    ./src/chip.cpp \
    ./src/gradientpalette.cpp \
    ./src/configuration.cpp \
    ./src/title.cpp \
    ./src/robot.cpp \
    ./src/fusionpair.cpp \
    ./src/qtdensity.cpp \
    ./src/dwag.cpp \
    ./src/runtimeinfo.cpp \
    ./src/result.cpp \
    src/tabdialog.cpp \
    src/creapairmatrix.cpp

HEADERS  += ./src/mainwindow.h \
    ./src/view.h \
    ./src/chip.h \
    ./src/gradientpalette.h \
    ./src/configuration.h \
    ./src/title.h \
    ./src/robot.h \
    ./src/fusionpair.h \
    ./src/qtdensity.h \
    ./src/dwag.h \
    ./src/runtimeinfo.h \
    ./src/result.h \
    src/tabdialog.h \
    src/gmm.h \
    src/creapairmatrix.h

FORMS    +=

DISTFILES += \
    AlignForm.ui.qml \
    Align.qml
LIBS += -lz




################################# RInside ##################################################

## beyond the default configuration, also use SVG graphics
QT += 			svg

## comment this out if you need a different version of R,
## and set set R_HOME accordingly as an environment variable
R_HOME = 		$$system(R RHOME)
#message("R_HOME is" $$R_HOME)

## include headers and libraries for R
RCPPFLAGS = 		$$system($$R_HOME/bin/R CMD config --cppflags)
RLDFLAGS = 		$$system($$R_HOME/bin/R CMD config --ldflags)
RBLAS = 		$$system($$R_HOME/bin/R CMD config BLAS_LIBS)
RLAPACK = 		$$system($$R_HOME/bin/R CMD config LAPACK_LIBS)

## if you need to set an rpath to R itself, also uncomment
RRPATH =		-Wl,-rpath,$$R_HOME/lib

## include headers and libraries for Rcpp interface classes
## note that RCPPLIBS will be empty with Rcpp (>= 0.11.0) and can be omitted
RCPPINCL = 		$$system($$R_HOME/bin/Rscript -e \"Rcpp:::CxxFlags\(\)\")
RCPPLIBS = 		$$system($$R_HOME/bin/Rscript -e \"Rcpp:::LdFlags\(\)\")

## for some reason when building with Qt we get this each time
##   /usr/local/lib/R/site-library/Rcpp/include/Rcpp/module/Module_generated_ctor_signature.h:25: warning: unused parameter ‘classname
## so we turn unused parameter warnings off
## no longer needed with Rcpp 0.9.3 or later
#RCPPWARNING =		-Wno-unused-parameter

## include headers and libraries for RInside embedding classes
RINSIDEINCL = 		$$system($$R_HOME/bin/Rscript -e \"RInside:::CxxFlags\(\)\")
RINSIDELIBS = 		$$system($$R_HOME/bin/Rscript -e \"RInside:::LdFlags\(\)\")

## compiler etc settings used in default make rules
QMAKE_CXXFLAGS +=	$$RCPPWARNING $$RCPPFLAGS $$RCPPINCL $$RINSIDEINCL
QMAKE_LIBS +=           $$RLDFLAGS $$RBLAS $$RLAPACK $$RINSIDELIBS $$RCPPLIBS

################################# RInside ##################################################
