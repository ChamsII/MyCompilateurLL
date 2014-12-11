#ifndef __CODE3ADRMIPS__
#define __CODE3ADRMIPS__

#include "syntabs.h"

typedef enum { o_constante, o_etiq, o_temp, o_indirect } t_oper;

typedef struct oper3a_ {
  t_oper type;
  union {
    char *etiq;
    int temp;
    int constante;
    struct { struct oper3a_ *base; struct oper3a_ *decalage; } indirect ;
  } u;
} oper3a;

typedef enum {lw, la, li, sw, move, add, sub, addi, subi, mult, divi, mfhi, 
              mflo, and, or, not, neg, blt, bne, beq, bgt, ble, bge, j, jal, 
              jr, syscall, savereg, retreg } instr_mips;

typedef struct instr3a_ {
  instr_mips op; 
  oper3a *arg0;     // le resultat
  oper3a *arg1;     // premier operande
  oper3a *arg2;     // deuxieme operande
  /* Données auxiliaires */
  char *comment;    // commentaire à recopier sur la sortie MIPS
  int dernier_util; // Dernière ligne ou arg0 est utilisé
} instr3a;

typedef enum { l_etiq, l_instr } t_ligne;

typedef struct ligne3a_ {
  t_ligne type_ligne;
  union {
    char *etiq;
    instr3a *instr;
  } u;
} ligne3a;

// -----------------------------------------------------------------------------
// tableau contenant le code MIPS à 3 adresses
ligne3a *code;

// -----------------------------------------------------------------------------
// Fonctions pour créer des nouveaux opérandes de chaque type
oper3a *cree_temp();
oper3a *cree_etiq( char *etiq );
oper3a *cree_etiq_num(char *etiq, int n ); // utilise pour les sauts si/tantque
oper3a *cree_const( int constante );
// base peut être une étiquette ou constante, décalage peut être une étiquette 
// ou temp
oper3a *cree_indirect( oper3a *base, oper3a *decalage );

// -----------------------------------------------------------------------------
// Fonctions pour ajouter une nouvelle ligne ou étiquette au tableau code
void ajoute_instr_3a( instr_mips op, oper3a *arg0, oper3a *arg1, 
                      oper3a *arg2, char *comment );
void ajoute_etiq(oper3a *etiq);

// -----------------------------------------------------------------------------
// Afficher le code MIPS après avoir rempli le tableau de code 3 adresses
void code3adr_to_mips();

#endif
