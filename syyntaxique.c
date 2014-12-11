/**
 * @file syyntaxique.c
 * @brief Fonctions du correcteur des mots franÃ§ais
 */
 
#include "grammaire.h"  
#include "dictionnaire.h" 
#define FAUX          (0)
#define VRAI          (1)



int rechIdentif = -1;
int rechExecutable = -1;
int trace_abs = 1;
int principalFct = FAUX;
//char choix[200];

int adrLocCrt = 0;
int contexte = C_VAR_GLOBALE;

int nbVarLoc;
int nbArgs;                     /* nombre d'argument de la fonction en cours d'analyse */
int retour = FAUX;                  /* prend la valeur VRAI si la fontion en cours d'analyse renvoie une expression, FAUX sinon */ 
/********************************************************************************************************/
/********************************************** ANALYSEUR SYNTAXIQUE ********************************************/
/****************************************************************************************************************/

/**************************************************************************************************************/
/************************ Pile d'execution : ***********************/
void ajouterCode(int instruction){
  if (tailMem > MAX_MEM)
    erreur(ERR_OUT_MEM, "compilation");
  mem[tailMem++] = instruction;
}

/**************************************************************************************************************/


/*** Fonction création du programme */
void programme(void) {
  int indice, identifiant;
  char *fct = "programme";
  balise_ouvrante(fct, trace_abs);
  adrGlobCrt = 0;
  tailMem = 0;
  ajouterCode(PILE);
  ajouterCode(1);
  ajouterCode(APPELFCT);
  indice = tailMem++;
  ajouterCode(STOP);
  
	while (classe != FDF){
		
		if(classe == ENTIER) { listeDeclVariable(); }
		if (classe == ID_FCT) {listeDeclFonction(); }
	}

  if (principalFct == FAUX)
     erreur(ERR_SEM, "fonction 'main' absente");
	
  identifiant = rechercheExecutable("main");
  mem[indice] = dico.tab[identifiant].adresse;

  if (classe != FDF){
    erreur(ERR_SYNT, "fin du programme");
  }
  /**** destruction du dictionnaire DICO  ****/

  
  /*************************************/
  balise_fermante(fct, trace_abs);
  
}

/*************************************** DECLARATION DE VARIABLE ID_VAR *****************************************/
/*** Fonction liste de declaration de variable */
void listeDeclVariable(void){
	char *fct = "listeDeclVariable";
	balise_ouvrante(fct, trace_abs);
	
	do{
		declVariable();		/* vérification si c'est une variable */
		if(classe == ';') break;
		else if(classe == ',') { affiche_element("symbole",yytext, trace_abs); classe = yylex(); continue; }
		else if(classe == '[') { 
				declTableau(); 
				if(classe == ',') {affiche_element("symbole",yytext, trace_abs); classe = yylex(); continue; }
			}
			else { printf("point virgule dans listeDeclVariable \n"); erreur(ERR_SYNT,";"); }
	}while(classe != ';');
	affiche_element("symbole",yytext, trace_abs);
	balise_fermante(fct, trace_abs);
	classe = yylex();
}
 

