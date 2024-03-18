/**
 * @file 
 * @brief fichier header du code source io.c
 * @author Hamza Krika
 * @version 1.0.0 
*/

#ifndef __IO_H
#define __IO_H

#include <stdio.h>
#include "grille.h"
#include "jeu.h"

// affichage d'un trait horizontal
void affiche_trait (int c);

// affichage d'une ligne de la grille
void affiche_ligne (int c, int* ligne, int vieillissement);

// affichage d'une grille
void affiche_grille (grille g, int vieillissement);

// effacement d'une grille
void efface_grille (grille g);

// debute le jeu
void debut_jeu(grille *g, grille *gc);

#endif
