
SRC_DIR = ..
LIB_DIR = ../../lib

SRCS = $(SRC_DIR)/OFDMMain.cpp $(SRC_DIR)/OFDM_baseline_nrvs_same_array_cyclic.cpp
SRCS += ../../lte_phy.cpp $(LIB_DIR)/GeneralFunc.cpp $(LIB_DIR)/gauss.cpp $(LIB_DIR)/check.cpp ../../fft/fft_baseline.cpp

OBJS := $(patsubst %.cpp, %.o, $(SRCS))

INC_FLAGS = -I.. -I../../ -I$(LIB_DIR)

CXXFLAGS = -g -Wall
LINKFLAGS = 

CXXFLAGS += $(INC_FLAGS)

TARGETS = OFDM.out

OFDM.out: $(OBJS)
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(OBJS) $(TARGETS)
