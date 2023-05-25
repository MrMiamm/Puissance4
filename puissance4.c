
/**
* \page Puissance4
* 
* \author William CHERON
* \version 1.0
* \date 30 octobre 2021
*
* Ce programme permet de jouer à deux joueurs aux puissance4,
* en permettant aux joueurs de choisir la colonne où placer un jeton
* avec les touches q, d et espace.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/** 
* \def PION_A
* \brief Représente les pions du joueur1 
*/
#define PION_A 'X'

/** 
* \def PION_B
* \brief Représente les pions du joueur2
*/
#define PION_B 'O'

/** 
* \def VIDE
* \brief Représente les cases vides de la grille
*/
#define VIDE ' '

/** 
* \def INCONNU
* \brief Représente la valeur du vainqueur s'il n'y en a pas
*/
#define INCONNU ' '

/** 
* \def NBLIG
* \brief Représente le nombre de lignes dans la grille
*/
#define NBLIG 6

/** 
* \def NBCOL
* \brief Représente le nombre de colonnes dans la grille
*/
#define NBCOL 7

/** 
* \def PUISSANCE
* \brief Représente le nombre de pions alignés qu'il faut pour gagner
*/
#define PUISSANCE 4

/** 
* \def COLONNE_DEBUT
* \brief Correspond à la colonne au dessus de laquelle le pion à placer s'affichera
*/
#define COLONNE_DEBUT NBCOL/2

/**
* \typedef Grille
* \brief défini la grille de puissance4 allant jusqu'à NBLIG pour les lignes et NBCOL pour les colonnes
* 
* Cette grille sert à stocker les valeurs PION_A, PION_B et VIDE
* afin de savoir à quoi correspond un emplacement spécifique de la grille
*/
typedef char Grille[NBLIG][NBCOL];

void initGrille(Grille grille);
void afficher(Grille grille, char pion, int col);
bool grillePleine(Grille grille);
void jouer(Grille grille, char pion, int *li, int *col);
int choisirColonne(Grille grille, char pion, int col);
int trouverLigne(Grille grille, int col);
bool estVainqueur(Grille grille, int li, int col);
void finDePartie(char pion);

/***************************************************** 
*               PROGRAMME PRINCIPAL                  * 
*****************************************************/

/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme (0 : sortie normale).
* 
* Le programme principal permettant
* de jouer au puissance4 en faisant jouer deux joueurs
* chacun leur tour puis affiche qui gagne ou l'égalité.
* 
* Variable char vainqueur: Défini qui est le vainqueur le PION_A, le PION_B ou INCONNU
* Variable int ligne: Indique la ligne du pion joué
* Variable int colonne: Indique la colonne du pion joué
* Variable Grille g: La grille du jeu pouvant contenir des pions ou des cases vides
*/
int main(){
    char vainqueur;
    int ligne;
    int colonne;
    Grille g;
    
    initGrille(g); //Initialise la grille avec la valeur VIDE
    vainqueur = INCONNU;
    afficher(g, PION_A, COLONNE_DEBUT);

    while ((vainqueur == INCONNU) && (!(grillePleine(g)))){
        jouer(g, PION_A, &ligne, &colonne); //Récupère l'indice de la ligne et de la colonne du PION_A
        afficher(g, PION_B, COLONNE_DEBUT);

        if (estVainqueur(g, ligne, colonne)){ //Si le PION_A placé fait gagner
            vainqueur = PION_A;
        } else if (!(grillePleine(g))){ //Si la grille n'est pas pleine
            
            jouer(g, PION_B, &ligne, &colonne); //Récupère l'indice de la ligne et de la colonne du PION_B
            afficher(g, PION_A, COLONNE_DEBUT);

            if (estVainqueur(g, ligne, colonne)){ //Si le PION_B placé fait gagner
                vainqueur = PION_B;
            }
        }
    }
    finDePartie(vainqueur); //Affiche le vainqueur ou l'égalité
    return EXIT_SUCCESS;
}

/***************************************************** 
*               FONCTIONS ET PROCEDURES              * 
*****************************************************/

