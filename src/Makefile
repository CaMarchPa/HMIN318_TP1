#MAKEFILE
#all: TP1

#run: TP1
#	./main ../img/MR_head.Coronal.hdr

main: main.o
	g++ -o TP1 main.o

main.o: main.cpp CImg.h
	g++ -c main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

clean:
	rm -f *.o *~ main
