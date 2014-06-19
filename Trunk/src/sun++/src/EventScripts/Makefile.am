INCLUDES = -I$(top_srcdir)/src/arcemu-logonserver \
   -I$(top_srcdir)/src/arcemu-realmserver -I$(top_srcdir)/src/arcemu-shared -I$(top_srcdir)/src/arcemu-voicechat \
   -I$(top_srcdir)/src/arcemu-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../ascent-shared/g3dlite -I$(srcdir)/../../../ascent-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSunEventScripts.la

libSunEventScripts_la_SOURCES = Setup.cpp \
../InstanceScripts/Base.cpp \
Halloween.cpp \
WinterVeil.cpp
