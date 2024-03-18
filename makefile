CPPFLAGS += -Iinclude -I/usr/include/cairo
LDFLAGS += -lcairo -lm -lX11

ifeq ($(MODE), TEXTE)
main : 	main.o grille.o io.o jeu.o
	gcc -g -Wall -o main obj/main.o obj/grille.o obj/io.o obj/jeu.o $(CPPFLAGS) $(LDFLAGS)
	mkdir bin
	mv main bin

else
main : 	visual_main.o grille.o io.o jeu.o lib
	gcc -g -Wall -o main libjeu.a $(CPPFLAGS) $(LDFLAGS)
	mkdir bin
	mv main bin
	
endif
	
main.o : src/main.c include/grille.h include/io.h include/jeu.h
		gcc -g -c src/main.c $(CPPFLAGS) $(LDFLAGS)
		mkdir obj
		mv main.o obj

visual_main.o : src/visual_main.c include/grille.h include/io.h include/jeu.h
		gcc -g -c src/visual_main.c $(CPPFLAGS) $(LDFLAGS)
		mkdir obj
		mv visual_main.o obj
		
grille.o : src/grille.c include/grille.h
		gcc -g -c src/grille.c
		mv grille.o obj
		
		
io.o : src/io.c include/io.h include/grille.h include/jeu.h
		gcc -g -c src/io.c
		mv io.o obj
		

jeu.o : src/jeu.c include/jeu.h include/grille.h
		gcc -g -c src/jeu.c
		mv jeu.o obj

lib : 
	ar -crv libjeu.a obj/visual_main.o obj/grille.o obj/io.o obj/jeu.o
	ranlib libjeu.a

docs :
		doxygen Doxyfile
		mkdir doc
		mv html doc
		mv latex doc


dist :
		tar -cvf KrikaHamza-GoL-5.0.0.tar.xz makefile src/grille.c include/grille.h src/io.c include/io.h src/jeu.c include/jeu.h src/main.c src/visual_main.c Doxyfile

clean : 
		rm -R -f *.o *.a main main.exe doc bin obj

