OBJS=analyser.o brlToken.o main.o parser.o libmecab.lib
LIBS=
main.exe: $(OBJS)
	@g++ $(OBJS) $(LIBS) -static -o main.exe

.PHONY: clean
clean:
	rm -f *.o

.cpp.o:
	@g++ -c $<
