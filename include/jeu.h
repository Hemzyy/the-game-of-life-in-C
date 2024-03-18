/**
 * @file 
 * @brief fichier header du code source jeu.c
 * @author Hamza Krika
 * @version 1.0.0 
*/

#ifndef __JEU_H
#define __JEU_H

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))


#include "grille.h"
#include <stdio.h>

// modulo modifié pour traiter correctement les bords i=0 et j=0
// dans le calcul des voisins avec bords cycliques
static inline int modulo(int i, int m) {return (i+m)%m;}

// compte le nombre de voisins vivants de la cellule (i,j)
// les bords sont cycliques.
int compte_voisins_vivants_c (int i, int j, grille g);

// compte le nombre de voisins vivants de la cellule (i,j)
// les bords sont non cycliques.
int compte_voisins_vivants_nc (int i, int j, grille g);

// fait évoluer la grille g d'un pas de temps
void evolue (grille *g, grille *gc, int (*compte_voisins_vivants)(int,int,grille), int vieillissement);

#endif
