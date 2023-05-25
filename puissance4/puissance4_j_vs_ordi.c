#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// les constantes
#define NB_LIGNES 6
#define NB_COLONNES 7
#define NON_TROUVE -1
#define PION_A 'X'
#define PION_B 'O'
#define VIDE ' '
#define INCONNU ' '

// les types
typedef int Grille[NB_LIGNES][NB_COLONNES];

// prototypes des fonctions
void initGrille(Grille laGrille);
void afficher(Grille laGrille, char pion);
bool grillePleine(Grille laGrille);
void jouerBot(Grille laGrille, char pion, int *ligne, int *colonne, int last_ligne, int last_colonne);
void jouer(Grille laGrille, char pion, int * ligne, int * colonne);
int choisirColonne(Grille laGrille, char pion);
int chercherLigne(Grille laGrille, int col);
bool estVainqueur(Grille laGrille, int lig, int col);
void finDePartie(char vainqueur);
int jouerPlusAuCentre(Grille laGrille);
void jouerDernierPionAdjacent(Grille laGrille, int derniere_lig, int derniere_col, int *ligne, int *colonne);
void trouveDeuxPionsAligne(Grille laGrille, int nb_pions, int *posX, int *posY);
void testAlignementPionLigne(Grille laGrille, int nb_pions, int ligne, bool *trouve, int *ligne_align, int *colonne_align);


// Programme principal. C'est le pion A qui commence Ã  jouer
int main(){
    Grille laGrille;
    char vainqueur=INCONNU;
    int ligne, colonne;
    int lig_bot, col_bot;
    lig_bot = -1;
    col_bot = -1;
    initGrille(laGrille);
    afficher(laGrille, PION_A);
    while (vainqueur==INCONNU && !grillePleine(laGrille)){
        jouer(laGrille,PION_A, &ligne, &colonne);
        afficher(laGrille, PION_B);
        if (estVainqueur(laGrille, ligne, colonne) ){
            vainqueur = PION_A;
        } else if (!grillePleine(laGrille)){
            jouerBot(laGrille, PION_B, &ligne, &colonne, lig_bot, col_bot);
            afficher(laGrille, PION_A);
            if (estVainqueur(laGrille, ligne, colonne) ){
                vainqueur = PION_B;
            }
        }
        if (laGrille[ligne][colonne] == PION_B){
            lig_bot = ligne;
            col_bot = colonne;
        } 
    }
    finDePartie(vainqueur);
    system(EXIT_SUCCESS);
}

void initGrille(Grille laGrille){
    int l, c;
    for (l=0 ; l<NB_LIGNES ; l++){
        for (c=0 ; c<NB_COLONNES ; c++){
            laGrille[l][c] = VIDE;
        }
    }
}

void afficher(Grille laGrille, char pion){
    int l, c;
    printf("\t");
    printf("  %c\n", pion);
    printf("\t");
    for (c=0; c<NB_COLONNES ; c++){
        printf("----");
    }
    printf("-\n");
    for (l=0 ; l<NB_LIGNES ; l++){
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
            printf("| %c ", laGrille[l][c]);
        }
        printf("|\n");
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
            printf("----");
        }
        printf("-\n");
    }
    printf("\t");
    for (c=0; c<NB_COLONNES ; c++){
            printf("  %d ",c);
    }
    printf("\n\n");

}

bool grillePleine(Grille laGrille){
    bool pleine = true;
    int c = 0;
    while (pleine && c<NB_COLONNES){
        if (laGrille[0][c] == VIDE){
            pleine = false;
        }
        c++;
    }
    return pleine;
}

void jouer(Grille laGrille, char pion, int * ligne, int * colonne ){
   *ligne = -1;
    do {
        *colonne = choisirColonne(laGrille, pion);
        *ligne = chercherLigne(laGrille, *colonne);
    }while (*ligne==-1);
    laGrille[*ligne][*colonne] = pion;
}

int choisirColonne(Grille laGrille, char pion){
    int col;
    do{
        printf("Numero de colonne ? ");
        scanf("%d", &col);
    } while (col<0 ||col>6);
    return col;
}

void jouerBot(Grille laGrille, char pion, int *ligne, int *colonne, int last_ligne, int last_colonne){
    int lig, col;
    lig = NON_TROUVE;
    col = NON_TROUVE;
    int i = 3;
    while ((lig == NON_TROUVE) && (i > 1)){
        trouveDeuxPionsAligne(laGrille, i, &(lig), &(col));
        i--;
    }
    if (lig != NON_TROUVE){
        *ligne = lig;
        *colonne = col;
    } else{
        jouerDernierPionAdjacent(laGrille, last_ligne, last_colonne, &lig, &col);
        if (lig != NON_TROUVE){
        *ligne = lig;
        *colonne = col;
        } else{
            *colonne = jouerPlusAuCentre(laGrille);
            *ligne = chercherLigne(laGrille, *colonne);
        }
        
    }
    laGrille[*ligne][*colonne] = pion;
}

