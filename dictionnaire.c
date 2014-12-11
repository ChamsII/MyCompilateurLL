#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "dictionnaire.h"

/*-------------------------------------------------------------------------*/

void entreeFonction(void){
  dico.base = dico.sommet;
  contexte = C_VAR_LOCALE;
  nbArgs = 0;
  nbVarLoc = 0;
}

/*-------------------------------------------------------------------------*/

void sortieFonction(void){
  dico.sommet = dico.base;
  dico.base = 0;
  contexte = C_VAR_GLOBALE ;  
}

/*-------------------------------------------------------------------------*/

int ajouteIdentificateur(char *identif, int classe, int type, int adresse, int complement)
{
  dico.tab[dico.sommet].identif = strdup(identif);
  dico.tab[dico.sommet].classe = classe;
  dico.tab[dico.sommet].type = type;
  dico.tab[dico.sommet].adresse = adresse;
  dico.tab[dico.sommet].complement = complement;
  dico.sommet++;

  if(dico.sommet == MAX_TAILLE){
    fprintf(stderr, "attention, plus de place dans le dictionnaire des \
                     identificateurs, augmenter MAX_TAILLE\n");
    exit(1);
  }
 /* affiche_dico(); */
  return dico.sommet - 1;
}


/*-------------------------------------------------------------------------*/
int ajouter(char *element){

  if(dico.sommet == MAX_TAILLE){
    fprintf(stderr, "attention, plus de place dans le dictionnaire des \
                     identificateurs, augmenter MAX_TAILLE\n");
    exit(1);
  }
  
  dico.tab[dico.sommet].identif = strdup(element);
  dico.tab[dico.sommet].classe = 0;
  dico.tab[dico.sommet].type = 0;
  dico.tab[dico.sommet].adresse = 0;
  dico.tab[dico.sommet].complement = 0;
  dico.sommet++;

  if(dico.sommet == MAX_TAILLE){
    fprintf(stderr, "attention, plus de place dans le dictionnaire des \
                     identificateurs, augmenter MAX_TAILLE\n");
    exit(1);
  }
 //affiche_dico(); 
  return dico.sommet - 1;
}
/*-------------------------------------------------------------------------*/

int rechercheExecutable(char *identif)
{
  int i;
  for(i = dico.sommet - 1; i >= 0; i--){
    if(!strcmp(identif, dico.tab[i].identif))
      return i;
  }
  return -1;
}

/*-------------------------------------------------------------------------*/

int rechercheDeclarative(char *identif)
{
  int i;
  for(i = dico.base; i < dico.sommet; i++){
    if(!strcmp(identif, dico.tab[i].identif))
      return i;
  }
  
   for(i = 0; i < dico.sommet; i++){
    if(!strcmp(identif, dico.tab[i].identif)){
		if(dico.tab[i].classe == C_VAR_GLOBALE)
			return i;
	}
  }
  return -1;
}

/*-------------------------------------------------------------------------*/

void affiche_dico(void)
{
  int i;
  printf("------------------------------------------\n");
  printf("base = %d\n", dico.base);
  printf("sommet = %d\n", dico.sommet);
  for(i=0; i < dico.sommet; i++){
    printf("%d ", i);
    printf("%s ", dico.tab[i].identif);
    if(dico.tab[i].classe == C_VAR_GLOBALE)
      printf("GLOBALE ");
    else
    if(dico.tab[i].classe == C_VAR_LOCALE)
      printf("LOCALE ");
    else
    if(dico.tab[i].classe == C_ARG)
      printf("ARGUMENT ");

    if(dico.tab[i].type == T_ENTIER)
      printf("ENTIER ");
    else if(dico.tab[i].type == T_TABLEAU)
      printf("TABLEAU ");
/*     else if(dico.tab[i].type == _ARGUMENT) */
/*       printf("ARGUMENT "); */
    else if(dico.tab[i].type == C_FONCTION)
      printf("FONCTION ");
    
    printf("%d ", dico.tab[i].adresse);
    printf("%d\n", dico.tab[i].complement);
  }
  printf("------------------------------------------\n");
}
