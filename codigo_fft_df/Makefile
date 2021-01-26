PROG=mainMIPS-DF
CXX=g++
FLAGS=
LIBS=-lsystemc
OBJS=mainMIPS-DF.o MIPScore.o mmuMem.o

$(PROG): $(OBJS)
	$(CXX) $(FLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< $(LIBS)

clean:
	rm -rf $(PROG) $(OBJS)
