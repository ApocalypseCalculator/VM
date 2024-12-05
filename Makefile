CXX=g++-14
CXXFLAGS=-std=c++20 -Wall -g -lncurses -MMD
srcfiles := $(wildcard */*.cc) $(wildcard *.cc)
objects=$(patsubst %.cc,%.o,$(srcfiles))
vm: $(objects)
	$(CXX) $(CXXFLAGS) -o vm $(objects)
.PHONY: clean
clean:
	rm *.o **/*.o *.d **/*.d vm || true
-include $(objects:.o=.d)