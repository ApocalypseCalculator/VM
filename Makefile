CXX=g++-14
CXXFLAGS=-std=c++20 -Wall -g -lncurses

objects=action/action.o action/insert.o controller/controller.o controller/parser.o controller/curseskb.o view/view.o view/cmdbarview.o view/fileview.o model/model.o model/vmstate.o test.o

vm: $(objects)
	$(CXX) $(CXXFLAGS) -o vm $(objects)

.PHONY: clean
clean:
	rm *.o **/*.o vm || true