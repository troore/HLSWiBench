
SRC_DIR = ..
LIB_DIR = ../../lib

SRCS = $(SRC_DIR)/ResMapMain.cpp $(SRC_DIR)/ResMapper_base.cpp ../../dmrs/dmrs.cpp
SRCS += ../../lte_phy.cpp $(LIB_DIR)/GeneralFunc.cpp $(LIB_DIR)/gauss.cpp $(LIB_DIR)/check.cpp ../../OFDM/OFDM_baseline_nrvs_same_array_cyclic.cpp ../../Equalizing/Equalizer_baseline.cpp ../../fft/fft_baseline.cpp

OBJS := $(patsubst %.cpp, %.o, $(SRCS))

INC_FLAGS = -I.. -I../../ -I$(LIB_DIR) -I../../Equalizing -I../../OFDM

CXXFLAGS = -g -Wall
LINKFLAGS = 

CXXFLAGS += $(INC_FLAGS)

TARGETS = ResMapper.out

ResMapper.out: $(OBJS)
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(OBJS) $(TARGETS)
