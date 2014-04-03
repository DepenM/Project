.SUFFIXES: .cpp .hpp

# Programs
SHELL 	= bash
CC     	= g++
LD	= ld
RM 	= rm
ECHO	= /bin/echo
CAT	= cat
PRINTF	= printf
SED	= sed
DOXYGEN = doxygen
######################################
# Project Name (generate executable with this name)
TARGET = cs296_11_exe

# Project Paths
PROJECT_ROOT=./
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/myobjs
BINDIR = $(PROJECT_ROOT)/mybins
DOCDIR = $(PROJECT_ROOT)/doc
LIBDIR = $(PROJECT_ROOT)/mylibs
INCLUDE = $(EXTERNAL_ROOT)/src/Box2D/Box2D/Box2D.h
BOX2DFOLD = $(EXTERNAL_ROOT)/src/Box2D

# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr/include/

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL

# Compiler and Linker flags
CPPFLAGS =-g -O3 -Wall -fno-strict-aliasing
CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGS+=-L $(BOX2D_ROOT)/lib -L $(GLUI_ROOT)/lib

######################################

NO_COLOR=\e[0m
OK_COLOR=\e[1;32m
ERR_COLOR=\e[1;31m
WARN_COLOR=\e[1;33m
MESG_COLOR=\e[1;34m
FILE_COLOR=\e[1;37m

OK_STRING="[OK]"
ERR_STRING="[ERRORS]"
WARN_STRING="[WARNINGS]"
OK_FMT="${OK_COLOR}%30s\n${NO_COLOR}"
ERR_FMT="${ERR_COLOR}%30s\n${NO_COLOR}"
WARN_FMT="${WARN_COLOR}%30s\n${NO_COLOR}"
SHARED_LIB=FALSE
######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.hpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
OB := $(wildcard $(OBJDIR)/*[!main].o)
ABC := $(SRCS:$(SRCDIR)/%.cpp=.//myobjs/%.o)



		

.PHONY: all setup doc clean distclean project

all: setup

exe: $(OBJS) $(BINDIR)/$(TARGET)

project: clean setup exe
	 ./mybins/cs296_11_exe

exelib : target
	@if test $(SHARED_LIB) = FALSE; then \
		$(CC) -o $(BINDIR)/cs296_11_exe $(LDFLAGS) .//myobjs/main.o $(LIBDIR)/libCS296test.a $(LIBS); \
	else \
		$(CC) -o $(BINDIR)/cs296_11_exe $(LDFLAGS) .//myobjs/main.o $(LIBDIR)/libCS296test.so $(LIBS); \
	fi

target : $(OB) $(SRCDIR)
	@if test $(SHARED_LIB) = FALSE; then \
		ar -cq libCS296test.a $(OB); \
		 mv libCS296test.a .//mylibs/libCS296test.a; \
	else \
#$(CC) -Wall -fPIC -c *.c; \
		gcc -shared -Wall $(CPPFLAGS) -o $(LIBDIR)/libCS296test.so -fPIC $(SRCDIR)/*[!main].cpp; \
#$(CC) -shared -Wl -soname libCS296test1.so.1 -o libCS296test.so $(OB); \
#mv libCS296test.so .//mylibs/libCS296test.so; \
	fi	

setup:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p myobjs
	@mkdir -p mylibs
	@mkdir -p mybins
	@if [ ! -f $(INCLUDE) ]; then \
		cd ./external/src; \
		tar xvzf Box2D.tgz; \
		cd Box2D; \
		mkdir -p build296; \
		cd build296; \
		cmake ../; \
		make; \
		make install; \
		cd ..; \
		cd ..; \
		cd ..; \
		cd ..; \
	else \
		echo "Box2D already installed"; \
	fi

#	@if test -e $(INCLUDE); then \
#		cd $(EXTERNAL_ROOT)/src; \
#		tar xvzf Box2D.tgz; \
#		cd Box2D; \
#		mkdir -p build296; \
#		cd build296; \
#		cmake ../; \
#		make; \
#		make install; \
#		cd..; \
#		cd..; \
#		cd..; \
#		cd..; \
#		make; \
#	else \
#		  @$(ECHO) "Box2D already installed"; \
#	fi
          
          

$(BINDIR)/$(TARGET): $(OBJS)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $@)"
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS) 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

-include $(OBJS:.o=.d)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) $(CPPFLAGS) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

doc:
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"

clean:
	@$(ECHO) -n "Cleaning up..."
	@$(RM) -rf $(OBJDIR) $(LIBDIR) $(BINDIR)
	@$(ECHO) "Done"
	@$(RM) -rf ./doc/cs296_report_11.dvi ./doc/cs296_report_11.pdf ./doc/cs296_report_11.bbl ./doc/cs296_report_11.log ./doc/cs296_report_11.blg ./doc/cs296_report_11.aux ./doc/ball-eps-converted-to.pdf ./doc/platform-eps-converted-to.pdf ./doc/Wall-eps-converted-to.pdf

distclean: clean
	@$(RM) -rf $(PROJECT_ROOT)/bin $(BOX2DFOLD) $(PROJECT_ROOT)/obj $(EXTERNAL_ROOT)/include/Box2D $(EXTERNAL_ROOT)/lib/Box2D $(EXTERNAL_ROOT)/lib/libBox2D.a
report : 
	@cd doc; \
	latex cs296_report_11; \
	bibtex cs296_report_11; \
	latex cs296_report_11; \
	latex cs296_report_11; \
	latex cs296_report_11; \
	dvipdfm cs296_report_11; \
	cd ..
