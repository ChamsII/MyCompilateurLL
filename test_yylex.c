/**
 * @file test_yylex.c
 * @brief Fonctions du correcteur des mots franÃ§ais
 */

#include "lexique.h"
#include "grammaire.h"
#include "arbre_abstraite.h"

/*************************************** DECLARATION DES VARIABLES ***********************************************/
/** variable global */
int point = 0; 	/** position du point dans yyin */
int car_lu;		/** car à la position du point */
int depart = 0;			/** point de depart pour la mize à zéro de yytext */
char *entree;	/** Chaine de caractère à lire */
FILE *yyin = NULL;		/** Fichier code.l  */
char *retour;
int nb_lignes = 1;

/*********************************** CODE MACHINE ET EXECUTION DU PROGRAMME *************************************/
extern int mem[MAX_MEM+1];
//extern int adrGlobCrt;
//extern int tailMem;
char choix[200];
extern void codeMachine(int *mem, int debut, int fin); 
extern void lancerMachine(int * mem, int tailleCode, int tailleEspGlob);      /* dans machine.c                     */  
/*****************************************************************************************************************/
/******************************************* ANALYSEUR LEXICALE ************************************************/

/** lecture du fichier FILE  et création de la chaine d'entrée */
void lire_entree(void){
	int caractereActuel = 0;
	int carlu = 0;
	long tailleFichier;
	fseek(yyin,0,SEEK_END);
	tailleFichier = ftell(yyin);
	fseek(yyin,0,SEEK_SET);
	tailleFichier = tailleFichier + 2;
	if(entree == NULL)
		entree = (char *) malloc (sizeof(char) * tailleFichier);
	else
		memset (entree, 0, sizeof (tailleFichier));
	if(entree == NULL) printf("Erreur location memoire : malloc\n");
    	if (yyin != NULL)
    	{	
        	do
        	{
            	caractereActuel = fgetc(yyin); // On lit le caractère
				entree[carlu] = caractereActuel; // on ajoute le carectère dans la chaine entrée
				carlu = carlu + 1;
        	} while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
 
        fclose(yyin);
    	}
}

/** car_suivant() : le caractere suivant à lire */
void car_suivant(void){
	car_lu = entree[++point];
}

/**   est_fini_fichier() : fin du fichier entré en paramèttre  */
int fin_fichier(car){
	if(car == '.' || car == EOF) return 1;
	return 0;
}

/**  est_presentation() : supprimer les espace vide d'en haut du fichier  et les présentation machin */
int est_presentation(car) {
	if (fin_fichier(car) == 0 && car == ' ') return 1;
	return 0;
}

/**  est_espace() : supprimer les espace vide  */
int est_espace(car) {
	if (car == ' ') return 1;
	return 0;
}

/** est_debut_comm() : debut commentaire */
int est_debut_comm(car){
	 if(car == '#') return 1;
	return 0;
}

/** est_fini_comm() : fin de commentaire par # ou saut de ligne \n */
int est_fini_comm(car) {
	if(car == '#' || car =='\n') return 1;
	return 0;
}

/** est_saut_ligne() :  saut de ligne \n */
int est_saut_ligne(car) {
	if(car =='\n'){ 
		nb_lignes +=1;  
		return 1; 
	}
	return 0;
}

/** est_lettre()  qui peut être soit majuscule ou minuscule */
	/** est_maj() : si c'est majiscule */
int est_maj(car) {
	if('A' <= car && car <='Z') return 1;
	return 0;
}
	/** est_min() : si c'est minuscule */
int est_min(car) {
	if('a'<= car && car <= 'z') return 1;
	return 0;
}
	/** est_lettre() : combinaison entre majiscule et minuscule */
int est_lettre(car) {
	if(est_min (car) == 1) return 1;
	else if (est_maj(car) == 1) return 1;
	else return 0;
}

/** est_chiffre() : si c'est un chiffre pour la determination des entier */
int est_chiffre(car){
	if('0' <= car && car <= '9') return 1;
	return 0;
}

/** pour les identificateur avec melange de lettre et de chiffre */
int est_lettre_ou_chiffre(car){
	if(est_lettre(car)==1 || est_chiffre(car)==1) return 1;
	return 0;
}

/** pour les tiré de 8 _ dans les identificateurs */
int est_souligne(car){
	if(car =='_') return 1;
	return 0;
}