/***  Fonction déclaration d'une variable */
void declVariable(void){
  char *fct = "declVariable";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != ENTIER){		/* Depuis de declaration de variable : entier */
	printf("entier declVariable \n");
    erreur(ERR_SYNT, "entier");
	}
  affiche_element("mot_clef",yytext, trace_abs);
  classe = yylex();
  
  if (classe != ID_VAR)	{       	/* vérification si c une variable : expl : $variable */
	printf("id_var declVariable \n");
    erreur(ERR_SYNT, "identificateur");
	}
	
  affiche_element("id_variable",yytext, trace_abs);
  rechIdentif = rechercheDeclarative(yytext);

  if (rechIdentif >= 0)
    erreur(ERR_SEM, "identificateur deja declare : declVariable");

  rechIdentif = ajouter(yytext);
  //dico.tab[rechIdentif].classe = contexteLocal ? C_VAR_LOCALE : C_VAR_GLOBALE;

  
  classe = yylex();
  if(classe == ';' || classe == ','){
    dico.tab[rechIdentif].type = T_ENTIER;
    if (contexte == C_VAR_LOCALE){
      dico.tab[rechIdentif].classe = C_VAR_LOCALE;
      dico.tab[rechIdentif].adresse = adrLocCrt + 1;
    }
    else{
      dico.tab[rechIdentif].classe = C_VAR_GLOBALE;
      dico.tab[rechIdentif].adresse = adrGlobCrt + 1;
    }
  }
  else if(classe == '['){
    dico.tab[rechIdentif].type = T_TABLEAU;
	//printf("puisq c tableau alors le type de %d est %d \n", rechIdentif,dico.tab[rechIdentif].type);
	}
	
	affiche_element("symbole",yytext, trace_abs);
	balise_fermante(fct, trace_abs);
}

/***   Fonction déclaration d'un tableau d'entier ENTIER ID_VAR [NOMBRE]  */
void declTableau(void){
  int taille;
  char *fct = "declTableau";
  balise_ouvrante(fct, trace_abs);
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();

  if (classe != NOMBRE){
		printf("nombre declTableau \n");
		erreur(ERR_SYNT, "nombre entier dans un tableau");
   }
   
   affiche_element("nombre",yytext, trace_abs);
   taille = rechercheDeclarative(dico.tab[rechIdentif].identif);
  // printf("valeur du tableau %d et notre chere type est  %d\n", valeur, taille);
   
  if (valeur <= 0)
    erreur(ERR_SEM, "un tableau doit avoir au minimum une case : declTableau");
	
  dico.tab[taille].type = T_TABLEAU;
  dico.tab[rechIdentif].complement = valeur;

  if (contexte == C_VAR_LOCALE)
    dico.tab[rechIdentif].classe = C_VAR_LOCALE;
  else{
    dico.tab[rechIdentif].classe = C_VAR_GLOBALE;
    dico.tab[rechIdentif].adresse = adrGlobCrt;
    adrGlobCrt += valeur;
  }
  
  classe = yylex();
  //printf("la classe %d \n", classe);
  if (classe != ']' ){
	 printf("']' declTableau \n");
     erreur(ERR_SYNT, "]");
	}

  affiche_element("symbole",yytext, trace_abs);
  balise_fermante(fct, trace_abs);
  classe = yylex();
  
}

/*****************************************************************************************************************/

