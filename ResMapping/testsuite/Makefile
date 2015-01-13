
SRC_DIR = ..
LIB_DIR = ../../lib

SRCS = $(SRC_DIR)/ResMapMain.cpp $(SRC_DIR)/ResMapper_base.cpp ../../dmrs/dmrs.cpp
SRCS += ../../lte_phy.cpp $(LIB_DIR)/GeneralFunc.cpp $(LIB_DIR)/gauss.cpp $(LIB_DIR)/check.cpp

OBJS := $(patsubst %.cpp, %.o, $(SRCS))

INC_FLAGS = -I.. -I../../ -I$(LIB_DIR)

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
