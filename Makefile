
tomasulo : tomasulo.c
	cc -o tomasulo tomasulo.c

.Phony: run clean

run: tomasulo
	./tomasulo

clean:
	rm tomasulo