/***************************************** DECLARATION DE FONCTION ***********************************************/
void declFonction(void){
  int id2, i;
  char *fct = "declFonction";
  balise_ouvrante(fct, trace_abs);
  //entreeFonction();
  
  if (classe != ID_FCT) {
	 printf("id_fct declFonction \n");
     erreur(ERR_SYNT, "fonction");
	}

  if (strcmp(yytext, "main") == 0){
    principalFct = VRAI;
  }
  affiche_element("id_fonction",yytext, trace_abs);
  rechExecutable = rechercheExecutable(yytext);

  if (rechExecutable >= 0)
    erreur(ERR_SEM, "nom de fonction deja declare : declFonction");

  rechExecutable = ajouter(yytext);
  dico.tab[rechExecutable].classe = C_FONCTION;
  dico.tab[rechExecutable].adresse = tailMem;
  
  ajouterCode(ENTREE);
  classe = yylex();
  if (classe != '('){
	 printf("'(' declFonctione \n");
     erreur(ERR_SYNT, "'('");
	}
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  /******** creation du dico local *********/
  entreeFonction();
  /*****************************************/
  while (classe == ENTIER){
    affiche_element("mot_clef",yytext, trace_abs);
	classe = yylex();
    id2 = rechercheDeclarative(yytext);

    if (id2 >= 0)
      erreur(ERR_SEM, "variable fonction deja declare : declFonction");

	affiche_element("id_variable",yytext, trace_abs);
    id2 = ajouter(yytext);
    dico.tab[id2].classe = C_ARG;
    dico.tab[id2].type = T_ENTIER;
    dico.tab[rechExecutable].complement++;
	
    classe = yylex();
    if (classe == ')')
      break;
	  
    if (classe != ','){
	  printf("',' declFonction \n");
      erreur(ERR_SYNT, "','");
	 }
	 
	affiche_element("symbole",yytext, trace_abs);
    classe = yylex();
	if (classe != ENTIER){
	  printf("ENTIER ',' virgule en exccès : declFonction \n");
      erreur(ERR_SYNT, "entier");
	}
  }
  
  if (classe != ')'){
		printf("')' declFonction \n");
		erreur(ERR_SYNT, "')'");
	}
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  
  nbArgs = dico.tab[rechExecutable].complement;

  if (classe == ENTIER){
	
	do{
		declVariable();		/* vérification si c'est une variable */
		if(classe == ';') break;
		else if(classe == ',') {affiche_element("symbole",yytext, trace_abs); classe = yylex(); continue; }
		else if(classe == '[') { 
				declTableau(); 
				if(classe == ',') { affiche_element("symbole",yytext, trace_abs); classe = yylex(); continue; }
			}
			else { printf("';' declFonction --> declVariable \n"); erreur(ERR_SYNT,";"); }
		nbVarLoc++;
	}while(classe != ';');	
	
    affiche_element("symbole",yytext, trace_abs);
	classe = yylex();
  }

  if (nbVarLoc > 0){
    ajouterCode(PILE);
    ajouterCode(nbVarLoc);
  }

  /** attribution des adresses des arguments  */
  for( i = 0 ; i < dico.tab[rechExecutable].complement ; i++ )
    dico.tab[dico.base+i].adresse = i - dico.tab[rechExecutable].complement - 2;
    

  instructionBloc();
  dico.tab[rechExecutable].type = retour ? T_ENTIER : 0;

  /**** destruction du dico local  ***/
	sortieFonction();
	adrLocCrt = 0;
  /***********************************/

  if (dico.tab[rechExecutable].type == T_ENTIER){
    ajouterCode(DEPL);
    ajouterCode(- dico.tab[rechExecutable].complement - 3);
  }
  if (mem[tailMem-1] != RETOURNE){
    ajouterCode(SORTIE);
    ajouterCode(RETOURNE);
  }

  retour = FAUX;
  balise_fermante(fct, trace_abs);
}

/*** Fonction liste de declaration de fonction */
void listeDeclFonction(void){
	char *fct = "listeDeclFonction";
	balise_ouvrante(fct, trace_abs);
	do{
		declFonction();		
	}while(classe == ID_FCT);
	balise_fermante(fct, trace_abs);
}
/*****************************************************************************************************************/
/************************************************ INSTRUCTIONS  **************************************************/

/*** Fonction instruction appelant tout les instructions */
void instruction(void){
	char *fct = "instruction";
	balise_ouvrante(fct, trace_abs);
	
	switch(classe){
	  case ID_VAR:      	instructionAffect();   	break;
	  case '{':   			instructionBloc();     	break;
	  case SI:           	instructionSi();       	break;
	  case TANTQUE:      	instructionTantque();  	break;
	  case ID_FCT:        	instructionAppel();    	break;
	  case RETOUR:     		instructionRetour();	break;
	  case ECRIRE:       	instructionEcriture();  break;
	  case ';': 			instructionVide();     	break;
	  default:  erreur(ERR_SYNT, "Instruction");
  }
  balise_fermante(fct, trace_abs);
}

