OBJS=analyser.o brlToken.o main.o libmecab.lib
LIBS=
main.exe: $(OBJS)
	@g++ $(OBJS) $(LIBS) -o main.exe

.PHONY: clean
clean:
	rm -f *.o

.cpp.o:
	@g++ -c $<
