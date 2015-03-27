all:
	gcc -std=c99 -lm -pthread pidparent.c -o pidparent.exe
	
clean:
	rm pidparent.exe

