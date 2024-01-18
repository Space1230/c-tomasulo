
objects = main.o utils.o commands.o ROB.o RS.o Reg.o

tomasulo : $(objects)
	cc -o tomasulo $(objects)

main.o : utils.h commands.h ROB.h RS.h Reg.h
utils.o:
commands.o:
ROB.o: utils.h commands.h
RS.o: main.h utils.h commands.h
Reg.o: utils.h

.Phony: run clean

run: tomasulo
	./tomasulo

clean:
	rm tomasulo $(objects)
