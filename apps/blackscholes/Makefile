NPU_MODE	:=	NPU_FANN

PREFIX=${PARSECDIR}/pkgs/apps/blackscholes/inst/${PARSECPLAT}

CSRC    = $(SRC)
TARGET  = blackscholes_$(NPU_MODE).out
M4_BASE = .
MACROS  = c.m4.pthreads

include ../../config.mk


SRC	= src/blackscholes.c


# Default build single precision version
NCO     = -DNCO=4

ifdef chk_err
ERR     = -DERR_CHK
endif

ifdef single
NCO = -DNCO=4
endif

ifdef size
SZ = -DN=$(size)
else
SZ = -DN=960
endif

ifdef double
NCO = -DNCO=2
endif

CXXFLAGS += $(NCO) $(FUNC) $(ERR) $(CSRC)

CFLAGS		:= -Wall -Wnarrowing -lfann -lbackprop -lboost_regex -std=c++11 -O0 -MD
LDFLAGS		:= -lfann -lbackprop -lboost_regex

INCLUDE 	:= -I${ANALOG_INC} -I${FANN_INC}
LIB			:= -L${ANALOG_LIB} -L${FANN_LIB}



all: $(TARGET)

$(TARGET): clean $(CSRC)
	$(CXX) -D$(NPU_MODE) $(CFLAGS) $(INCLUDE) $(CXXFLAGS) -g $(LDFLAGS) $(LIB) -o bin/$(TARGET)

$(M4_SRC): $(SRC)
	$(M4) $(M4_BASE)/$(MACROS) $< > $@

clean:
	rm -f $(TARGET) $(M4_SRC)
	mkdir -p bin

install:
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin/$(TARGET)

