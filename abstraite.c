#include <stdio.h>
#include "unitesSyntaxiques.h"

static struct {
    int codeOp;
    int operande;
    char *code;
} codes[] = {
    { EMPC,    		1,  "EMPC    	" },
    { EMPL,    		1,  "EMPL    	" },
    { DEPL,    		1,  "DEPL    	" },
    { EMPG,    		1,  "EMPG    	" },
    { DEPG,    		1,  "DEPG    	" },
    { EMPT,    		1,  "EMPT    	" },
    { DEPT,    		1,  "DEPT    	" },
    { ADD,     		0,  "ADD     	" },
    { SOUS,   	 	0,  "SOUS    	" },
    { MULT,   		0,  "MULT    	" },
    { DIVI,   		0,  "DIVI    	" },
    { EQU,     		0,  "EQU     	" },
    { INFE,    		0,  "INFE    	" },
    { NEG,     		0,  "NEG     	" },
    { LIR,     		0,  "LIR     	" },
    { ECRIV,   		0,  "ECRIV   	" },
    { SAUT,    		1,  "SAUT    	" },
    { SIVRAI,  		1,  "SIVRAI  	" },
    { SIFAUX,  		1,  "SIFAUX  	" },
    { APPELFCT,  	1,  "APPELFCT  	" },
    { RETOURNE, 	0,  "RETOURNE  	" },
    { ENTREE, 	 	0,  "ENTREE  	" },
    { SORTIE,  		0,  "SORTIE  	" },
    { PILE,    		1,  "PILE    	" },
    { STOP,    		0,  "STOP    	" },
    { 0,       		0,  "INCONNU 	" } };

#define NCODES (sizeof codes / sizeof codes[0] - 1) 

/* codeMachine(mem, debut, fin) affiche le code qui occupe 
   les mémoires mem[debut], mem[debut + 1] ... mem[fin - 1]
   Le texte affiché est également mis dans un fichier nommé 
   codeMachine.log */

void codeMachine(int *mem, int debut, int fin) {
    int i, j, k;
    char tmp[40];
    FILE *fichier = fopen("codeMachine.log", "w");
    
    i = debut;
    while (i < fin) {
        k = mem[i];
        for (j = 0; j < NCODES; j++)
            if (codes[j].codeOp == k)
                break;
                
        if (codes[j].operande) {
            sprintf(tmp, "%4d  %s %d\n", i, codes[j].code, mem[i+1]);
            i += 2;
        }
        else { 
            sprintf(tmp, "%4d  %s\n", i, codes[j].code);
            i += 1;
        }
        //fprintf(stderr,"%s \n",tmp);
        if (fichier != NULL) { 
            fprintf(fichier, "%s", tmp);
			
        }
    }
    if (fichier != NULL)
        fclose(fichier);   
}

