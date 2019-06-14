ROOTCXXFLAGS := $(shell root-config --cflags --glibs)
LIBS := -lMinuit -lRooFitCore

# all: max_L minuit2_fit

max_L: max_L.cpp
	$(CXX) -g $< -o $@.exe $(ROOTCXXFLAGS) $(LIBS)


minuit2_fit: minuit2_fit.cpp
	$(CXX) -g $< -o $@.exe $(ROOTCXXFLAGS) 

roofit_gauss: roofit_gauss.cpp
	$(CXX) -g $< -o $@.exe $(ROOTCXXFLAGS) $(LIBS)

all: max_L minuit2_fit