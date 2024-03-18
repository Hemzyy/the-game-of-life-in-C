/**
 * @file visual_main.c
 * @brief fichier du code principale qui execute le jeu en mode visual
 * @author Hamza Krika
 * @version 1.0.0 
*/

#include <stdio.h>
#include "../include/grille.h"
#include "../include/io.h"
#include "../include/jeu.h"

#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>

#define SIZEX 1000
#define SIZEY 700
#define BUFSIZE 5


int temps_devol = 0;
int vieillissement = 0;
int cyclique=1;

void init_buf(char *buf, size_t size){
	for(int i=0; i<size; i++){
		buf[i] = i + '0';
	}
}

/**
 * fonction pour dessiner les grilles
 * \param surface *cairo_surface_t
 * \param g *grille
 * \return void
*/
void paint(cairo_surface_t *surface, grille* g)
{
	int cX, cY = 0;

	// create cairo mask
	cairo_t *cr;
	cr=cairo_create(surface);

	// background
	cairo_set_source_rgb (cr, 255.0, 255.0, 255.0);
	cairo_paint(cr);
	
	// lines
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	for(int i=0; i<g->nbc+1;i++){
		cairo_move_to (cr, 0.0, i*50);
		cairo_line_to(cr, 50*g->nbl, i*50);
		cairo_set_line_width (cr, 2);
		cairo_stroke (cr);
	}

	// colonne
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	for(int j=0; j<g->nbl+1;j++){
		cairo_move_to (cr, j*50, 0.0);
		cairo_line_to(cr, j*50, 50*g->nbc);
		cairo_set_line_width (cr, 2);
		cairo_stroke (cr);
	}
	
	// filled rectangle
	int i, j;
	for (i=0; i<g->nbl; i++){
		for (j=0; j<g->nbc; ++j){
			if(est_vivante(i,j,*g)){
				cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
				cairo_rectangle(cr,i*50,j*50,50,50);
				cairo_fill(cr);	

				if(vieillissement){
					char buf[BUFSIZ];
					init_buf(buf, BUFSIZ);
					cairo_set_source_rgb(cr, 255, 255, 255);
					cairo_set_font_size(cr, 17);
					cairo_move_to(cr, i*50+20, j*50+30);
					init_buf(buf, BUFSIZE);
					snprintf(buf, 5, "%d", g->cellules[i][j]);
					cairo_show_text(cr, buf);
				}

			}else if(est_nonViable(i,j,*g)){
				cairo_set_source_rgb (cr, 255, 0, 0);
				cairo_rectangle(cr,i*50,j*50,50,50);
				cairo_fill(cr);
			}
		}
	}

	//print game info
	char buf[BUFSIZ];
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_font_size(cr, 25);
	//temps d'evolution
	cairo_move_to(cr, g->nbc*50 + 100, 100);
	cairo_show_text(cr, "Temps d'evolution: ");
	cairo_move_to(cr, g->nbc*50 + 350, 100);
	init_buf(buf, BUFSIZE);
	snprintf(buf, 5, "%d", temps_devol);
	cairo_show_text(cr, buf);

	//cyclique
	cairo_move_to(cr, g->nbc*50 + 100, 150);
	cairo_show_text(cr, "Voisinage: ");
	cairo_move_to(cr, g->nbc*50 + 350, 150);
	if(cyclique){
		cairo_show_text(cr, "cyclique");
	}else{
		cairo_show_text(cr, "non cyclique");
	}

	//vieillissement
	cairo_move_to(cr, g->nbc*50 + 100, 200);
	cairo_show_text(cr, "vieillissement: ");
	cairo_move_to(cr, g->nbc*50 + 350, 200);
	if(!vieillissement){
		cairo_show_text(cr, "Activé");
	}else if(vieillissement){
		cairo_show_text(cr, "Désactivé");
	}

	//charger une nouvelle grille
	cairo_move_to(cr, 50, g->nbl*50 + 150);
	cairo_show_text(cr, "Pour charger une nouvlle grille, appuyer sur n et entrez son chemain");
	cairo_move_to(cr, 50, g->nbl*50 + 175);
	cairo_show_text(cr, "dans le terminal");
	cairo_move_to(cr, 50, g->nbl*50 + 200);
	cairo_show_text(cr, "Exemple: \"grilles/grille<num>.txt\"");
	
	cairo_destroy(cr); // destroy cairo mask
}

void visualGame(grille *g, grille* gc){
	char nouvFichier[30];
	// X11 display
	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;

	int (*compte_voisins_vivants) (int, int, grille);
	compte_voisins_vivants = compte_voisins_vivants_c;

	// init the display
	if(!(dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr=DefaultScreen(dpy);
	rootwin=RootWindow(dpy, scr);

	win=XCreateSimpleWindow(dpy, rootwin, 1, 1, SIZEX, SIZEY, 0, 
			BlackPixel(dpy, scr), BlackPixel(dpy, scr));

	XStoreName(dpy, win, "jeu de la vie");
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask|KeyPressMask);
	XMapWindow(dpy, win);
	
	// create cairo surface
	cairo_surface_t *cs; 
	cs=cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), SIZEX, SIZEY);

	// run the event loop
	while(1) {
		XNextEvent(dpy, &e);
		paint(cs, g);

		if(e.type==KeyPress){

			if(e.xkey.keycode == 54){ //touche c pour activer/ désactiver le voisinage cyclique
				if (compte_voisins_vivants == compte_voisins_vivants_c){
					compte_voisins_vivants = compte_voisins_vivants_nc;
					cyclique++;
					cyclique %= 2;
					
				}else{
					cyclique++;
					cyclique %= 2;
					compte_voisins_vivants = compte_voisins_vivants_c;
				}
				paint(cs, g);
			}
			if(e.xkey.keycode == 55){ //touche v pour actver/ désactiver le vieillissement
				vieillissement += 1;
				vieillissement %= 2;
				paint(cs, g);
			}
			if(e.xkey.keycode == 57){ //touche n pour charger une nouvelle grille
				printf("Pour choisir un nouveau fichier grille: 'grilles/grille<num>.txt'");
				printf("\nDonner un nouveau fichier a charger:\n");
				scanf("%s", nouvFichier);
				libere_grille(g);
				libere_grille(gc);
				init_grille_from_file(nouvFichier, g);
				alloue_grille(g->nbl, g->nbc, gc);
				temps_devol = 0;
				paint(cs, g);
			}
			
		}
		if(e.type==ButtonPress){
			if(e.xbutton.button == 1){
				evolue(g,gc, compte_voisins_vivants, vieillissement);
				temps_devol++;
				paint(cs, g);
			}else if(e.xbutton.button == 3){
				break;
			}
		}
	}

	cairo_surface_destroy(cs); // destroy cairo surface
	XCloseDisplay(dpy); // close the display

}


int main (int argc, char ** argv) {
	
	if (argc != 2 )
	{
		printf("usage : .bin/main <fichier grille>");
		return 1;
	}

	grille g, gc;
	init_grille_from_file(argv[1],&g);
	alloue_grille (g.nbl, g.nbc, &gc);

	visualGame(&g, &gc);

	libere_grille(&g);
	libere_grille(&gc);

	return 0;
}
