exe:main.o libmath.so
	gcc -o exe main.o -L . -lmath -lm
common_math:common_math/common_math.o
	gcc -g -c -I common_math -fPIC common_math/common_math.c -o common_math/common_math.o
complex_math:complex_math/complex_math.o
	gcc -g -c -I complex_math -fPIC complex_math/complex_math.c -o complex_math/complex_math.o
trig_math:trig_math/trig_math.o
	gcc -g -c -I trig_math -fPIC trig_math/trig_math.c -o trig_math/trig_math.o
libmath.so:trig_math/trig_math.o common_math/common_math.o complex_math/complex_math.o
	gcc -shared -o libmath.so common_math/common_math.o complex_math/complex_math.o trig_math/trig_math.o
main.o:
	gcc -g -c -I common_math -I complex_math -I trig_math main.c -o main.o
clean:
	rm common_math/common_math.o
	rm complex_math/complex_math.o
	rm trig_math/trig_math.o
	rm main.o
	rm libmath.so
	rm exe
