PROG=mainMezclar
CXX=g++
FLAGS=
LIBS=-lsystemc
OBJS=mainMezclar.o mezclar.o

$(PROG): $(OBJS)
	$(CXX) $(FLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $ $< $(LIBS)

clean:
	rm -rf $(PROG) $(OBJS)