/*** Fonction instruction d'affectation  = */
void instructionAffect(void){
  char *fct = "instructionAffect";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != ID_VAR) {
	 printf("identificateur : instructionAffect \n");
     erreur(ERR_SYNT, "identificateur");
	}
	//affiche_dico();
  rechIdentif = rechercheDeclarative(yytext);
  if (rechIdentif < 0){
	printf("id %d et chain %s \n",rechIdentif, yytext);
    erreur(ERR_SEM, "identificateur non declare. : instructionAffect");
  }
  
  //afficherDico();
  affiche_element("id_variable",yytext, trace_abs);
  classe = yylex();
  
  if (classe == '['){  	// cas d'un expression  $d[]
    affiche_element("symbole",yytext, trace_abs);
    classe = yylex();
    expression();

    if (classe != ']')
       erreur(ERR_SYNT, "']'");
	   
	affiche_element("symbole",yytext, trace_abs);
 /*	if (dico.tab[rechIdentif].type != T_TABLEAU)
      erreur(ERR_SEM, "membre gauche d'affectation, type incompatible"); */
    
    classe = yylex();
  }
    else
    if (dico.tab[rechIdentif].type != T_ENTIER)
      erreur(ERR_SEM, "membre gauche d'affectation, type incompatible");

  
  if (classe != '=') {
	 printf("'=' instructionAffect \n");
     erreur(ERR_SYNT, "'='");
	}
	
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  expression();

  if (classe != ';'){ 
	 printf("';' instructionAffect %d \n",classe);
     erreur(ERR_SYNT, "';'"); 
	}
	
  affiche_element("symbole",yytext, trace_abs);	
  if (dico.tab[rechIdentif].type == T_ENTIER){
    if (dico.tab[rechIdentif].classe == C_VAR_GLOBALE)
      ajouterCode(DEPG);
    else
      ajouterCode(DEPL);
      ajouterCode(dico.tab[rechIdentif].adresse);
  }
  else{
    ajouterCode(DEPT);
    ajouterCode(dico.tab[rechIdentif].adresse);
  }

  balise_fermante(fct, trace_abs);
  classe = yylex();
}

/****  Fonction  instruction bloc {   */
void instructionBloc(void){
  char *fct = "instructionBloc";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != '{'){
	 printf("'{' instructionBloc \n");
     erreur(ERR_SYNT, "'{'");
  }
  
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  while (classe != '}'){ 
	//printf("'}' instructionBlock \n");
	instruction(); 
	}

  balise_fermante(fct, trace_abs);
  classe = yylex();
}

/***  Fonction instruction si   */
void instructionSi(void){
  int tampon1, tampon2;
  char *fct = "instructionSi";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != SI){
	 printf("si  instructionSi\n");
     erreur(ERR_SYNT, "'si'");
  }
	
  affiche_element("mot_clef",yytext, trace_abs);
  classe = yylex();
  expression();

   if (classe != ALORS){
	 printf("alors  instructionSi\n");
     erreur(ERR_SYNT, "'alors'");
	}
	
  affiche_element("mot_clef",yytext, trace_abs);
  classe = yylex();
  /**************** pile d'execution *********************/
	  ajouterCode(SIFAUX);
	  tampon1 = tailMem++;
	  instruction();
	  ajouterCode(SAUT);
	  tampon2 = tailMem++;
	  mem[tampon1] = tailMem;
  /************************************/

  if (classe == SINON){
    affiche_element("mot_clef",yytext, trace_abs);
    classe = yylex();
    instruction();
  }
  
  mem[tampon2] = tailMem;
  balise_fermante(fct, trace_abs);
}


