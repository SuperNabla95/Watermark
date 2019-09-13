all: main.cpp
	g++ -std=c++11 main.cpp -o watermark -L/usr/X11R6/lib -lm -lpthread -lX11

clean:
	$(RM) main