/** est_operateur pour les opérateurs aurorisés : +,-,*,/,<,=,&,|,! */
int est_operateur(car){
	char *chaine = "+-*/<=&|!";
   	unsigned int i;
        for (i = 0; i < 9; i++){
                if (chaine[i] == car) {  return 1; }
        }
	return 0;
}

/** est_separateur() : pour les sépaateurs autorisés : () {} , ; [ ] */
int est_separateur(car){
	char *chaine = "(){},;";
   	int i;
        for (i = 0; i < 6; i++){
                if (chaine[i] == car) return 1;
        }
		if (car == '[') return 1;
		if (car == ']') return 1;
		
	return 0;
}

/** est_dollar() : dollar pour marquer le debut d'une variable (identificateur) */
int est_dollar(car){
	if(car == '$') return 1;
	return 0;
}

/** est_egale : agale  pour former ==  */
int est_egale(car){
	if(car == '=') return 1;
	return 0;
}

/** debut_lex : debut de l'analyseur, se positionner au debut de l'entree */
void debut_lex(void){
	point = 0;
	car_lu = entree[point];
}

/** espace_comm : Eliminer les commentaire et les espaces */
void espace_comm(void){
	while(est_presentation(car_lu) == 1){ car_suivant(); }
	while(est_saut_ligne(car_lu) == 1){ car_suivant();}
	if (est_espace(car_lu) == 1) { while(est_espace(car_lu) == 1){ car_suivant();} }
	while(est_debut_comm(car_lu) == 1){
		car_suivant();
		while(est_fini_comm(car_lu) == 0){
			if(fin_fichier(car_lu) == 1) break;
			car_suivant();
		}
		if(fin_fichier(car_lu) == 0){
			car_suivant();
			while(est_presentation(car_lu) == 1){ car_suivant(); }
		}
	}
}


/** est_entier() : reconnaitre si c'est un entier  */
void est_entier(void){
	classe = NOMBRE;  car_suivant();
	while(est_chiffre(car_lu) == 1) { car_suivant(); }
}

/** est_identificateur() : reconnaitre si c'est un identificateur  */
void est_identificateur(void){
	classe = ID_VAR; car_suivant();
	while(est_lettre(car_lu) == 1) { car_suivant(); }
	while(est_souligne(car_lu) == 1 && est_lettre_ou_chiffre(entree[point+1]) == 1){
		car_suivant();
		while(est_lettre_ou_chiffre(car_lu) == 1) { car_suivant(); }
	}


	memset (yytext, 0, sizeof (15));
	mettre_un_zero(depart, point); 

}

/** mettre_un_zero(depart, point) : chaine terminant par 0 pour la représentation du lexeme */
void mettre_un_zero(depart, point){
	int i, j=0;
	memset (yytext, 0, sizeof (15));
	*yytext = '\0';
	for(i=depart; i< point; i++) { yytext[j] = entree[i]; j++; }
	yytext[j] = '\0';
}

/** est_mot_cle() : vérifier si c'est un mot clé : si, ecrire, lire, ....  */
void est_mot_cle(void){
	int taille;
	car_suivant();
	
	while(est_lettre(car_lu) == 1) { car_suivant(); }

	mettre_un_zero(depart, point);
	taille = point - depart;

	char buuf[taille];
	int j;
	for(j=0; j < taille; j++){
		buuf[j] = yytext[j];
	}
		buuf[j] = '\0';
		if(strcmp(buuf,"si") == 0){ classe = SI; }
		else if(strcmp(buuf,"alors") == 0){ classe = ALORS;  }
		else if(strcmp(buuf,"sinon") == 0){ classe = SINON;  }
		else if(strcmp(buuf,"tantque") == 0){ classe = TANTQUE; }
		else if(strcmp(buuf,"faire") == 0){ classe = FAIRE; }
		else if(strcmp(buuf,"entier") == 0){ classe = ENTIER;}
		else if(strcmp(buuf,"retour") == 0){ classe = RETOUR;}
		else if(strcmp(buuf,"lire") == 0){ classe = LIRE; }
		else if(strcmp(buuf,"ecrire") == 0){ classe = ECRIRE;}
		else if(strcmp(buuf,"main") == 0){ classe = ID_FCT;}
		else{ classe = ID_FCT; }
	
}


