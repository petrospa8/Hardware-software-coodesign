PROG=Main_methods
CXX=g++
FLAGS=
LIBS=-lsystemc
OBJS=Main_methods.o ALU.o bancoRegistros.o

$(PROG): $(OBJS)
	$(CXX) $(FLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $ $< $(LIBS)

clean:
	rm -rf $(PROG) $(OBJS)
