
#ifndef __ERREUR_H_
#define __ERREUR_H_


#define ERR_LEX      401
#define ERR_SYNT     402
#define ERR_SEM      403
#define OK           404
#define NON	     	 405
#define ERR_OUT_MEM  406
#define ERR_EXEC     407
#define ERR_NO       408

/** Traitement des erreurs */
void erreur(int type_err, char * s);
/** Affichage de la balise ouvrante " format xml" */
void balise_ouvrante(char *fct_, int trace_xml);
/** Affichage de la balise fermante " format xml" */
void balise_fermante(char *fct_, int trace_xml);
/** Affichage element " format xml" */
void affiche_element(char *fct_, char *texte_, int trace_xml);
/** Affichage texte " format xml" */
void affiche_texte(char *texte_, int trace_xml);

#endif

