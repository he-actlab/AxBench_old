CC			:= g++
LD			:= g++

CFLAGS		:= -g -Wall -lboost_regex -O0 -std=c++11
LFLAGS		:= -g -lboost_regex

MODULES		:= ann
SRC_DIR		:= src
BUILD_DIR	:= obj data data/dot data/jpeg lib
OBJ_DIR		:= obj

SRC 		:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ 		:= $(patsubst src/%.cpp,obj/%.o,$(SRC))

vpath %.cpp $(SRC_DIR)



.PHONY: all checkdirs clean doc
	
all: checkdirs create_lib

bin/ann: $(OBJ)
	$(LD) $^ $(LFLAGS) -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

create_lib: $(OBJ)
	ar crf lib/libbackprop.a $^

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	if [ ! -d $@ ] ; then mkdir -p $@ ; fi

clean:
	@rm -rf $(OBJ_DIR)