void jouerDernierPionAdjacent(Grille laGrille, int derniere_lig, int derniere_col, int *ligne, int *colonne){

    //Placer le pion en bas à droite du dernier pion
    if ((derniere_lig < NB_LIGNES-2) && (derniere_col < NB_COLONNES-2) && (laGrille[derniere_lig+1][derniere_col+1] == VIDE) && ((derniere_lig+1 == (NB_LIGNES-1)) || (laGrille[derniere_lig+2][derniere_col+1] != VIDE))){
        *ligne = derniere_lig+1;
        *colonne = derniere_col+1;
        printf("\n1");
    } 
    //Placer le pion en haut à droite du dernier pion
    else if((derniere_lig > 0) && (derniere_col < NB_COLONNES-2) && (laGrille[derniere_lig-1][derniere_col+1] == VIDE) && (laGrille[derniere_lig][derniere_col+1] != VIDE)){
        *ligne = derniere_lig-1;
        *colonne = derniere_col+1;
        printf("\n2");
    } 
    //Placer le pion en haut à gauche du dernier pion
    else if((derniere_lig > 0) && (derniere_col > 0) && (laGrille[derniere_lig-1][derniere_col-1] == VIDE) && (laGrille[derniere_lig][derniere_col-1] != VIDE)){
        *ligne = derniere_lig-1;
        *colonne = derniere_col-1;
        printf("\n3");
    }
    //Placer le pion en bas à gauche du dernier pion
    else if((derniere_lig < NB_LIGNES-2) && (derniere_col > 0) && (laGrille[derniere_lig+1][derniere_col-1] == VIDE) && ((derniere_lig+1 == (NB_LIGNES-1)) || (laGrille[derniere_lig+2][derniere_col-1] != VIDE))){
        *ligne = derniere_lig+1;
        *colonne = derniere_col-1;
        printf("\n4");
    }
    //Placer le pion à droite du dernier pion
    else if((derniere_col < NB_COLONNES-2) && (laGrille[derniere_lig][derniere_col+1] == VIDE) && ((derniere_lig == (NB_LIGNES-1)) || (laGrille[derniere_lig+1][derniere_col+1] != VIDE))){
        *ligne = derniere_lig;
        *colonne = derniere_col+1;
        printf("\n5");
    } 
    //Placer le pion au dessus du dernier pion
    else if((derniere_lig > 0) && (laGrille[derniere_lig-1][derniere_col] == VIDE)){
        *ligne = derniere_lig-1;
        *colonne = derniere_col;
        printf("\n6");
    }
    //Placer le pion à gauche du dernier pion
    else if((derniere_col > 0) && (laGrille[derniere_lig][derniere_col-1] == VIDE) && ((derniere_lig == (NB_LIGNES-1)) || (laGrille[derniere_lig+1][derniere_col-1] != VIDE))){
        *ligne = derniere_lig;
        *colonne = derniere_col-1;
        printf("\n7");
    } else{
        *ligne = NON_TROUVE;
        *colonne = NON_TROUVE;
    }
    printf("\nLast lig: %d Last Col: %d Ligne: %d Col: %d",derniere_lig,derniere_col,*ligne,*colonne);
}

void trouveDeuxPionsAligne(Grille laGrille, int nb_pions, int *posX, int *posY){
    /*Cette procédure permet de vérifier l'alignement
    de nb_pions pour l'adversaire, ainsi si il y a un
    alignement elle donne en paramètre de sortie
    si elle trouve un alignement et la colonne
    et la ligne ou placer le pion*/

    bool trouve = false;
    *posX = NON_TROUVE;
    *posY = NON_TROUVE;
    int lig = (NB_LIGNES-1);

    while ((trouve == false) && (lig >= 0)){ //Pour chaques lignes
        testAlignementPionLigne(laGrille, nb_pions, lig, &trouve, &(*posX), &(*posY));
        if (trouve == false){
            lig--;
        }
    }
}

