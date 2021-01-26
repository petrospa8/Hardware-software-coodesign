PROG=Main_methods
CXX=g++
FLAGS=
LIBS=-lsystemc
OBJS=Main_methods.cpp mcd.o

$(PROG): $(OBJS)
	$(CXX) $(FLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $ $< $(LIBS)

clean:
	rm -rf $(PROG) $(OBJS)