/*** Fonction instruction tantque  */
void instructionTantque(void){

  int tampon1, tampon2;
  char *fct = "instructionTantque";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != TANTQUE){
	 printf("tanque  instructionTanque\n");
     erreur(ERR_SYNT, "'tantque'");
  }
  
  affiche_element("mot_clef",yytext, trace_abs);
  classe = yylex();
  /************ pile d'execution ****************/
	  tampon1 = tailMem;
	  expression();
	  ajouterCode(SIFAUX);
	  tampon2 = tailMem++;
  /********************************************/

  if (classe != FAIRE){
	 printf("faire  instructionTanque\n");
     erreur(ERR_SYNT, "'faire'");
	}
	
  affiche_element("mot_clef",yytext, trace_abs);
  classe = yylex();
  instruction();
  /************ pile d'execution *************/
  ajouterCode(SAUT);
  ajouterCode(tampon1);
  mem[tampon2] = tailMem; 
  /*****************************************/
  
 balise_fermante(fct, trace_abs);
}


/***   Fonction argumentEffectifs **/
void argumentEffectifs(int recheExecutable){
  
  int nbArgs = 0;
  char *fct = "argumentEffectifs";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != ')'){
    affiche_element("symbole",yytext, trace_abs);
    expression();
    nbArgs ++;
    while (classe == ','){
	  affiche_element("symbole",yytext, trace_abs);
      classe = yylex();
      expression();
	  nbArgs ++;
    }
  }
  if (nbArgs > dico.tab[recheExecutable].complement)
    erreur(ERR_SEM, "trop d'arguments");

  if (nbArgs < dico.tab[recheExecutable].complement)
    erreur(ERR_SEM, "moins d'arguments");
	
	balise_fermante(fct, trace_abs);
}


/***   Fonction Appelfct : appel de fonction */
void AppelFct(void) {

  char *fct = "AppelFct";
  balise_ouvrante(fct, trace_abs);
  rechExecutable = -1;
  if (classe != ID_FCT){
	 printf("id_fct  AppelFct\n");
     erreur(ERR_SYNT, "nom de la fonction");
	}
	 
  rechExecutable = rechercheExecutable(yytext);

  if (rechExecutable < 0)
    erreur(ERR_SEM, "fonction non declare");

  if (dico.tab[rechExecutable].classe != C_FONCTION)
    erreur(ERR_SEM, "identificateur de fonction aubligatoire !");
  
  affiche_element("id_fonction",yytext, trace_abs);
  classe = yylex();
  if (classe != '('){
	 printf("'('  AppelFct\n");
     erreur(ERR_SYNT, "'(Fct'");
	}
	
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  argumentEffectifs(rechExecutable);
  
  if (classe != ')'){
	 printf("')'  AppelFct\n");
     erreur(ERR_SYNT, "')'");
	}
	
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  balise_fermante(fct, trace_abs);
}

/*** Fonction instruction appel  */
void instructionAppel(void){
  char *fct = "instructionAppel";
  balise_ouvrante(fct, trace_abs);
  AppelFct();   // Appelfct

  if (classe != ';'){ 
	 printf("';' instructionAppel %d \n",classe);
     erreur(ERR_SYNT, "';'"); 
	}
	
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  /************** pile d'execution *****************/
	  ajouterCode(APPELFCT);
	  ajouterCode(dico.tab[rechExecutable].adresse);
	  if (dico.tab[rechExecutable].complement > 0){
		ajouterCode(PILE);
		ajouterCode(-dico.tab[rechExecutable].complement);
	  }
  /**********************************************/
  //affiche_element("hhhhhhhhhhhhhh",yytext, trace_abs);
  //classe = yylex();
  balise_fermante(fct, trace_abs);
}


/***    Fonction instruction Retour   */
void instructionRetour(void){
  char *fct = "instructionRetour";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != RETOUR){
	 printf("retour instructionRetour\n");
     erreur(ERR_SYNT, "'retour'");
	}

  affiche_element("mot_clef",yytext, trace_abs);
  classe = yylex();  
  expression();
 /******************* pile d'execution ******************/ 
	  if (principalFct == VRAI){
		ajouterCode(DEPG);
		ajouterCode(-1);
	  }
	  else{
		ajouterCode(DEPL);
		ajouterCode(-nbArgs-3);
	  }
  /******************************************************/
  
  if (classe != ';'){ 
	 printf("';' instructionRetour %d \n",classe);
     erreur(ERR_SYNT, "';'"); 
	}
  /***************** pile d'execution **********************/ 
	  ajouterCode(SORTIE);
	  ajouterCode(RETOURNE);
	  retour = VRAI;
  /*****************************************************/
   affiche_element("symbole",yytext, trace_abs);
   classe = yylex();
   balise_fermante(fct, trace_abs);
}

