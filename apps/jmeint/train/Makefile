CC			:= g++
LD			:= g++


CFLAGS		:= -Wall -lfann -lbackprop -lboost_regex -std=c++11
LFLAGS		:= -lfann -lbackprop -lboost_regex

INCLUDE		:=  -I../../../backprop/src
LIB			:=  -L../../../backprop/lib

MODULES		:= train_FANN

all: $(MODULES)

$(MODULES): $(MODULES).o
	$(LD) $^ $(LIB) $(LFLAGS) -o $@

$(MODULES).o: $(MODULES).cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(MODULES).o
	@rm -rf $(MODULES)
