#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code3adr.h"
#include "syntabs.h"
#include "util.h"

#define N_BLOC_CODE 100

int nb_blocs = 0;                      
int ligne = 0;

char *instr_nom[] = { "lw", "la", "li", "sw", "move", "add", "sub", "addi", 
                      "subi", "mult", "div", "mfhi", "mflo", "and", "or", "not", 
                      "neg", "blt", "bne", "beq", "bgt", "ble", "bge", "j", 
                      "jal", "jr", "syscall" };

/*-------------------------------------------------------------------------
 * Attention : Toujours créer un temporaire IMMÉDIATEMENT avant de l'utiliser
 * pour stocker le résultat d'une opération. Un opéraire créé à la ligne X
 * et utilisé à la ligne X+N, N>0 ne pourra pas être utilisé pour mettre à 
 * jour le champ dernier_util de l'instruction
 */
oper3a *cree_temp() {
  oper3a *r = malloc( sizeof( oper3a ) );
  r->type = o_temp;
  r->u.temp = ligne;
  return r;
}

/*-------------------------------------------------------------------------*/

oper3a *cree_etiq( char *etiq ) {
  oper3a *r = malloc( sizeof( oper3a ) );
  r->type = o_etiq;
  r->u.etiq = malloc( sizeof(char) * (strlen(etiq)+1) );
  strcpy( r->u.etiq, etiq );
  return r;
}

/*-------------------------------------------------------------------------*/

oper3a *cree_etiq_num( char *etiq, int num ) {
  oper3a *r;
  char *etiq2 = malloc( sizeof(char) * ( strlen( etiq ) + 11 ) );
  sprintf( etiq2, "%s%d", etiq, num );
  r = cree_etiq( etiq2 );
  free( etiq2 );
  return r;
}

/*-------------------------------------------------------------------------*/

oper3a *cree_const( int constante ) {
  oper3a *r = malloc( sizeof( oper3a ) );
  r->type = o_constante;
  r->u.constante = constante;
  return r;
}

/*-------------------------------------------------------------------------*/

oper3a *cree_indirect( oper3a *base, oper3a *decalage ) {
  oper3a *r = malloc( sizeof( oper3a ) );
  r->type = o_indirect;
  r->u.indirect.base = base;
  r->u.indirect.decalage = decalage;  
  return r;
}

/*-------------------------------------------------------------------------*/

void ajoute_ligne() {
  if( ligne >= nb_blocs * N_BLOC_CODE ) {
    nb_blocs++;
    code = realloc( code, sizeof(ligne3a) * nb_blocs * N_BLOC_CODE );
  }
}

/*-------------------------------------------------------------------------*/

void ajoute_oper( oper3a *arg, oper3a **dest ) {
  *dest = arg;
  // mise a jour de derniere ligne d'utilisation d'un temporaire
  // si l'argument n'est pas nul, est un temp et ne vient pas d'être créé
  if( arg && arg->type == o_temp && arg->u.temp != ligne ) {
    // la ligne de code ou le temp a été créé a comme dernière utilisation la
    // ligne courante
    code[ arg->u.temp ].u.instr->dernier_util = ligne;   
  }
}

/*-------------------------------------------------------------------------*/

void ajoute_instr_3a(instr_mips op, oper3a *arg0, oper3a *arg1, oper3a *arg2, char *comment ) {
  ajoute_ligne();
  code[ ligne ].type_ligne = l_instr;
  code[ ligne ].u.instr = malloc( sizeof( instr3a ) );
  code[ ligne ].u.instr->op = op;
  ajoute_oper( arg0, &code[ ligne ].u.instr->arg0 );
  ajoute_oper( arg1, &code[ ligne ].u.instr->arg1 );
  ajoute_oper( arg2, &code[ ligne ].u.instr->arg2 );
  if( comment ) {
    code[ ligne ].u.instr->comment = malloc( (strlen(comment)+1) * sizeof(char) );
    strcpy( code[ ligne ].u.instr->comment, comment );
  }
  else {
    code[ ligne ].u.instr->comment = NULL;
  }
  code[ ligne ].u.instr->dernier_util = ligne;
  ligne++;
}

/*-------------------------------------------------------------------------*/

void ajoute_etiq(oper3a *etiq) {
  ajoute_ligne();
  code[ligne].type_ligne = l_etiq;
  code[ligne].u.etiq = malloc( sizeof(char) * (strlen( etiq->u.etiq )+1) );
  strcpy( code[ligne].u.etiq, etiq->u.etiq );
  ligne++;
}

/*-------------------------------------------------------------------------*/

int getReg( int temp, int l_courante ) {
  // TODO: IMPLÉMENTER ALLOCATION DE REGISTRES
  return temp;
}

/*-------------------------------------------------------------------------*/

void affiche_operande( char *avant, oper3a *oper, int l_courante ) {
  int reg;
  if( oper ) {
    switch( oper->type ) {
      case o_etiq :
        printf( "%s%s",avant, oper->u.etiq );            
        break;
      case o_constante :
        printf( "%s%d",avant, oper->u.constante );      
        break;
      case o_indirect :
        printf( "%s", avant );
        affiche_operande( "", oper->u.indirect.base, l_courante );
        affiche_operande( "(", oper->u.indirect.decalage, l_courante   );
        printf( ")" );
        break;
      case o_temp :        
        reg = getReg( oper->u.temp, l_courante );
        if( reg == -1 ) {
          erreur( "Plus de registre disponible" );
        }
        printf( "%s$t%d", avant, reg);
        break;
    }
  }
}

/*-------------------------------------------------------------------------*/

void print_instr( instr3a *instr, int l ) {
  printf( "\t%s\t", instr_nom[ instr->op ] );
  affiche_operande( "", instr->arg0, l );      
  affiche_operande( ", ", instr->arg1, l );
  affiche_operande( ", ", instr->arg2, l );
  printf( "\t\t\t# (%d) ", instr->dernier_util );
  if( instr->comment ) {
    printf( "%s", instr->comment );
  }
  printf( "\n" );
}

/*-------------------------------------------------------------------------*/

void code3adr_to_mips( n_prog *p ) {
  int l;
  for( l = 0; l < ligne; l++ ) {
    if ( code[l].type_ligne == l_etiq ) {
      printf( "%s:\n", code[l].u.etiq );
    }
    else {
      print_instr( code[l].u.instr, l );      
    }
  }
}