/***    Fonction instruction ecrire  */
void instructionEcriture(void){
	  char *fct = "instructionEcriture";
      balise_ouvrante(fct, trace_abs);
	  
	  if (classe != ECRIRE){
		 printf(" ecrire instructionEcriture\n");
		 erreur(ERR_SYNT, "'ecrire'");
		}
	  
	  affiche_element("mot_clef",yytext, trace_abs);
	  classe = yylex();
	  if (classe != '('){
		 printf("'(' instructionEcriture \n");
		 erreur(ERR_SYNT, "'('");
		}

	  affiche_element("symbole",yytext, trace_abs);
	  classe = yylex();
	  expression();
	  if (classe != ')'){
		 printf("')' instructionEcriture \n");
		 erreur(ERR_SYNT, "')'");
		}
      
	  affiche_element("symbole",yytext, trace_abs);
	  classe = yylex();
		
	  if (classe != ';'){ 
		 printf("';'instructionEcriture %d \n",classe);
		 erreur(ERR_SYNT, "';'"); 
		}		
		
	  ajouterCode(ECRIV);
	  affiche_element("symbole",yytext, trace_abs);
	  classe = yylex();
	  balise_fermante(fct, trace_abs);
}

/***  Fonction instruction vide */
void instructionVide(void){
  char *fct = "instructionVide";
  balise_ouvrante(fct, trace_abs);
  
  if (classe != ';'){ 
	 printf("';' instructionVide %d \n",classe);
     erreur(ERR_SYNT, "';'");   
	}
	
  affiche_element("symbole",yytext, trace_abs);
  classe = yylex();
  balise_fermante(fct, trace_abs);
}


/***************************************************************************************************************/
/**********************************************  OPPERATIONS **************************************************/

/***   Fonction   expression()      */
void expression(void)
{  	
	int memExp[100],indiceExp=0,Oper=0;
	char *fct = "expression";
    balise_ouvrante(fct, trace_abs);
	
	conjonction();
	if(classe == '|'){
	    ajouterCode(SIVRAI);
		memExp[indiceExp++]= tailMem++;
		Oper=1;
	}
	while(classe == '|'){
	    affiche_element("symbole",yytext, trace_abs);
		classe=yylex();
		conjonction();
		ajouterCode(SIVRAI);
		memExp[indiceExp++]= tailMem++;
	}
	if(Oper == 1){
    ajouterCode(EMPC);
    ajouterCode(0);
    ajouterCode(SAUT);
    mem[tailMem] = tailMem + 3;
    tailMem++;
    for(indiceExp--;indiceExp>=0;indiceExp--)
      mem[memExp[indiceExp]] = tailMem; 
    ajouterCode(EMPC);
    ajouterCode(1);
  }
  
 balise_fermante(fct, trace_abs);
}

/***  Fonction   conjonction()  */
void conjonction(void)
{  
	int memConj[100],indiceConj=0,oper=0;
	char *fct = "conjonction";
    balise_ouvrante(fct, trace_abs);
	
	negation(); 
	if(classe == '&'){
    ajouterCode(SIFAUX);
    memConj[indiceConj++]= tailMem++;
    oper = 1;
	}
	while(classe == '&'){
	    affiche_element("symbole",yytext, trace_abs);
		classe = yylex();
		negation();
		ajouterCode(SIFAUX);
		memConj[indiceConj++]= tailMem++;
	}
	if(oper == 1){
    ajouterCode(EMPC);
    ajouterCode(1);
    ajouterCode(SAUT);
    mem[tailMem] = tailMem + 3;
    tailMem++;
    for(indiceConj--;indiceConj>=0;indiceConj--)
      mem[memConj[indiceConj]] = tailMem; 
    ajouterCode(EMPC);
    ajouterCode(0);
  }
  balise_fermante(fct, trace_abs);
}


