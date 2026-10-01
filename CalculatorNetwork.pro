TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    CalcServer \
    CalcClient \
    CalcTests

DISTFILES += \
    README.md \
    .gitignore