CXX = c++
CXXFLAGS = -std=c++11

LEMON_HOME = /usr/local/lib

merge_graphs : merge_graphs.cpp usinglemon.h
	$(CXX) $(CXXFLAGS) -o $@ $< -I${LEMON_HOME}/include -L${LEMON_HOME}/lib -lemon
