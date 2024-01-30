gcc -c src/main.c -lGL -lGLU -lglut -lm
gcc -c src/doomu_chan/base.c -lGL -lGLU -lglut -lm
gcc -c src/doomu_chan/render.c -lGL -lGLU -lglut -lm

gcc -o main main.o base.o render.o -lGL -lGLU -lglut -lm