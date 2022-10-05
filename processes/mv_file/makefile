FUNC= ../source/func.c

myexe: $(FUNC)
	gcc $(FUNC) -c
	gcc func.o mvP.c -o myexe

clean:
	rm -f func.o
	rm -f myexe