/** lex_suivant() : sous programme annalyseur lex **/ 
int lex_suivant (void){
	espace_comm();

	depart = point; 
	if(fin_fichier(car_lu) == 1){ classe = FDF; yytext = "FDF"; }
	else if(est_dollar(car_lu) == 1) { est_identificateur(); }

	else if(est_chiffre(car_lu) == 1) { est_entier(); mettre_un_zero(depart, point); valeur = atoi(yytext); }

	else if(est_operateur(car_lu)==1){ 
		classe = car_lu; car_suivant(); 
		if(est_egale(car_lu) ==1){ classe = EGAL; car_suivant(); }
		mettre_un_zero(depart, point); 
	}
	else if(est_separateur(car_lu)==1){ classe = car_lu; car_suivant(); mettre_un_zero(depart, point); }

	else if(est_lettre(car_lu) == 1) { est_mot_cle(); }

	else {classe = ERROR; car_suivant(); mettre_un_zero(depart, point);}

	
	return classe;
}


/** fonction yylex() : pilote de l'analyseur lexical */
int yylex(){
	int unite = 0;
	if(point == 0){
		lire_entree();
		debut_lex();	
	} 
		memset (yytext, 0, sizeof (15)); // vider la chaine yytext!
		fflush(stdout);
		unite = lex_suivant();
		return unite;
}

void reinitialise(void){
	int ute = 0;
	point = 0;
	debut_lex();
	ute = lex_suivant();
	printf("aaaaaaaa %s, %d\n", yytext, ute);
}

/****************************************************************************************************************/
/************************************************ FONCTION MAIN *************************************************/
/****************************************************************************************************************/
/***  fonction main  teste compilateur */
int main(int argc, char **argv) {
	//int uniteCourante;
	//char sortieSyn[30];
	//char sortieAbstr[30];
	//char sortieLex[30];
	//retour = (char *) malloc (sizeof(char) * 15);
	yytext = (char *) malloc (sizeof(char) * 15);
	/************ Ouverture du fichier code.l  */
	yyin = fopen(argv[1], "r");
	if(yyin == NULL){
		fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
		exit(1);
	}
	
	classe = yylex();
	programme();
	   /************ Teste pour l'analyseur lexical et redirection dans un fichier .lex  **/
   /**** sprintf(sortieLex, "%s.lex", argv[1]);
    uniteCourante = yylex();
   	if ((freopen(sortieLex, "w", stdout)) != NULL)
    {
		while (uniteCourante != FDF) {
			printf("%s, %d\n", yytext, uniteCourante);
			uniteCourante = yylex();
		}
		printf("%s, %d\n", ".", uniteCourante);
       fclose (stdout);
    }  **/
	
	/*********** Appel de l'analyseur syntaxique  et redirection dans un fichier .synt.xml  */
	/** sprintf(sortieSyn, "%s.synt.xml", argv[1]);
	classe = yylex();
	if ((freopen(sortieSyn, "w", stdout)) != NULL) 
    {
		programme();
        fclose (stdout);
    } **/
	
	/************* Appel analyseur abstraite  et redirection dans un fichier .synt.xml */
	/**** sprintf(sortieAbstr, "%s.asynt.xml", argv[1]);
	classe = yylex();
	if ((freopen(sortieAbstr, "w", stdout)) != NULL)
    {
		//reinitialise();
        affiche_prog();  
        fclose (stdout);
    } **/

    if (classe != FDF)
		erreur(ERR_SYNT, "Fermeture du fichier .l");
		
	
	/*** Création du code machine qui sera stocker dans un fichier codeMachine.log : Pile d'exécution *****/
	/*************************** Affichage de la pile d'exécution ****************************************/
	strcpy(choix, "");
	  while (choix[0] != 'n' && choix[0] != 'N'){
		fprintf(stderr,"\n voulez-vous crée le code machine : la pile d'exécution (o/n)...");
		fflush(stdout);
		scanf("%s", choix);
		if (choix[0] == 'o' || choix[0] == 'O'){
		  fprintf(stderr,"\n");
		  codeMachine(mem, 0, tailMem);
		  break;
		}
	  }
  /********************************* execution du code machine ****************************/
  /***************************** simulation d'une machine virtuelle **********************/
	strcpy(choix, "");
	  //while (choix[0] != 'n' && choix[0] != 'N'){
		printf("\n voulez-vous lancer la machine virtuelle (o/n)...");
		fflush(stdout);
		scanf("%s", choix);
		if (choix[0] == 'o' || choix[0] == 'O'){
		  printf("\n");
		  lancerMachine(mem, tailMem, adrGlobCrt);
		}
	  //}
		
   free(retour);
  return 0;
}



