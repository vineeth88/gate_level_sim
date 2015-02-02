BENCH = b12
DEFINE_BENCH = -D__${BENCH}
BENCH_DIR = ./lib/${BENCH}
LIBS =  ${BENCH_DIR}/cpp/Vtop__ALL.a

CXX = g++
CFLAGS = -c -Wall $(DEFINE_BENCH)

SYSTEMPERL = /home/vineeth/Tools/SystemPerl/src
VERILATOR_INCLUDE = /usr/local/share/verilator/include

INCL_DIR = ./include
SRC_DIR = ./src
EXEC_DIR = ./exec
INCLUDE = -I$(SYSTEMPERL) -I$(VERILATOR_INCLUDE) -I$(INCL_DIR)

DEBUG = -g -O0 -DMEM_ALLOC_DBG_ON
#DEBUG = -g -D_DBG_DEST_CALL_
CFLAGS += ${DEBUG}

all: gen

OBJS_TEST = testSim.o ${BENCH}Int.o \
circuit.o rtLevelSim.o \
verilated.o SpCoverage.o
TARGET_MAIN = ${BENCH}TestSim

OBJS_GEN = circuit.o rtLevelSim.o \
${BENCH}Int.o \
verilated.o SpCoverage.o \
gaLib.o graphLib.o \
branchSim.o 
#newSim.o
TARGET_GEN = ${BENCH}Sim
#DEFINE_GEN = -D_STAGE1_OLD_FN_

OBJS_METRIC = circuit.o rtLevelSim.o \
${BENCH}Int.o \
verilated.o SpCoverage.o \
newMetric.o
TARGET_METRIC = ${BENCH}CompMetric

test: $(OBJS_TEST)
	$(CXX) $(OBJS_TEST) $(INCLUDE) $(LIBS) -o $(EXEC_DIR)/$(TARGET_MAIN) -lm -lstdc++ 2>&1 | c++filt

gen: $(OBJS_GEN)
	$(CXX) $(OBJS_GEN) $(INCLUDE) $(LIBS) -o $(EXEC_DIR)/$(TARGET_GEN) -pg -lm -lstdc++ 2>&1 | c++filt

proc: $(OBJS_METRIC)
	$(CXX) $(OBJS_METRIC) $(INCLUDE) $(LIBS) -o $(EXEC_DIR)/$(TARGET_METRIC) -pg -lm -lstdc++ 2>&1 | c++filt

# All object files below

circuit.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(SRC_DIR)/circuit.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o circuit.o $(SRC_DIR)/circuit.cpp

rtLevelSim.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/${BENCH}Int.cpp $(INCL_DIR)/rtLevelSim.h $(SRC_DIR)/rtLevelSim.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o rtLevelSim.o $(SRC_DIR)/rtLevelSim.cpp

testSim.o:	$(INCL_DIR)/circuit.h $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/testSim.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o testSim.o $(SRC_DIR)/testSim.cpp

newSim.o: $(INCL_DIR)/verInclude.h $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(INCL_DIR)/gaLib.h $(SRC_DIR)/newSim.cpp $(SRC_DIR)/${BENCH}Int.cpp $(SRC_DIR)/gaLib.cpp 
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o newSim.o $(SRC_DIR)/newSim.cpp

branchSim.o: $(INCL_DIR)/verInclude.h $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(INCL_DIR)/gaLib.h $(SRC_DIR)/branchSim.cpp $(SRC_DIR)/${BENCH}Int.cpp $(SRC_DIR)/gaLib.cpp 
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o branchSim.o $(SRC_DIR)/branchSim.cpp

gaLib.o: $(SRC_DIR)/gaLib.cpp $(INCL_DIR)/gaLib.h $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/${BENCH}Int.cpp $(INCL_DIR)/verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o gaLib.o $(SRC_DIR)/gaLib.cpp

verilated.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o verilated.o ${VERILATOR_INCLUDE}/verilated.cpp

SpCoverage.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o SpCoverage.o ${SYSTEMPERL}/SpCoverage.cpp

${BENCH}Int.o: $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/${BENCH}Int.cpp $(INCL_DIR)/vType.h $(INCL_DIR)/verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o ${BENCH}Int.o $(SRC_DIR)/${BENCH}Int.cpp

graphLib.o: $(SRC_DIR)/graphLib.cpp $(INCL_DIR)/graphLib.h $(INCL_DIR)/verInclude.h $(INCL_DIR)/incl.h $(INCL_DIR)/vType.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o graphLib.o $(SRC_DIR)/graphLib.cpp

newMetric.o: $(INCL_DIR)/circuit.h $(INCL_DIR)/incl.h $(INCL_DIR)/rtLevelSim.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/newMetric.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o newMetric.o $(SRC_DIR)/newMetric.cpp

fsimNew:
	$(CXX) -g -o $(EXEC_DIR)/fsimNew $(SRC_DIR)/faultsim.cpp

clean:
	rm -f *.o
	rm -f *.d
