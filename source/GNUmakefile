######################################################
#
# GNUmakefile to build the LCCD library
#
# @author F. Gaede, DESY
# @version $Id: GNUmakefile,v 1.1.1.1 2005-02-10 12:17:54 gaede Exp $
######################################################


LCCDBASE := $(PWD)/..
export LCCDBASE 


#CLHEP_INCLUDE := $(CLHEP)/include


LCCDINCLUDE := -I $(LCCDBASE)/source/include -I $(LCIO)/src/cpp/include
#-I $(CLHEP_INCLUDE)
export LCCDINCLUDE

ifdef LCCDDEBUG
 LCCDCPPFLAGS = -g -Wall -ansi
else
 LCCDCPPFLAGS = -O3 -Wall -ansi
endif


ifdef CondDBMySQL
 LCCDCPPFLAGS += -D USE_CONDDB -I $(CondDBMySQL)/include
endif


export LCCDCPPFLAGS


.PHONY: lib clean CORE doc

all: lib bin


lib: CORE


bin: lib
	$(MAKE) -C ./test bin

CORE:
	 echo $(LCCDBASE) ; $(MAKE) -C ./src lib


clean:
	$(MAKE) -C ./src clean; $(MAKE) -C ./test clean; \
	rm -rf $(LCCDBASE)/lib/liblccd.a

doc:
	cd $(LCCDBASE)/doc ; doxygen




