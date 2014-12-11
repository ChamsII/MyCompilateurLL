/**
 * @file syyntaxique.c
 * @brief Fonctions du correcteur des mots franÃ§ais
 */
 

#include "arbre_abstraite.h"

int trace_abss = 1;
char variable[15];
/**************************************************************************************************************/


/*** Fonction création du programme */
void affiche_prog(void) {
  char *fct = "prog";
  balise_ouvrante(fct, trace_abss);
	while (classe != FDF){
		if(classe == ENTIER) { affiche_listeDeclVariable(); }
		if (classe == ID_FCT) { affiche_listeDeclFonction(); }
	}
  balise_fermante(fct, trace_abss);
}

/*************************************** DECLARATION DE VARIABLE ID_VAR *****************************************/
/*** Fonction liste de declaration de variable */
void affiche_listeDeclVariable(void){
	char *fct = "l_Decl";
	balise_ouvrante(fct, trace_abss);
	
	do{
		affiche_declVariable();		/* vérification si c'est une variable */
		if(classe == ';') break;
		else if(classe == ',') { classe = yylex(); continue; }
		else if(classe == '[') { 
				affiche_declTableau(); 
				if(classe == ',') { classe = yylex(); continue; }
			}
			else { printf("point virgule dans listeDeclVariable \n"); erreur(ERR_SYNT,";"); }
	}while(classe != ';');
	balise_fermante(fct, trace_abss);
	classe = yylex();
}
 

/***  Fonction déclaration d'une variable */
void affiche_declVariable(void){
  classe = yylex();
  memset (variable, 0, sizeof (15));
  sprintf(variable, "%s", yytext);
  classe = yylex();
  if(classe == ';' || classe == ','){
	    affiche_element("decVar",variable, trace_abss);
  }
}

/***   Fonction déclaration d'un tableau d'entier ENTIER ID_VAR [NOMBRE]  */
void affiche_declTableau(void){
  char texte[15];
  classe = yylex();
  classe = yylex();
  sprintf(texte, "%s[%d]", variable, valeur);
  affiche_element("decTab",texte, trace_abss);
  classe = yylex();
  
}

/*****************************************************************************************************************/

/***************************************** DECLARATION DE FONCTION ***********************************************/
void affiche_declFonction(void){
  char *fct = "declFonc";
  balise_ouvrante(fct, trace_abss);
  affiche_texte(yytext, trace_abss);
  classe = yylex();
  classe = yylex();
  while (classe == ENTIER){
	classe = yylex();
	affiche_element("decVar",yytext, trace_abss);
    classe = yylex();
    if (classe == ')')
      break;
    classe = yylex();
  }
  classe = yylex();
  if (classe == ENTIER){
	do{
		affiche_declVariable();		/* vérification si c'est une variable */
		if(classe == ';') break;
		else if(classe == ',') {classe = yylex(); continue; }
		else if(classe == '[') { 
				affiche_declTableau(); 
				if(classe == ',') {classe = yylex(); continue; }
			}
			else { printf("';' declFonction --> declVariable \n"); erreur(ERR_SYNT,";"); }
	}while(classe != ';');	
	classe = yylex();
  }
  affiche_instructionBloc();
  balise_fermante(fct, trace_abss);
}

/*** Fonction liste de declaration de fonction */
void affiche_listeDeclFonction(void){
	char *fct = "l_Decl";
	balise_ouvrante(fct, trace_abss);
	do{
		affiche_declFonction();		
	}while(classe == ID_FCT);
	balise_fermante(fct, trace_abss);
}
/*****************************************************************************************************************/
/************************************************ INSTRUCTIONS  **************************************************/

/*** Fonction instruction appelant tout les instructions */
void affiche_instruction(void){	
	switch(classe){
	  case ID_VAR:      	affiche_instructionAffect();   	break;
	  case '{':   			affiche_instructionBloc();     	break;
	  case SI:           	affiche_instructionSi();       	break;
	  case TANTQUE:      	affiche_instructionTantque();  	break;
	  case ID_FCT:        	affiche_instructionAppel();    	break;
	  case RETOUR:     		affiche_instructionRetour();	break;
	  case ECRIRE:       	affiche_instructionEcriture();  break;
	  case ';': 			affiche_instructionVide();     	break;
	  default:  erreur(ERR_SYNT, "Instruction");
  }
}

/*** Fonction instruction d'affectation  = */
void affiche_instructionAffect(void){
  char *fct = "inst_affect";
  balise_ouvrante(fct, trace_abss);
  affiche_element("var_indicee",yytext, trace_abss);
  classe = yylex();
  
  if (classe == '['){ 
    classe = yylex();
    affiche_expression();
    classe = yylex();
  }
  classe = yylex();
  affiche_expression();
  balise_fermante(fct, trace_abss);
  classe = yylex();
}

/****  Fonction  instruction bloc {   */
void affiche_instructionBloc(void){
  char *fct = "instr_Bloc";
  balise_ouvrante(fct, trace_abss);
  classe = yylex();
  while (classe != '}'){ 
	affiche_instruction(); 
	}
  balise_fermante(fct, trace_abss);
  classe = yylex();
}

/***  Fonction instruction si   */
void affiche_instructionSi(void){
  char *fct = "instr_si";
  balise_ouvrante(fct, trace_abss);
  classe = yylex();
  affiche_expression();
  classe = yylex();
  affiche_instruction();

  if (classe == SINON){
    affiche_element("mot_clef",yytext, trace_abss);
    classe = yylex();
    affiche_instruction();
  }
  balise_fermante(fct, trace_abss);
}


