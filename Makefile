CXX=g++-14
CXXFLAGS=-std=c++20 -Wall -g

objects=action/action.o action/insert.o controller/controller.o controller/parser.o controller/curseskb.o view/view.o view/cmdbarview.o view/fileview.o model/model.o model/vmstate.o test.o

vm: $(objects)
	$(CXX) $(CXXFLAGS) -o vm $(objects) -lncurses

$(objects): %.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ -lncurses

.PHONY: clean
clean:
	rm *.o **/*.o vm