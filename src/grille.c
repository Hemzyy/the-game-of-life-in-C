/**
 * @file grille.c
 * * code contient les fonctions pour allouer, liberer et charger des grilles
 * @author Hamza Krika
 * @version 1.0.0 
*/

#include "../include/grille.h"


/**
 * alloue de l'espace memoire pour une grille de taille lxc
 * \relatesalso grille
 * \param l int
 * \param c int
 * \param g *grille
 * \return void
*/
void alloue_grille (int l, int c, grille* g){
	g->nbl = l;
	g->nbc = c;
	g->cellules = malloc(sizeof(int *) * l);
	for(int i=0;i<l;i++){
		g->cellules[i] = malloc(sizeof(int) * c);
	}
	for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {   // comme vue en TP la table 2D
            g->cellules[i][j] = 0;
        }
    }
}


/**
 * libere l'espace memoire d'une grille
 * \relatesalso grille
 * \param g *grille
 * \return void
*/
void libere_grille(grille* g){
	for(int i=0;i<g->nbl;i++){
		free(g->cellules[i]);
		}
	free(g->cellules);
	}


/**
 * fonction permet de charger une grille a partir d'un fichier
 * \relatesalso grille
 * \param filename *char
 * \param g *grille
 * \return void
*/
void init_grille_from_file (char * filename, grille* g){
	FILE * pfile = NULL;
	pfile = fopen(filename, "r");
	assert (pfile != NULL);
	
	int i,j,n,l,c,vivantes=0, nonViables=0;
	
	fscanf(pfile, "%d", & l);
	fscanf(pfile, "%d", & c);
	
	alloue_grille(l,c,g);
	
	fscanf(pfile, "%d", & vivantes);
	for (n=0; n< vivantes; ++n){
		fscanf(pfile, "%d", & i);
		fscanf(pfile, "%d", & j);
		set_vivante(i,j,*g);
	}
	
	fscanf(pfile, "%d", & nonViables);
	for (n=0; n< nonViables; ++n){
		fscanf(pfile, "%d", & i);
		fscanf(pfile, "%d", & j);
		set_nonViable(i,j,*g);
	}
	
	fclose (pfile);
	return;
}

/**
 * fonction permet de copier une grille
 * \relatesalso grille
 * \param gs grille
 * \param gd grille
 * \return void
*/
void copie_grille (grille gs, grille gd){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) gd.cellules[i][j] = gs.cellules[i][j];
	return;	
}