/*** Fonction instruction tantque  */
void affiche_instructionTantque(void){
  char *fct = "instr_tantque";
  balise_ouvrante(fct, trace_abss);
  classe = yylex();
  affiche_expression();
  classe = yylex();
  affiche_instruction();
  balise_fermante(fct, trace_abss);
}


/***   Fonction argumentEffectifs **/
void affiche_argumentEffectifs(int recheExecutable){
  if (classe != ')'){
    affiche_expression();
    while (classe == ','){
      classe = yylex();
      affiche_expression();
    }
  }
}


/***   Fonction Appelfct : appel de fonction */
void affiche_AppelFct(void) {
  char *fct = "appel_fct";
  balise_ouvrante(fct, trace_abss);
  affiche_texte(yytext, trace_abss);
  classe = yylex();
  classe = yylex();
  affiche_argumentEffectifs(0);
  classe = yylex();
  balise_fermante(fct, trace_abss);
}

/*** Fonction instruction appel  */
void affiche_instructionAppel(void){
  char *fct = "instr_appel";
  balise_ouvrante(fct, trace_abss);
  affiche_AppelFct(); 
  classe = yylex();
  balise_fermante(fct, trace_abss);
}


/***    Fonction instruction Retour   */
void affiche_instructionRetour(void){
  char *fct = "instr_retour";
  balise_ouvrante(fct, trace_abss);
  classe = yylex();  
  affiche_expression();
  classe = yylex();
  balise_fermante(fct, trace_abss);
}

/***    Fonction instruction ecrire  */
void affiche_instructionEcriture(void){
	  char *fct = "instr_ecriture";
      balise_ouvrante(fct, trace_abss);
	  classe = yylex();
	  classe = yylex();
	  affiche_expression();
	  classe = yylex();
	  classe = yylex();
	  balise_fermante(fct, trace_abss);
}

/***  Fonction instruction vide */
void affiche_instructionVide(void){
  char *fct = "instr_vide";
  balise_ouvrante(fct, trace_abss);
  classe = yylex();
  balise_fermante(fct, trace_abss);
}


/***************************************************************************************************************/
/**********************************************  OPPERATIONS **************************************************/

/***   Fonction   expression()      */
void affiche_expression(void)
{  	
	char *fct = "opExpre";
	balise_ouvrante(fct, trace_abss);
	affiche_conjonction();
	while(classe == '|'){
		affiche_texte("ou", trace_abss);
		classe=yylex();
		affiche_conjonction();
	}  
	balise_fermante(fct, trace_abss);
}

/***  Fonction   conjonction()  */
void affiche_conjonction(void)
{  
	affiche_negation(); 
	while(classe == '&'){
	    affiche_texte("et", trace_abss);
		classe = yylex();
		affiche_negation();
	}
}


/*** Fonction negation()  */
void affiche_negation(void)
{	
	if(classe == '!'){
		affiche_texte("non", trace_abss);
		classe = yylex();
	}
	affiche_comparaison();
	while(classe == '!'){
		affiche_texte("non", trace_abss);
		classe = yylex();
		affiche_comparaison();
	}
}

/*** Fnction comparaison()   */
void affiche_comparaison(void){
  affiche_expArith();
  switch(classe){
	case EGAL:
		affiche_texte("egal", trace_abss);
	    classe = yylex();
	    affiche_expArith();
	break;
	case '=':
		affiche_texte("egal", trace_abss);
	    classe = yylex();
	    affiche_expArith();
	break;
	case '<' :
		affiche_texte("inf", trace_abss);
	    classe = yylex();
	    affiche_expArith();
	 break; 
  }
}


/***  Fonction expArith()   */
void affiche_expArith(void){
	affiche_terme();
	while (classe == '+' || classe == '-'){
		if (classe == '+')
			affiche_texte("plus", trace_abss);
		else
			affiche_texte("moins", trace_abss);
	    classe = yylex();
	    affiche_terme();
	}
}


/*** Fonction terme()  */
void affiche_terme(void){	
	affiche_facteur();
	while (classe == '*' || classe == '/'){
		if (classe == '*')
			affiche_texte("fois", trace_abss);
		else
			affiche_texte("divise", trace_abss);
			
	    classe = yylex();
	    affiche_facteur();
	  }
}


/*** Fonction facteur()  */
void affiche_facteur(void){
	  switch (classe){
		  case '(':{
			classe = yylex();
			affiche_expression();
			classe = yylex();
			break;
		  }

		  case NOMBRE:{
			affiche_element("intExpr",yytext, trace_abss);
			classe = yylex();
			break;
		  }

		  case ID_FCT:{
			affiche_texte(yytext, trace_abss);
			classe = yylex();			
			classe = yylex();
			affiche_argumentEffectifs(0);		
			classe = yylex();
			break;
		  }
		  
		  case ID_VAR : {	
			affiche_element("var_simple",yytext, trace_abss);
		    classe = yylex();
			if (classe == '['){
			  classe = yylex();
			  affiche_expression();
			  classe = yylex();
			}
			break;
		   }
			
		  case LIRE:{
			classe = yylex();
			classe = yylex();
			classe = yylex();
			break;
		  }
		  default : { 
			printf ("%s - %d \n", yytext, classe);
		    erreur(ERR_SYNT, "expression");
			}
	  }
}



