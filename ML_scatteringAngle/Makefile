ROOTCXXFLAGS := $(shell root-config --cflags --glibs)
LIBS := -lMinuit -lRooFitCore


max_L: max_L.cpp
	$(CXX) -g $< -o $@.exe $(ROOTCXXFLAGS) $(LIBS)


all: max_L minuit2_fit