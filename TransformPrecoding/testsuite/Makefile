
SRC_DIR = ..
LIB_DIR = ../../lib

SRCS = $(SRC_DIR)/TransformPreMain.cpp $(SRC_DIR)/TransformPrecoder_base.cpp
SRCS += ../../lte_phy.cpp $(LIB_DIR)/GeneralFunc.cpp $(LIB_DIR)/gauss.cpp $(LIB_DIR)/check.cpp ../../dft/dft_base.cpp

OBJS := $(patsubst %.cpp, %.o, $(SRCS))

INC_FLAGS = -I.. -I../../ -I$(LIB_DIR)

CXXFLAGS = -g -Wall
LINKFLAGS = 

CXXFLAGS += $(INC_FLAGS)

TARGETS = TPTD.out

TPTD.out: $(OBJS)
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(OBJS) $(TARGETS)
