
SRC_DIR = ..
LIB_DIR = ../../lib

SRCS = $(SRC_DIR)/EqualizerMain.cpp $(SRC_DIR)/Equalizer_baseline.cpp
SRCS += ../../ResMapping/ResMapper_base.cpp ../../OFDM/OFDM_baseline_nrvs_same_array_cyclic.cpp
SRCS += ../../lte_phy.cpp $(LIB_DIR)/GeneralFunc.cpp $(LIB_DIR)/gauss.cpp $(LIB_DIR)/check.cpp ../../dmrs/dmrs_base.cpp ../../fft/fft_baseline.cpp

OBJS := $(patsubst %.cpp, %.o, $(SRCS))

INC_FLAGS = -I.. -I../../ -I$(LIB_DIR) -I../../fft -I../../ResMapping -I../../OFDM

CXXFLAGS = -g -Wall
LINKFLAGS = 

CXXFLAGS += $(INC_FLAGS)

TARGETS = CEEQ.out

CEEQ.out: $(OBJS)
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(OBJS) $(TARGETS)
