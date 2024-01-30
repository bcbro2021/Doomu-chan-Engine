gcc -c src/main.c -lSDL2 -lm
gcc -c src/doomu_chan/base.c -lSDL2 -lm
gcc -c src/doomu_chan/window.c -lSDL2 -lm

gcc -o main main.o base.o window.o -lSDL2 -lm