void testAlignementPionLigne(Grille laGrille, int nb_pions, int ligne, bool *trouve, int *ligne_align, int *colonne_align){
    /*Cette procédure permet de vérifier l'alignement
    de nb_pions pour l'adversaire sur une ligne en
    particulier, ainsi si il y a un
    alignement elle donne en paramètre de sortie
    si elle trouve un alignement et la colonne
    et la ligne ou placer le pion*/

    int colonne = 0;
    int i, j, cpt;

    while ((*trouve == false) && (colonne < NB_COLONNES)){ //Pour chaque colonnes de la ligne actuelle

        if (laGrille[ligne][colonne] == PION_A){ //Vérifie si on regarde le pion adverse
            char lePion = laGrille[ligne][colonne]; //Le pion actuel dont on vérifie l'alignement

            if (!(*trouve)){
                //Regarder la diagonale, en comptant le nombre de pions en haut à gauche du pion actuel
                i = ligne;
                j = colonne;
                cpt = 0;

                while (j>=0 && i>=0 && laGrille[i][j]==lePion){
                    cpt++;
                    i--;
                    j--;
                }
                if ((cpt>=nb_pions) && (colonne+1 < NB_COLONNES) && (ligne+1 < NB_LIGNES) && (laGrille[(ligne+1)][(colonne+1)] == VIDE)){
                    if (ligne+1 == (NB_LIGNES-1) || (laGrille[(ligne+2)][(colonne+1)] != VIDE)){ //Vérifie si on se situe à la première ligne ou si il y a un pion en dessous pour pouvoir placer le pion
                        *trouve = true;
                        //Donne les coordonées juste à en bas à droite du pion actuel
                        *colonne_align = colonne+1;
                        *ligne_align = ligne+1;
                    }
                }
            }
            if (!(*trouve)){
                //Regarder la diagonale, en comptant le nombre de pions en bas à droite du pion actuel
                i = ligne;
                j = colonne;
                cpt = 0;

                while (i<NB_LIGNES && j<NB_COLONNES && laGrille[i][j]==lePion){
                    cpt++;
                    i++;
                    j++;
                }
                if ((cpt>=nb_pions) && (colonne-1 >= 0) && (ligne-1 >= 0) && (laGrille[(ligne-1)][(colonne-1)] == VIDE)){
                    if (laGrille[ligne][(colonne-1)] != VIDE){ //Vérifie si il y a un pion en dessous pour pouvoir placer le pion
                        *trouve = true;
                        //Donne les coordonées juste à en haut à gauche du pion actuel
                        *colonne_align = colonne-1;
                        *ligne_align = ligne-1;
                    }
                }
            }
            if (!(*trouve)){
                //Regarder la diagonale, en comptant le nombre de pions en haut à droite du pion actuel
                i = ligne;
                j = colonne;
                cpt = 0;

                while (j<NB_COLONNES && i>=0 && laGrille[i][j]==lePion){
                    cpt++;
                    i--;
                    j++;
                }
                if ((cpt>=nb_pions) && (colonne-1 >= 0) && (ligne+1 < NB_LIGNES) && (laGrille[(ligne+1)][(colonne-1)] == VIDE)){
                    if (ligne+1 == (NB_LIGNES-1) || (laGrille[(ligne+2)][(colonne-1)] != VIDE)){ //Vérifie si on se situe à la première ligne ou si il y a un pion en dessous pour pouvoir placer le pion
                        *trouve = true;
                        //Donne les coordonées juste à en bas à gauche du pion actuel
                        *colonne_align = colonne-1;
                        *ligne_align = ligne+1;
                    }
                }
            }
            if (!(*trouve)){
                //Regarder la diagonale, en comptant le nombre de pions en bas à gauche du pion actuel
                i = ligne;
                j = colonne;
                cpt = 0;

                while (i<NB_LIGNES && j>=0 && laGrille[i][j]==lePion){
                    cpt++;
                    i++;
                    j--;
                }
                if ((cpt>=nb_pions) && (colonne+1 < NB_COLONNES) && (ligne-1 >= 0) && (laGrille[(ligne-1)][(colonne+1)] == VIDE)){
                    if (laGrille[(ligne)][(colonne+1)] != VIDE){ //Vérifie si il y a un pion en dessous pour pouvoir placer le pion
                    *trouve = true;
                    //Donne les coordonées juste à en haut à droite du pion actuel
                    *colonne_align = colonne+1;
                    *ligne_align = ligne-1;
                    }
                }
            }
            if (!(*trouve)){
                //Regarder l'horizontale, en comptant le nombre de pions à droite du pion actuel
                j = colonne;
                cpt = 0;

                while (j<NB_COLONNES && laGrille[ligne][j]==lePion){
                    cpt++;
                    j++;
                }
                if ((cpt>=nb_pions) && (colonne-1 >= 0) && (laGrille[ligne][(colonne-1)] == VIDE)){
                    if ((ligne == (NB_LIGNES-1)) || (laGrille[ligne+1][(colonne-1)] != VIDE)){ //Vérifie si on se situe à la première ligne ou si il y a un pion en dessous pour pouvoir placer le pion
                        *trouve = true;
                        //Donne les coordonées juste à gauche du pion actuel
                        *colonne_align = colonne-1;
                        *ligne_align = ligne;
                    }
                }
            }
            if (!(*trouve)){
                //Regarder l'horizontale, en comptant le nombre de pions à gauche du pion actuel
                j = colonne;
                cpt = 0;

                while (j>=0 && laGrille[ligne][j]==lePion){
                    cpt++;
                    j--;
                }
                if ((cpt>=nb_pions) && (colonne+1 < NB_COLONNES) && (laGrille[ligne][(colonne+1)] == VIDE)){
                    if ((ligne == (NB_LIGNES-1)) || (laGrille[ligne+1][(colonne+1)] != VIDE)){ //Vérifie si on se situe à la première ligne ou si il y a un pion en dessous pour pouvoir placer le pion
                        *trouve = true;
                        //Donne les coordonées juste à droite du pion actuel
                        *colonne_align = colonne+1;
                        *ligne_align = ligne;
                    }
                }
            }
            if (!(*trouve)){
                //Regarder la colonne, en comptant le nombre de pions en dessous du pion actuel
                i = ligne;
                cpt = 0;

                while (i<NB_LIGNES && laGrille[i][colonne]==lePion){
                    cpt++;
                    i++;
                }
                if ((cpt>=nb_pions) && (ligne-1 >= 0) && (laGrille[(ligne-1)][colonne] == VIDE)){
                    *trouve = true;
                    //Donne les coordonées juste en haut du pion actuel
                    *colonne_align = colonne;
                    *ligne_align = ligne-1;
                }
            }

        }
        if (*trouve == false){
            colonne++;
        }
    }
}

