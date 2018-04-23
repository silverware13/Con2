all :
	gcc -std=c99 -pthread -o concurrency2 concurrency2.c

clean:
	rm concurrency2
