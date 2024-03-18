/**
 * @file jeu.c codes des fonctions necessaire pour le jeu
 * @author Hamza Krika
 * @version 1.0.0 
*/

#include "../include/jeu.h"

/**
 * fonction pour compter le nombre des voisin vivants 
 * \param i int
 * \param j int 
 * \param g grille
 * \return le nombre de voisin vivants
*/
int compte_voisins_vivants_c (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);

	return v; 
}

int compte_voisins_vivants_nc (int i, int j, grille g){
	int v=0;
	for(int n = max(0,i-1);n <= min(g.nbl-1,i+1);n++){
		for(int l=max(0,j-1); l<=min(g.nbc-1, j+1);l++){
			if((n!=i || l!=j) && est_vivante(n,l,g)){
				v++;
			}
		}
	}
	return v;
}

/**
 * fonction d'evolution des etapes du jeu
 * \param g *grille
 * \param gc *grille
 * \param vieillissement int
 * \param compte_voisins_vivants *int()
 * \return void
*/
void evolue (grille *g, grille *gc, int (*compte_voisins_vivants)(int,int,grille), int vieillissement ){
	copie_grille (*g,*gc); // copie temporaire de la grille
	int i,j, l=g->nbl, c = g->nbc,v;
	for (i=0; i<l; i++)
	{
		for (j=0; j<c; ++j)
		{
			v = compte_voisins_vivants (i, j, *gc);
			if(vieillissement == 0){ //si le vieillissement est desactivé
				if (est_vivante(i,j,*g)) { // evolution d'une cellule vivante
					if ( v!=2 && v!= 3 ){
						set_morte(i,j,*g);
					}else{
						g->cellules[i][j]++;
					}
				}else{ // evolution d'une cellule morte
					if ( est_nonViable(i,j,*g)){
						continue;
					}else if( v==3 ){
						set_vivante(i,j,*g);
					}
				}
			}else if(vieillissement == 1){ //si le vieillissement est activé
				if (est_vivante(i,j,*g)){
					if(v!=2 && v!= 3 || g->cellules[i][j] == 8){
						set_morte(i,j,*g);
					}else{
						g->cellules[i][j]++;
					}
				}else{
					if ( est_nonViable(i,j,*g)){
						continue;
					}else if( v==3 ){
						set_vivante(i,j,*g);
					}
				}
			}
		}
	}
	return;
}