/**
* \fn void initGrille(Grille grille)
* \brief Initialise la grille avec du vide.
* \param grille : paramètre d'entrée/sortie qui représente la grille à initialiser.
*
* Chaque emplacement de la grille est initialisé à ' '
*/
void initGrille(Grille grille){
    for (int i = 0; i < NBLIG; i++){
        for (int j = 0; j < NBCOL; j++){
            grille[i][j] = VIDE;
        }  
    }  
}

/**
* \fn void afficher(Grille grille, char pion, int col)
* \brief Affiche la grille.
* \param grille : paramètre d'entrée qui représente la grille à afficher.
* \param pion : paramètre d'entrée qui représente le pion à placer.
* \param col : paramètre d'entrée qui représente la colonne où le pion apparait.
*
* Permet d'afficher le contenu de la grille ainsi que son armature
*/
void afficher(Grille grille, char pion, int col){
    /******************************************/
    /** 
    * Affichage de la première ligne indiquant
    * quelles touches appuyer et l'affichage de l'emplacement
    * du pion.
    */
    system("clear");
    printf("\n");
    printf("<- q ");
    for (int i = 0; i < (col); i++){
        printf("  ");
    }

    printf("<%c>",pion);

    for (int i = col+2; i <= NBCOL; i++){
        printf("  ");
    }
    printf(" d ->");
    printf("\n");

    /******************************************/
    /** 
    * Affichage du corps de la grille
    * incluant les différentes colonnes.
    */
    for (int i = 0; i < NBLIG; i++){
        printf("     │");
        for (int j = 0; j < NBCOL; j++){
            printf("%c",grille[i][j]);
            printf("│");
        }
        printf("\n");
    }

    /******************************************/
    /** 
    * Affichage de la dernière ligne
    * fermant la grille.
    */
    printf("     ├");
    for (int i = 0; i < NBLIG; i++){
        printf("─┴");
    }
    printf("─┤");
    printf("\n     ");
    printf("┴");
    for (int i = 0; i < NBLIG; i++){
        printf("  ");
    }
    printf(" ┴");
    printf("\n");
}

/**
* \fn int grillePleine(Grille grille)
* \brief Vérifie si la grille est pleine.
* \param grille : paramètre d'entrée qui représente la grille à vérifier.
* \return 1 si la grille est pleine, 0 sinon.
* 
* Vérifie si la dernière ligne, en partant du bas de la grille, est vide, si c'est le cas la grille entière est vide.
*/
bool grillePleine(Grille grille){
    bool grille_pleine = false;
    int nb_cases_remplies = 0;
    for (int i = 0; i < NBCOL; i++){
        if (grille[0][i] != VIDE){
            nb_cases_remplies+=1;
        }
    }
    if (nb_cases_remplies == NBCOL){
        grille_pleine = true;
    }
    return grille_pleine;
}

/**
* \fn int choisirColonne(Grille grille, char pion, int col)
* \brief Permet de choisir dans quelle colonne placer le pion
* \param grille : paramètre d'entrée qui représente la grille.
* \param pion : paramètre d'entrée qui représente le pion du joueur.
* \param col : paramètre d'entrée qui représente l'indice de départ de la colonne'.
* \return L'indice de la colonne choisie
* 
* Elle permet de choisir la colonne en utilisant les touches q et d et pour valider espace
*/
int choisirColonne(Grille grille, char pion, int col){
    int input = 0;
    while (input != ' '){ 
        afficher(grille, pion, col);
        input = getchar();
        if ((input == 'q') && (col > 0)){
            col-=1;
        } else if ((input == 'd') && (col < (NBCOL-1))){
            col+=1;
        }
    }
    return col;
}

/**
* \fn int trouverLigne(Grille grille, int col)
* \brief Permet de trouver le premier emplacement libre sur une colonne
* \param grille : paramètre d'entrée qui représente la grille.
* \param col : paramètre d'entrée qui représente la colonne où chercher l'emplacment vide'.
* \return L'indice de la ligne où l'on trouve le premier emplacment vide, ou si la colonne est pleine elle retourne -1.
* 
* Elle permet de trouver le premier emplacment vide grâce à une boucle qui si c'est vide
* en partant du bas de la colonne jusqu'en haut, si elle trouve un emplacement vide
* la boucle s'arrête.
*/
int trouverLigne(Grille grille, int col){
    int ligne;
    for (int i = (NBLIG-1); i >= -1; i--){
        if (i == -1){
            ligne = -1;
        } else if (grille[i][col] == VIDE){
            ligne = i;
            i = -1;
        }
        
    }
    return ligne;
}

