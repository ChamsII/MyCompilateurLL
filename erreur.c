#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "erreur.h"

/*-------------------------------------------------------------------------*/

extern int nb_lignes;
int indent_xml = 0;
int indent_step = 1; // set to 0 for no indentation

/*-------------------------------------------------------------------------*/


/***  Fonction pour gérer les erreurs */
void erreur(int type_err, char *erreure){

	switch (type_err){
	  case ERR_LEX:
	    fprintf(stderr, "Ligne %d : charactere '%s' illegal.\n",nb_lignes, erreure);
	    exit(type_err);
	  case ERR_SYNT:
	    fprintf(stderr, "Ligne %d : erreur : '%s 'attendu.\n",nb_lignes, erreure);
	    exit(type_err);
	  case ERR_SEM:
	    fprintf(stderr, "Ligne %d : erreur :' %s '\n",nb_lignes, erreure);
	    exit(type_err);
	  case ERR_OUT_MEM:
	    fprintf(stderr, "'%s ': debordement de memoire.\n", erreure);
	    exit(type_err);
	  case ERR_EXEC:
	    fprintf(stderr, "erreur d'execution : '%s'.\narret de la machine.\n", erreure);
	    exit(type_err);
	  }
}


/*-------------------------------------------------------------------------*/

void indent() {
    int i;
    for( i = 0; i < indent_xml; i++ ) {
      printf( "  " );
    }    
}
/*-------------------------------------------------------------------------*/
void balise_ouvrante(char *fct_, int trace_xml)
{
  if( trace_xml ) {
    indent();
    indent_xml += indent_step ;
	  fprintf (stdout, "<%s>\n", fct_);
	}  
}

/*-------------------------------------------------------------------------*/

void balise_fermante(char *fct_, int trace_xml)
{
  if(trace_xml) {
    indent_xml -= indent_step ;
    indent();
    fprintf (stdout, "</%s>\n", fct_);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_texte(char *texte_, int trace_xml) 
{
  if(trace_xml) {
    indent();
    fprintf (stdout, "%s\n", texte_);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_xml_texte( char *texte_ ) {
  int i = 0;
  while( texte_[ i ] != '\0' ) {
    if( texte_[ i ] == '<' ) {
      fprintf( stdout, "&lt;" );
    }
    else if( texte_[ i ] == '>' ) {
      fprintf( stdout, "&gt;" );
    }
    else if( texte_[ i ] == '&' ) {
      fprintf( stdout, "&amp;" );
    }
    else {
      putchar( texte_[i] );
    }
    i++;
  }
}

/*-------------------------------------------------------------------------*/

void affiche_element(char *fct_, char *texte_, int trace_xml)
{
  if(trace_xml) {
    indent();
    fprintf (stdout, "<%s> ", fct_ );
    affiche_xml_texte( texte_ );
    fprintf (stdout, "</%s>\n", fct_ );
  }
}

