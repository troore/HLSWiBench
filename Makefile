
LIB_SRC = GeneralFunc.cpp gauss.cpp lte_phy.cpp

LIB_OBJ = $(patsubst %.cpp, %.o, $(wildcard $(LIB_SRC)))

CXXFLAGS = -g

Turbo: Turbo.o TurboMain.o $(LIB_OBJ)
	$(CXX) -o $@ $^ $(LINKFLAGS)

Mod: Modulation.o ModMain.o $(LIB_OBJ)
	$(CXX) -o $@ $^ $(LINKFLAGS)

ResMap: ResMapper.o ResMapMain.o $(LIB_OBJ)
	$(CXX) -o $@ $^ $(LINKFLAGS)

Eq: Equalizer.o EqualizerMain.o ResMapper.o $(LIB_OBJ)
	$(CXX) -o $@ $^ $(LINKFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o
