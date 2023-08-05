executar: compilar
		./jogo

compilar:
	gcc *.c -c -g
	gcc *.o -o jogo -lm -lGLU -lglfw -lGLEW -lGL -lglut -lSOIL 

limpar:
	rm -rf jogo *.o