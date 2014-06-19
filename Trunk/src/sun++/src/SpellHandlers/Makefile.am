INCLUDES += -I$(srcdir) -I$(srcdir)/../../../../dep/include -I$(srcdir)/../../../arcemu-shared
INCLUDES += -I$(srcdir)/../../../arcemu-world -I$(srcdir)/../../../../dep/src
INCLUDES += -I$(srcdir)/../../../ascent-shared/g3dlite -I$(srcdir)/../../../ascent-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSpellHandlers.la

libSpellHandlers_la_SOURCES = HunterSpells.cpp ItemSpells_1.cpp \
MageSpells.cpp PaladinSpells.cpp RogueSpells.cpp \
ShamanSpells.cpp WarlockSpells.cpp WarriorSpells.cpp QIspells.cpp Setup.cpp