/**
* \fn void jouer(Grille grille, char pion, int *li, int *col)
* \brief Permet de faire jouer le joueur en lui faisant faire placer un pion
* \param grille : paramètre d'entrée qui représente la grille
* \param pion : paramètre d'entrée qui représente le pion du joueur
* \param *li : paramètre de sortie qui réprésente la ligne où le joueur à placé son pion
* \param *col : paramètre de sortie qui réprésente la colonne où le joueur à placé son pion
* \return L'indice de la colonne et de la ligne où le pion a été placé.
* 
* Elle permet de faire jouer le joueur en appelant les fonctions choisirColonne
* pour placer un pion dans une colonne, et trouverLigne pour retrouver la ligne
* du pion.
*/
void jouer(Grille grille, char pion, int *li, int *col){
    int ligne = -1;
    int colonne;
    while (ligne == -1){
        colonne = choisirColonne(grille, pion, COLONNE_DEBUT);
        ligne = trouverLigne(grille, colonne);
    }
    grille[ligne][colonne] = pion;
    *li = ligne;
    *col = colonne;
}

/**
* \fn int estVainqueur(Grille grille, int li, int col)
* \brief Permet de trouver un vainqueur et vérifiant si 4 (PUISSANCE) pions sont alignés
* \param grille : paramètre d'entrée qui représente la grille
* \param li : paramètre d'entrée qui réprésente la ligne du pion qui vient d'être placé
* \param col : paramètre d'entrée qui réprésente la colonne du pion qui vient d'être placé
* \return booléen, qui montre s'il y a un vainqueur ou non
* 
* Elle permet de vérifier s'il y a un vainqueur à partir de l'indice de
* la colonne et de la ligne, pour cela elle vérifie pour le cas d'une ligne
* si à partir de ces coordonnées s'il y a les mêmes pions adjancents à gauche
* et à droite et que ce nombre de pions et supérieur ou égal à 4 (PUISSANCE),
* et si c'est le cas la fontion retourne vrai. C'est le même principe pour une
* colonne ou les diagonales.
*/
bool estVainqueur(Grille grille, int li, int col){
    bool est_vainqueur = false;
    int pions_alignes = 1;
    bool trouve_pion_adjacent;
    char pion = grille[li][col];
    int i,j;
    /******************************************/
    /**
    * Vérifie si sur la ligne du pion,
    * si 4 d'entre eux sont alignés.
    */
    trouve_pion_adjacent = true;
    i = col-1;
    while ((i >= 0) && (trouve_pion_adjacent)){ //Vérifie les pions en haut à gauche du pion choisi
        if ((grille[li][i] == grille[li][i+1]) && (grille[li][i] == pion)){ //Vérifie si le pion en bas à droite est le même
            pions_alignes+=1;
            i--;//
        } else{
            trouve_pion_adjacent = false;
        }
    }
    trouve_pion_adjacent = true;
    i = col+1;
    while ((i < NBCOL) && (trouve_pion_adjacent)){ //Vérifie les pions en haut à gauche du pion choisi
        if ((grille[li][i] == grille[li][i-1]) && (grille[li][i] == pion)){ //Vérifie si le pion en bas à droite est le même
            pions_alignes+=1;
            i++;
        } else{
            trouve_pion_adjacent = false;
        }
    }
    if (pions_alignes >= PUISSANCE){
        est_vainqueur = true;
    }

    /******************************************/
    /** 
    * Vérifie si sur la colonne du pion,
    * si 4 d'entre eux sont alignés.
    */
    if (est_vainqueur == false){
        pions_alignes = 1;

        trouve_pion_adjacent = true;
        i = li-1;
        while ((i >= 0) && (trouve_pion_adjacent)){ //Vérifie les pions en haut à gauche du pion choisi
            if ((grille[i][col] == grille[i+1][col]) && (grille[i][col] == pion)){ //Vérifie si le pion en bas à droite est le même
                pions_alignes+=1;
                i--;
            } else{
                trouve_pion_adjacent = false;
            }
        }
        trouve_pion_adjacent = true;
        i = li+1;
        while ((i < NBLIG) && (trouve_pion_adjacent)){ //Vérifie les pions en haut à gauche du pion choisi
            if ((grille[i][col] == grille[i-1][col]) && (grille[i][col] == pion)){ //Vérifie si le pion en bas à droite est le même
                pions_alignes+=1;
                i++;
            } else{
                trouve_pion_adjacent = false;
            }
        }
        
        if (pions_alignes >= PUISSANCE){
            est_vainqueur = true;
        }
    }
    
    /******************************************/
    /**
    *  Vérifie si sur la diagonale du pion partant
    * du coin supérieur gauche allant jusqu'au
    * coin inférieur droit,
    * si 4 d'entre eux sont alignés.
    */
    if (est_vainqueur == false){
        pions_alignes = 1;

        i = li-1;
        j = col-1;
        trouve_pion_adjacent = true;
        while ((i >= 0) && (j >= 0) && (trouve_pion_adjacent)){ //Vérifie les pions en haut à gauche du pion choisi
            if ((grille[i][j] == grille[i+1][j+1]) && (grille[i][j] == pion)){ //Vérifie si le pion en bas à droite est le même
                pions_alignes+=1;
                i--;
                j--;
            } else{
                trouve_pion_adjacent = false;
            }
        }

        i = li+1;
        j = col+1;
        trouve_pion_adjacent = true;
        while ((i < NBLIG) && (j < NBCOL) && (trouve_pion_adjacent)){ //Vérifie les pions en bas à droite du pion choisi
            if ((grille[i][j] == grille[i-1][j-1]) && (grille[i][j] == pion)){ //Vérifie si le pion en haut à gauche est le même
                pions_alignes+=1;
                i++;
                j++;
            } else{
                trouve_pion_adjacent = false;
            }    
        }
        
        if (pions_alignes >= PUISSANCE){
            est_vainqueur = true;
        }
    }

    /******************************************/
    /** 
    * Vérifie si sur la diagonale du pion partant
    * du coin inférieur gauche allant jusqu'au
    * coin supérieur droit,
    * si 4 d'entre eux sont alignés.
    */
    if (est_vainqueur == false){
        pions_alignes = 1;

        i = li-1;
        j = col+1;
        trouve_pion_adjacent = true;
        while ((i >= 0) && (j < NBCOL) && (trouve_pion_adjacent)){ //Vérifie les pions en haut à droite du pion choisi
            if ((grille[i][j] == grille[i+1][j-1]) && (grille[i][j] == pion)){ //Vérifie si le pion en bas à gauche est le même
                pions_alignes+=1;
                i--;
                j++;
            } else{
                trouve_pion_adjacent = false;
            }
        }

        i = li+1;
        j = col-1;
        trouve_pion_adjacent = true;
        while ((i < NBLIG) && (j >= 0) && (trouve_pion_adjacent)){ //Vérifie les pions en bas à gauche du pion choisi
            if ((grille[i][j] == grille[i-1][j+1]) && (grille[i][j] == pion)){ //Vérifie si le pion en haut à droite est le même
                pions_alignes+=1;
                i++;
                j--;
            } else {
                trouve_pion_adjacent = false;
            }
        }

        if (pions_alignes >= PUISSANCE){
            est_vainqueur = true;
        }
    }
    return est_vainqueur;
}

/**
* \fn void finDePartie(char pion)
* \brief Permet de choisir dans quelle colonne placer le pion
* \param pion : paramètre d'entrée qui représente le pion du joueur gagnant ou un pion INCONNU
* 
* Si le pion gagnant est INCONNU alors la procédure affiche "Egalité" sinon
* elle affiche le pion gagnant passé en entrée.
*/
void finDePartie(char pion){
    if ((pion == PION_A) || (pion == PION_B)){
        printf("\nLes %c ont gagné !\n", pion);
    } else{
        printf("\nEgalité !\n");
    }
}