int jouerPlusAuCentre(Grille laGrille){
    int colonne;
    int i ; int j;
    colonne = NB_COLONNES /2;
    i =colonne-1;
    j = colonne+1;
    bool trouver = false;

    if (laGrille[0][colonne]==VIDE){
        colonne = colonne;
    } else{
        while(!trouver && i >-1 ){
            if (laGrille[0][j]==VIDE){
                colonne = j;
                trouver = true;
            }
            else if (laGrille[0][i]==VIDE){
                colonne =i;
                trouver = true;
            }
            i--;
            j++;
        }
    }
    return colonne;
}

int chercherLigne(Grille laGrille, int col){
    int ligne = -1;
    while (ligne<NB_LIGNES-1 && laGrille[ligne+1][col]==VIDE){
        ligne++;
    }
    return ligne;
}

bool estVainqueur(Grille laGrille, int lig, int col){
    // consiste Ã  regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
    char lePion = laGrille[lig][col];
    int cpt,i,j;
    int puissance = 4;

    // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
    i = lig;
    cpt = 0;
    while (i<NB_LIGNES && laGrille[i][col]==lePion){
        cpt++;
        i++;
    }
    if (cpt>=puissance){
        return true;
    }
    // regarder l'horizontale, en comptant le nombre de pions Ã  l'Est et Ã  l'Ouest
    j = col;
    cpt = 0;
    // on regarde Ã  l'est
    while (j>=0 && laGrille[lig][j]==lePion){
        cpt++;
        j--;
    }
    j = col+1;
    // on regarde Ã  l'ouest
    while (j<NB_COLONNES && laGrille[lig][j]==lePion){
        cpt++;
        j++;
    }
    if (cpt>=puissance){
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Ouest
    while (j>=0 && i>=0 && laGrille[i][j]==lePion){
        cpt++;
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    // on regarde au Sud Est
    while (i<NB_LIGNES && j<NB_COLONNES && laGrille[i][j]==lePion){
        cpt++;
        i++;
        j++;
    }
    if (cpt>=puissance){
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Est
    while (j<NB_COLONNES && i>=0 && laGrille[i][j]==lePion){
        cpt++;
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    // on regarde au Sud Ouest
    while (i<NB_LIGNES && j>=0 && laGrille[i][j]==lePion){
        cpt++;
        i++;
        j--;
    }
    if (cpt>=puissance){
        return true;
    }
    return false;
}

void finDePartie(char vainqueur){
    if (vainqueur != INCONNU){
        printf("Joueur %c vainqueur\n", vainqueur);
    } else {
        printf("MATCH NUL");
    }
}