#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "dicos.h"

struct AFD {
    int taille_Q;
    int taille_Sigma;
    int q0;
    bool* finaux;
    int** delta;
};

typedef struct AFD afd;

void liberer_afd(afd* A);

afd* initialiser_afd(int taille_Q, int taille_Sigma, int q0);

void ajout_transition_afd(afd* A, int q, char a, int p);

int delta_etoile_afd(afd* A, int q, char* u);

bool reconnu_afd(afd* A, char* u);

struct AFND {
    int taille_Q;
    int taille_Sigma;
    bool* initiaux;
    bool* finaux;
    liste** delta;
};

typedef struct AFND afnd;

void liberer_afnd(afnd* B){
    //Libère la mémoire d'un AFND.
    free(B->initiaux);
    free(B->finaux);
    for (int i=0; i<B->taille_Q; i++){
        for (int j=0; j<B->taille_Sigma; j++){
            liberer_liste(B->delta[i][j]);
        }
        free(B->delta[i]);
    }
    free(B->delta);
    free(B);
}

afnd* initialiser_afnd(int taille_Q, int taille_Sigma){
    //Initialise un AFND.
    afnd* B = malloc(sizeof(afnd));
    B->taille_Q = taille_Q;
    B->taille_Sigma = taille_Sigma;
    bool* finaux = malloc(taille_Q * sizeof(bool));
    bool* initiaux = malloc(taille_Q * sizeof(bool));
    liste** delta = malloc(taille_Q * sizeof(liste*));
    for (int q=0; q<taille_Q; q++){
        finaux[q] = false;
        initiaux[q] = false;
        liste* tab = malloc(taille_Sigma * sizeof(liste));
        for (int a=0; a<taille_Sigma; a++){
            tab[a] = NULL;
        }
        delta[q] = tab;
    }
    B->finaux = finaux;
    B->initiaux = initiaux;
    B->delta = delta;
    return B;
}

void ajout_transition_afnd(afnd* B, int q, char a, int p);

bool* delta_etats(afnd* B, bool* etats_depart, char a);

bool* delta_etoile_afnd(afnd* B, bool* etats_depart, char* u);

bool reconnu_afnd(afnd* B, char* u);

int etats_vers_entier(bool* etats, int taille_Q);

bool* entier_vers_etats(int numero_etat, int taille_Q);

afd* determiniser(afnd* B);


dico* etats_accessibles(afnd* B);

afd* determiniser_accessible(afnd* B);

int main(void){

    /*************** Tests pour les automates déterministes ***************/

    //Définition de l'automate A1
    // afd* A1 = initialiser_afd(4, 2, 0);
    // A1->finaux[3] = true;
    // ajout_transition_afd(A1, 0, 'b', 0); ajout_transition_afd(A1, 0, 'a', 1);
    // ajout_transition_afd(A1, 1, 'a', 2); ajout_transition_afd(A1, 1, 'b', 0);
    // ajout_transition_afd(A1, 2, 'a', 2); ajout_transition_afd(A1, 2, 'b', 3);
    // ajout_transition_afd(A1, 3, 'a', 1); ajout_transition_afd(A1, 3, 'b', 0);



    //Définition de l'automate A2 : à compléter !



    //Tests de reconnaissance de mots par A1 et A2
    // char u[] = "abbabbabaab";
    // char v[] = "baababbbbba";
    // char w[] = "aaabababb";
    // printf("%s", u); printf(" est-il reconnu par A1 ? %d\n", reconnu_afd(A1, u));
    // printf("%s", v); printf(" est-il reconnu par A1 ? %d\n", reconnu_afd(A1, v));
    // printf("%s", w); printf(" est-il reconnu par A1 ? %d\n", reconnu_afd(A1, w));
    // printf("%s", u); printf(" est-il reconnu par A2 ? %d\n", reconnu_afd(A2, u));
    // printf("%s", v); printf(" est-il reconnu par A2 ? %d\n", reconnu_afd(A2, v));
    // printf("%s", w); printf(" est-il reconnu par A2 ? %d\n", reconnu_afd(A2, w));

    //Libération de A1 et A2
    // liberer_afd(A1);
    // liberer_afd(A2);


    /*************** Tests pour les automates NON déterministes ***************/

    //Définition de l'auotmate B1
    // afnd* B1 = initialiser_afnd(6, 2);
    // B1->initiaux[0] = true; B1->initiaux[3] = true;
    // B1->finaux[2] = true; B1->finaux[5] = true;
    // ajout_transition_afnd(B1, 0, 'a', 0); ajout_transition_afnd(B1, 0, 'b', 0);
    // ajout_transition_afnd(B1, 0, 'a', 1); ajout_transition_afnd(B1, 1, 'b', 2);
    // ajout_transition_afnd(B1, 3, 'b', 4); ajout_transition_afnd(B1, 4, 'a', 5);
    // ajout_transition_afnd(B1, 5, 'a', 5); ajout_transition_afnd(B1, 5, 'b', 5);



    //Définition de l'automate B2 : à compléter !
   


    //Tests de reconnaissance de mots par B1 et B2
    // printf("%s", u); printf(" est-il reconnu par B1 ? %d\n", reconnu_afnd(B1, u));
    // printf("%s", v); printf(" est-il reconnu par B1 ? %d\n", reconnu_afnd(B1, v));
    // printf("%s", w); printf(" est-il reconnu par B1 ? %d\n", reconnu_afnd(B1, w));
    // printf("%s", u); printf(" est-il reconnu par B2 ? %d\n", reconnu_afnd(B2, u));
    // printf("%s", v); printf(" est-il reconnu par B2 ? %d\n", reconnu_afnd(B2, v));
    // printf("%s", w); printf(" est-il reconnu par B2 ? %d\n", reconnu_afnd(B2, w));

    //Déterminisation de B1 de manière brutale
    // afd* A3 = determiniser(B1);
    //A3 reconnaît il les mêmes mots que B1 ?
    // printf("%s", u); printf(" est-il reconnu par A3 ? %d\n", reconnu_afd(A3, u));
    // printf("%s", v); printf(" est-il reconnu par A3 ? %d\n", reconnu_afd(A3, v));
    // printf("%s", w); printf(" est-il reconnu par A3 ? %d\n", reconnu_afd(A3, w));
    // printf("taille A3 : %d\n", A3->taille_Q);

    //Déterminisation de B2 de manière accessible
    // afd* A4 = determiniser_accessible(B2);
    //A4 reconnaît il les mêmes mots que B2 ?
    // printf("%s", u); printf(" est-il reconnu par A4 ? %d\n", reconnu_afd(A4, u));
    // printf("%s", v); printf(" est-il reconnu par A4 ? %d\n", reconnu_afd(A4, v));
    // printf("%s", w); printf(" est-il reconnu par A4 ? %d\n", reconnu_afd(A4, w));
    // printf("taille A4 : %d\n", A4->taille_Q);

    //Libération de B1,B2,A3,A4
    // liberer_afd(A3);
    // liberer_afd(A4);
    // liberer_afnd(B1);    
    // liberer_afnd(B2);

    return 0;
}