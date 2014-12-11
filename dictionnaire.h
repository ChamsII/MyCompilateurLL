#ifndef __DICTIONNAIRE_H_
#define __DICTIONNAIRE_H_

#ifndef MAX_TAILLE_IDENTIF
#define MAX_TAILLE	 100
#define MAX_MEM      300
#endif

#define C_VAR_LOCALE 	601
#define C_VAR_GLOBALE 	602
#define C_ARG 			603
#define C_FONCTION 		604
#define T_ENTIER 		605
#define T_TABLEAU 		606

/** Structure du dictionnaire  de l'analyseur syntaxique */
typedef struct identificateur{
  char *identif;
  int classe;
  int type;
  int adresse;
  int complement; /* pour une fonction le nombre d'argumments et pour un tableau le nombre d'elements */
} identificateur;

typedef struct {
  identificateur tab[MAX_TAILLE];
  int base;
  int sommet;
} dico_;

void entreeFonction(void);
void sortieFonction(void);
int ajouteIdentificateur(char *identif,  int classe, int type, int adresse, int complement);
int ajouter(char *element);
int rechercheExecutable(char *identif);
int rechercheDeclarative(char *identif);
void affiche_dico(void);

dico_ dico;
extern int contexte;
extern int nbArgs;
extern int nbVarLoc;

int mem[MAX_MEM+1];
int adrGlobCrt;
int tailMem;

#endif
