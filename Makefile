# base Definition
SYSNAME := $(shell uname -s)

CXX = g++
CPP_FLAGS = -std=gnu++11 -Wall -O3 -I ./src -fPIC
LD_FLAGS = -lboost_program_options \
           $(shell pkg-config thrift-nb lept tesseract --libs) \
           -levent -lpthread -ldl

# if the OS is not Mac, use openmp.
ifneq ($(SYSNAME), Darwin)
    CPP_FLAGS += -fopenmp
endif

# for code generated by thrift
GEN_SRCS = $(wildcard ./src/gen-cpp/*.cpp)
GEN_OBJS = $(patsubst %.cpp, %.o, ${GEN_SRCS})

# for code created by hand
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp, %.o, ${SRCS})
BIN = tassadar_server

# depencies
all: $(BIN)

$(BIN): $(GEN_OBJS) $(OBJS)
	$(CXX) $(OBJS) $(GEN_OBJS) $(CPP_FLAGS) -o $@ $(LD_FLAGS)

$(GEN_OBJS): %.o: %.cpp
	$(CXX) $(CPP_FLAGS) $< -c -o $@

$(OBJS): %.o: %.cpp
	$(CXX) $(CPP_FLAGS) $< -c -o $@ -DCPU_ONLY $(LD_FLAGS)


.PHONY: clean
clean:
	rm $(OBJS) -f
	rm $(GEN_OBJS) -f
	rm $(BIN) -f