/*** Fonction negation()  */
void negation(void)
{
	int memNeg[100],indiceNeg=0,oper=0;
	char *fct = "negation";
    balise_ouvrante(fct, trace_abs);
	
	if(classe == '!'){
		affiche_element("symbole",yytext, trace_abs);
		classe = yylex();
		ajouterCode(SIFAUX);
		memNeg[indiceNeg++]= tailMem++;
		oper=1;
	}
	
	comparaison();
	while(classe == '!'){
		affiche_element("symbole",yytext, trace_abs);
		classe = yylex();
		comparaison();
		ajouterCode(SIFAUX);
		memNeg[indiceNeg++]= tailMem++;
	}
	
	
	if(oper == 1){
    ajouterCode(EMPN);
    ajouterCode(1);
    ajouterCode(SAUT);
    mem[tailMem] = tailMem + 3;
    tailMem++;
    for(indiceNeg--;indiceNeg>=0;indiceNeg--)
      mem[memNeg[indiceNeg]] = tailMem; 
    ajouterCode(EMPN);
    ajouterCode(0);
  }
  
  balise_fermante(fct, trace_abs);
}

/*** Fnction comparaison()   */
void comparaison(void){
  char *fct = "comparaison";
  balise_ouvrante(fct, trace_abs);
  
  expArith();
  switch(classe){
	case EGAL:
		affiche_element("symbole",yytext, trace_abs);
	    classe = yylex();
	    expArith();
		ajouterCode(EQU);
	break;
	case '=':
		affiche_element("symbole",yytext, trace_abs);
	    classe = yylex();
	    expArith();
		ajouterCode(EQU);
	break;
	case '<' :
		affiche_element("symbole",yytext, trace_abs);
	    classe = yylex();
	    expArith();
		ajouterCode(INFE);
	 break; 
  }
	balise_fermante(fct, trace_abs);
}


/***  Fonction expArith()   */
void expArith(void){
	int signe;
	char *fct = "expArith";
    balise_ouvrante(fct, trace_abs);
	  
	terme();
	while (classe == '+' || classe == '-'){
		signe = classe;
		affiche_element("symbole",yytext, trace_abs);
	    classe = yylex();
	    terme();
		if (signe == '+')
			ajouterCode(ADD);
		else
			ajouterCode(SOUS);
	}
	balise_fermante(fct, trace_abs);
}


/*** Fonction terme()  */
void terme(void){
	int signe;
	char *fct = "terme";
    balise_ouvrante(fct, trace_abs);
	
	facteur();
	while (classe == '*' || classe == '/'){
		signe = classe;
		affiche_element("symbole",yytext, trace_abs);
	    classe = yylex();
	    facteur();
		if (signe == '*')
			ajouterCode(MULT);
		else
			ajouterCode(DIVI);
	  }
	balise_fermante(fct, trace_abs);
}


