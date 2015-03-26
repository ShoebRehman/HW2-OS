all:
	gcc -std=c99 -lm -pthread pidparent.c -o pidparent.exe
	gcc -std=c99 -lm -pthread pidchild.c -o pidchild

clean:
	rm pidparent.exe pidchild

