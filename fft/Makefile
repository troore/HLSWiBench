
#CFLAGS = -g -DDEBUG
CFLAGS = -g

fft: fft_baseline.cpp fft_main.cpp
	g++ -o $@ $^ $(CFLAGS)
clean:
	rm -rf fft *.o
