
objects = main.o

tomasulo : $(objects)
	cc -o tomasulo $(objects)

.Phony: run clean

run: tomasulo
	./tomasulo

clean:
	rm tomasulo $(objects)
