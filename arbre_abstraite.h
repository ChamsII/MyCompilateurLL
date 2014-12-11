#ifndef __ABSTRAITE_H_
#define __ABSTRAITE_H_


#include "unitesSyntaxiques.h"
#include "lexique.h"
#include  "dictionnaire.h"


void affiche_prog(void);   		/*   D�claration du programme  */

void affiche_listeDeclVariable(void);    	/* liste de declaration de variable */
void affiche_declVariable(void); 		/*   declaration des variables  */
void affiche_declTableau(void);			/*   D�claration d'un tableau  ID_VAR[NOMBRE]  */

void affiche_listeDeclFonction(void);   	/* liste de d�claration de fonction */
void affiche_declFonction(void);	 	/*   D�claration des fonction  */
//void listeParam(void);			/* liste de param�tre = une ( liste de variable ) */

void affiche_instruction(void);			/* D�claration d'une instruction  */
void affiche_instructionAffect(void);		/* Instruction d'affectation */
void affiche_instructionBloc(void);		/* instruction de bloc  { } */
void affiche_instructionSi(void);		/* instruction SI */
void affiche_instructionTantque(void);		/* instruction tanque */
void affiche_instructionAppel(void);		/* instruction appel */
void affiche_instructionRetour(void);		/* instruction retourner */
void affiche_instructionEcriture(void);		/* instruction �criture */
void affiche_instructionVide(void);		/* instruction vide */

//void listeInstruction(void);		/* liste d'instruction -> instruction  ListeInstruction | EPSILONE */
void affiche_AppelFct(void);			/* fonction appel fonction */

void affiche_expression(void);			/* declaration d'une expression */
void affiche_conjonction(void);			/* declaration d'une conjonction */
void affiche_negation(void);			/* negation d'une expression */
void affiche_comparaison(void);			/* comparaison d'expressions */
void affiche_expArith(void);			/* expression arithmetique */
void affiche_terme(void);			/* terme */
void affiche_facteur(void);			/*facteur */
//void var(void);				/* variable    $a */
void affiche_argumentEffectifs(int rechExecutable);		/* argument effectifs  (liste d'expression) */
//void listeExpressions(void);		/* liste d'expressions  -> expression ',' listeExpressions | EPSILONE */
#endif

