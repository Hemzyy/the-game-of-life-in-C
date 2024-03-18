/**
 * @file io.h
 * * code d'affichage des grilles en mode texte
 * @author Hamza Krika
 * @version 1.0.0 
*/

#include "../include/io.h"

/**
 * fonction pour afficher les traits de la grille
 * \param c int
 * \return void
*/
void affiche_trait (int c){
	int i;
	for (i=0; i<c; ++i) printf ("|---");
	printf("|\n");
	return;
}

/**
 * fonction pour afficher les lignes de la grille
 * \param c int
 * \param ligne *int
 * \param vieillissement
 * \return void
*/
void affiche_ligne (int c, int* ligne, int vieillissement){
	int i;
	if (vieillissement)
	{
		for (i=0; i<c; ++i) 
			if (ligne[i] == 0 ) printf("|   ");
			else if (ligne[i] == -1 ) printf("| X ");
			else printf("| %d ",ligne[i]);
		printf("|\n");
	}
	else
	{
		for (i=0; i<c; ++i) 
			if (ligne[i] == 0 ) printf ("|   ");
			else if (ligne[i] == -1 ) printf("| X ");
			else printf ("| O ");
		printf("|\n");
	}
	return;
}


int temps_evol = 0;

/**
 * \relatesalso grille
 * fonction pour afficher la grille
 * \param g grille
 * \param vieillissement int
 * \return void
*/
void affiche_grille (grille g, int vieillissement){
	int i, l=g.nbl, c=g.nbc;
	printf("Temps d'evolution : %d", temps_evol);
	printf("\n");
	affiche_trait(c);
	for (i=0; i<l; ++i) {
		affiche_ligne(c, g.cellules[i], vieillissement);
		affiche_trait(c);
	}	
	printf("\n"); 
	return;
}

/**
 * \relatesalso grille
 * fonction pour effacer la grille
 * \param g grille
 * \return void
*/
void efface_grille (grille g){
	printf("\n\e[%dA",g.nbl*2 + 5); 
}

/**
 * \relatesalso grille
 * fonction du jeu
 * \param g *grille
 * \param gc *grille
 * \return void
*/
void debut_jeu(grille *g, grille *gc){
	int (*compte_voisins_vivants) (int, int, grille);
	compte_voisins_vivants = compte_voisins_vivants_c;
	char c = getchar();
	char nouvFichier[30];

	int vieillissement = 0;

	while (c != 'q') // touche 'q' pour quitter
	{ 
		switch (c) {
			case '\n' : 
			{ // touche "entree" pour évoluer
				temps_evol++;
				evolue(g,gc, compte_voisins_vivants, vieillissement);
				efface_grille(*g);
				affiche_grille(*g, vieillissement);
				break;
			}
			case 'n':
			{ // touche 'n' pour chager une autre grilles depuis le dossier grilles
				printf("Pour choisir un nouveau fichier grille: 'grilles/grille<num>.txt'");
				printf("\nDonner un nouveau fichier a charger:\n");
				scanf("%s", nouvFichier);
				libere_grille(g);
				libere_grille(gc);
				init_grille_from_file(nouvFichier, g);
				alloue_grille(g->nbl, g->nbc, gc);
				temps_evol = 0;
				affiche_grille(*g, vieillissement);
				break;
			}
			case 'c':
			{ // touche 'c' pour activer/desactiver le voisinage cyclique
				if (compte_voisins_vivants == compte_voisins_vivants_c){
					compte_voisins_vivants = compte_voisins_vivants_nc;
				}else{
					compte_voisins_vivants = compte_voisins_vivants_c;
				}
				break;
			}
			case 'v':
			{ // touche 'v' pour activer/desactiver le vieillissement
				vieillissement += 1;
				vieillissement %= 2;
				break;
			}
			default : 
			{ // touche non traitée
				printf("\n\e[1A");
				break;
			}
		}
		c = getchar(); 
	}
	return;	
}