/*** Fonction facteur()  */
void facteur(void){

	char *fct = "facteur";
    balise_ouvrante(fct, trace_abs);
	  switch (classe){
		  case '(':{
			affiche_element("symbole",yytext, trace_abs);
			classe = yylex();
			expression();
			if (classe != ')'){
			   erreur(ERR_SYNT, "')'");
			}
			affiche_element("symbole",yytext, trace_abs);
			classe = yylex();
			break;
		  }

		  case NOMBRE:{
			affiche_element("nombre",yytext, trace_abs);
			classe = yylex();
			ajouterCode(EMPC);
			ajouterCode(valeur);
			break;
		  }

		  case ID_FCT:{
			rechExecutable = rechercheExecutable(yytext);
            //affiche_dico();
			if (rechExecutable < 0){
			  printf("%s \n", yytext);
			  erreur(ERR_SEM, "fonction non declaré");
			 }
			 
			affiche_element("id_fonction",yytext, trace_abs);
			classe = yylex();
			
			if (classe != '(')
			     erreur(ERR_SYNT, "'('");
			
			affiche_element("symbole",yytext, trace_abs);			
			classe = yylex();
			if (dico.tab[rechExecutable].classe != C_FONCTION)
				erreur(ERR_SEM, "type incompatible");
				
			/****************** pile d'execution ********************/	
				ajouterCode(PILE);
				ajouterCode(1);
				argumentEffectifs(rechExecutable);
				ajouterCode(APPELFCT);
				ajouterCode(dico.tab[rechExecutable].adresse);		
				if (dico.tab[rechExecutable].complement > 0){
					 ajouterCode(PILE);
					 ajouterCode(-dico.tab[rechExecutable].complement);
				}
			/*******************************************************/	
			
			if (classe != ')')
				   erreur(ERR_SYNT, "')'");
				   
			affiche_element("symbole",yytext, trace_abs);	
			classe = yylex();
			break;
		  }
		  
		  case ID_VAR : {
			int identifiant;
			//printf("avant rech idvar %s\n", yytext);
			rechIdentif = rechercheDeclarative(yytext);
			//afficherDico();
			identifiant = rechIdentif;
			if (rechIdentif < 0)
			  erreur(ERR_SEM, "variable non declaré");
			  
			affiche_element("id_variable",yytext, trace_abs);
		    classe = yylex();

			if (classe == '['){
			  affiche_element("symbole",yytext, trace_abs);
			  classe = yylex();
			  expression();
			  
			  if (classe != ']')
				erreur(ERR_SYNT, "']'");
				
			  affiche_element("symbole",yytext, trace_abs);
			  classe = yylex();
			  if (dico.tab[identifiant].type != T_TABLEAU){
				printf("yytext %s le type est %d et la classe est %d\n", dico.tab[rechIdentif].identif, dico.tab[rechIdentif].type, classe);
				printf("identifiant %d et rechIdentif %d \n", identifiant, rechIdentif);
				erreur(ERR_SEM, "type incompatible : tableau");
			  }
				
			  ajouterCode(EMPT);
			  ajouterCode(dico.tab[rechIdentif].adresse);
			}else{
				//printf("apres ss%d \n",dico[id]->type);
				if (dico.tab[rechIdentif].type != T_ENTIER)
				  erreur(ERR_SEM, "type incompatible : entier");
				 // printf("apres ssvg%d \n",dico[id]->classe);
				if (dico.tab[rechIdentif].classe == C_VAR_LOCALE){
					//printf("avant  %d \n",id);
					ajouterCode(EMPG);
					//printf("apres %d \n",id);
				}
				else{
				  ajouterCode(EMPL);
				}
				
				//printf("apres sfffff %d \n",dico[id]->adresse);  
				//ajouterCode(dico[id]->adresse);
				ajouterCode(1);
			}

			break;
		   }
			
		  case LIRE:{
		    affiche_element("mot_clef",yytext, trace_abs);
			classe = yylex();
			if (classe != '('){
			   erreur(ERR_SYNT, "'('");
			}
			
			affiche_element("symbole",yytext, trace_abs);
			classe = yylex();
			if (classe!= ')'){
			   erreur(ERR_SYNT, "')'");
			}
			
			affiche_element("symbole",yytext, trace_abs);
			classe = yylex();
			ajouterCode(LIR);
			break;
		  }
		  default : { 
			printf ("%s - %d \n", yytext, classe);
		    erreur(ERR_SYNT, "expression");
			}
	  }
	 balise_fermante(fct, trace_abs);
}



