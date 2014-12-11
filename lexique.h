#ifndef __LEXIQUE_H_
#define __LEXIQUE_H_


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "unitesLexicales.h"
#include "erreur.h"

/**************************************************************************************************************/
/** variable global */
int classe;// = 0;			/** unité lexical lu dans l'entrée */
//int principalFct = NON;
int valeur;						/** valeur NOMBRE lu dans l'entrée */
char *yytext;					/** chaine lu dans l'entrée */
char lexeme[15];

/****************************************************************************************************************/
/** Lecture du fichier  code.l **/
void lire_entree(void); 
/** car_suivant() : le caractere suivant à lire* */
void car_suivant(void);
/**   est_fini_fichier() : fin du fichier entré en paramèttre  */
int fin_fichier(int car);
/**  est_presentation() : supprimer les espace vide d'en haut du fichier  et les présentation machin */
int est_presentation(int car);
/**  est_espace() : supprimer les espace vide  */
int est_espace(int car);
/** est_debut_comm() : debut commentaire */
int est_debut_comm(int car);
/** est_fini_comm() : fin de commentaire par # ou saut de ligne \n */
int est_fini_comm(int car);
/** est_saut_ligne() :  saut de ligne \n */
int est_saut_ligne(int car);
/** est_lettre()  qui peut être soit majuscule ou minuscule */
	/** est_maj() : si c'est majiscule */
int est_maj(int car);
	/** est_min() : si c'est minuscule */
int est_min(int car);
	/** est_lettre() : combinaison entre majiscule et minuscule */
int est_lettre(int car);
/** est_chiffre() : si c'est un chiffre pour la determination des entier */
int est_chiffre(int car);
/** pour les identificateur avec melange de lettre et de chiffre */
int est_lettre_ou_chiffre(int car);
/** pour les tiré de 8 _ dans les identificateurs */
int est_souligne(int car);
/** est_operateur pour les opérateurs aurorisés : +,-,*,/,<,=,&,|,! */
int est_operateur(int car);
/** est_separateur() : pour les sépaateurs autorisés : () {} , ; */
int est_separateur(int car);
/** est_dollar() : dollar pour marquer le debut d'une variable (identificateur) */
int est_dollar(int car);
/** est_egale : agale  pour former ==  */
int est_egale(int car);
/**  debut_lex  : demarre la lecture du fichier */
void debut_lex(void);
/** Eliminer les commentaire et les espaces */
void espace_comm(void);
/** est_entier() : reconnaitre si c'est un entier  */
void est_entier(void);
/** est_identificateur() : reconnaitre si c'est un identificateur  */
void est_identificateur(void);
/** mettre_un_zero(depart, point) : chaine terminant par 0 pour la représentation du lexeme */
void mettre_un_zero(int depart, int point);
//char* mettre_un_zero(int depart, int point);
/** est_mot_cle() : vérifier si c'est un mot clé : si, ecrire, lire, ....  */
void est_mot_cle(void);
/** yylex() : sous programme annalyseur lex **/ 
int lex_suivant (void);
/** fonction yylex() : pilote de l'analyseur lexical */
int yylex();

void reinitialise(void);

#endif
