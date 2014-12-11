CC = gcc
CFLAGS = -Wall -g

OBJ = erreur.o test_yylex.o syyntaxique.o abstraite.o machine.o dictionnaire.o arbre_abstraite.o

all: compilateur

compilateur: $(OBJ)
	@echo File Compilateur compiled ---> start running
	$(CC) $(CFLAGS) -o compilateur $(OBJ) 

erreur.o: erreur.c
	@echo Compiling: erreur.c
	$(CC) $(CFLAGS) -c erreur.c -o erreur.o

dictionnaire.o: dictionnaire.c
	@echo Compiling: dictionnaire.c
	$(CC) $(CFLAGS) -c dictionnaire.c -o dictionnaire.o

test_yylex.o: test_yylex.c
	@echo Compiling: test_yylex.c
	$(CC) $(CFLAGS) -c test_yylex.c -o test_yylex.o 

syyntaxique.o:  syyntaxique.c
	@echo Compiling: syyntaxique.c
	$(CC) $(CFLAGS) -c syyntaxique.c -o syyntaxique.o
	
machine.o: machine.c
	@echo Compiling: machine.c
	$(CC) $(CFLAGS) -c machine.c -o machine.o
	
abstraite.o: abstraite.c
	@echo Compiling: abstraite.c
	$(CC) $(CFLAGS) -c abstraite.c -o abstraite.o

arbre_abstraite.o: arbre_abstraite.c
	@echo Compiling: arbre_abstraite.c
	$(CC) $(CFLAGS) -c arbre_abstraite.c -o arbre_abstraite.o

erreur.c: erreur.h

lexique.h: unitesLexicales.h erreur.h

test_yylex.c: lexique.h 

grammaire.h: unitesSyntaxiques.h dictionnaire.h lexique.h

arbre_abstraite.h : unitesSyntaxiques.h dictionnaire.h lexique.h

syyntaxique.c: grammaire.h

abstraite.c: unitesSyntaxiques.h

machine.c : unitesSyntaxiques.h 

dictionnaire.c : dictionnaire.h

arbre_abstraite.c : arbre_abstraite.h 

.PHONY: delete_all

clean_all: 
	@echo Delete $(OBJ) compilateur
	@echo Delete $(OBJ) compilateur
	-rm -f $(OBJ) compilateur
	
help :
	@echo
	@echo '  options du make : '
	@echo
	@echo '     all ............. (re)compiler le projet'
	@echo '     clean_all ....... nettoyer les sources'
	@echo '     help ............ afficher ce menu'
	@echo



