NPU_MODE	:= NPU_FANN
CC			:= g++
LD			:= g++

include ../../config.mk

CFLAGS		:= -Wall -lfann -lbackprop -lboost_regex -std=c++11
LFLAGS		:= -lfann -lbackprop -lboost_regex

INCLUDE 	:= -I${ANALOG_INC} -I${FANN_INC}
LIB			:= -L${ANALOG_LIB} -L${FANN_LIB}

MODULE		:= jpeg_$(NPU_MODE).out

CPP_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.c=.o)))


.PHONY: all clean
	
all: DIR $(MODULE)

DIR:
	if [ ! -d "./bin" ];then 	\
		mkdir bin;				\
	fi
	if [ ! -d "./obj" ];then 	\
		mkdir obj;				\
	fi

$(MODULE): $(OBJ_FILES)
	$(LD) $^ $(LIB) $(LFLAGS) -o bin/$@

obj/%.o: src/%.c
	$(CC) -D$(NPU_MODE) $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf *.o
	@rm -rf *.d
	@rm -rf inversek2j_$(NPU_MODE).out
	@rm -rf bin
	@rm -rf obj
