#ifndef __GRAMMAIRE_H_
#define __GRAMMAIRE_H_


#include "unitesSyntaxiques.h"
#include "lexique.h"
#include  "dictionnaire.h"


void programme(void);   		/*   Déclaration du programme  */

void listeDeclVariable(void);    	/* liste de declaration de variable */
void declVariable(void); 		/*   declaration des variables  */
void declTableau(void);			/*   Déclaration d'un tableau  ID_VAR[NOMBRE]  */

void listeDeclFonction(void);   	/* liste de déclaration de fonction */
void declFonction(void);	 	/*   Déclaration des fonction  */
//void listeParam(void);			/* liste de paramètre = une ( liste de variable ) */

void instruction(void);			/* Déclaration d'une instruction  */
void instructionAffect(void);		/* Instruction d'affectation */
void instructionBloc(void);		/* instruction de bloc  { } */
void instructionSi(void);		/* instruction SI */
void instructionTantque(void);		/* instruction tanque */
void instructionAppel(void);		/* instruction appel */
void instructionRetour(void);		/* instruction retourner */
void instructionEcriture(void);		/* instruction écriture */
void instructionVide(void);		/* instruction vide */

//void listeInstruction(void);		/* liste d'instruction -> instruction  ListeInstruction | EPSILONE */
void AppelFct(void);			/* fonction appel fonction */

void expression(void);			/* declaration d'une expression */
void conjonction(void);			/* declaration d'une conjonction */
void negation(void);			/* negation d'une expression */
void comparaison(void);			/* comparaison d'expressions */
void expArith(void);			/* expression arithmetique */
void terme(void);			/* terme */
void facteur(void);			/*facteur */
//void var(void);				/* variable    $a */
void argumentEffectifs(int rechExecutable);		/* argument effectifs  (liste d'expression) */
//void listeExpressions(void);		/* liste d'expressions  -> expression ',' listeExpressions | EPSILONE */
#endif


/*

	programme -> [ listeDeclVariables ';' ]
			listeDeclFonctions
			'.'

	listeDeclVariables -> declVariable [ ',' listeDeclVariables ]
	declVariable -> ENTIER ID_VAR [ '[' NOMBRE ']' ]
	listeDeclFonctions -> declFonction listeDeclFonctions | EPSILON
	declFonction -> ID_FCT listeParam
			[ listeDeclVariables ';' ]
			instructionBloc

	listeParam -> '(' [ listeDeclVariables ] ')'
	instruction -> instructionAffect
			| instructionBloc
			| instructionSi
			| instructionTantque
			| instructionAppel
			| instructionRetour
			| instructionEcriture
			| instructionVide
	instructionBloc -> '{' listeInstructions '}'
	listeInstructions -> instruction ListeInstructions | EPSILON
	instructionAppel -> appelFct ';'
	instructionAffect -> var '=' expression ';'
	instructionSi -> SI expression ALORS '{' instruction '}' [ SINON '{' instruction '}' ]
	instructionTantque -> TANTQUE expression FAIRE '{' instruction '}'
	instructionRetour -> RETOUR expression ';'
	instructionEcriture -> ECRIRE '(' expression ')' ';'
	instructionVide -> ';'
	expression -> conjonction [ '|' expression ]
	conjonction -> negation [ '&' conjonction]
	negation -> [ '!' ] comparaison
	comparaison -> expArith [ ( '=' | '<' ) expArith ]
	expArith -> terme [ ( '+' | '-' ) expArith ]
	terme -> facteur [ ( '*' | '/' ) terme]
	facteur -> '(' expression ')'
			| NOMBRE
			| appelFct
			| var
			| LIRE '(' ')'
	var -> ID_VAR [ '[' expression ']' ]
	appelFct -> ID_FCT argumentsEffectifs
	argumentsEffectifs -> '(' listeExpressions ')'
	listeExpressions -> expression listeExpressions2 | EPSILON
	listeExpressions2 -> ',' expression listeExpressions2 | EPSILON


*/
