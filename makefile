EXECUTABLE = main
CFLAGS = -c -Wall -O5 -DDEBUG -Ilib
LDFLAGS =
LIBS = -lsfml-graphics -lsfml-system -lsfml-window -lconfig++





# main
all: $(EXECUTABLE)

run: $(EXECUTABLE)
	$(EXECUTABLE) 


rebuild: clean $(EXECUTABLE)

rerun: clean $(EXECUTABLE)
	$(EXECUTABLE)



$(EXECUTABLE): main.o Line.o GraphSolution.o
	g++ $(LDFLAGS) -o $(EXECUTABLE) main.o Line.o GraphSolution.o $(LIBS)

main.o: main.cpp
	g++ $(CFLAGS) -o main.o main.cpp


Line.o: Line.cpp
	g++ $(CFLAGS) -o Line.o Line.cpp

GraphSolution.o: GraphSolution.cpp
	g++ $(CFLAGS) -o GraphSolution.o GraphSolution.cpp






# clean
clean:
	-rm *.o $(EXECUTABLE)





# end
