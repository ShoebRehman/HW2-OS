all:
	gcc -std=c99 -lm -pthread thread.c -o thread.exe
	
clean:
	rm thread.exe

