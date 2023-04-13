struct maillon {
    int val;
    struct maillon* suivant;
};

typedef struct maillon maillon;
typedef maillon* liste;

struct maillon_couple {
    int64_t cle;
    int valeur;
    struct maillon_couple* suivant;
};

typedef struct maillon_couple maillon_couple;
typedef maillon_couple* liste_couples;

//Un dictionnaire est représenté par un tableau dans lequel les collisions sont gérées par chaînage.
//Le champ capacite correspond à la taille de ce tableau.
//Le champ nb_elements correspond au nombre d'éléments réellement présents dans le dictionnaire.
typedef struct dico {
    int capacite;
    int nb_elements;
    liste_couples* donnees;
} dico;

//Libère toute la mémoire réservée sur le tas pour une liste d'entiers.
void liberer_liste(liste l);

//Ajoute en tête l'élément x à une liste l.
liste cons(int x, liste l);

//Libère toute la mémoire allouée sur le tas pour une liste de couples.
void liberer_liste_couples(liste_couples l);

//Ajoute en tête un couple (k,v) à la liste de couples l
liste_couples cons_couple(int64_t k, int v, liste_couples l);

//Libère toute la mémoire utilisée par le dictionnaire (pointé par) d
void liberer_dico(dico* d);

//Créé un (pointeur sur un) dictionnaire vide
dico* creer_dico(void);

//Indique le nombre d'éléments stockés dans le dictionnaire (pointé par) d
int taille_dico(dico* d);

//Calcule le haché d'un entier k selon un dictionnaire dont la taille est taille_dico
int hash(int taille_dico, int64_t k);

//Indique vrai ssi k est le premier champ d'un couple dans la liste de couples l
bool appartient_liste_cle(liste_couples l, int64_t k);

//Indique vrai ssi k est la clé d'un des éléments stockés dans le dictionnaire (pointé par) d
bool appartient_dico_cle(dico* d, int64_t k);

//Renvoie la valeur associée à la clé k dans le dictionnaire (pointé par) d
int valeur_associee(dico* d, int64_t k);

//Renvoie une des clés d'un élément présent dans le dictionnaire (pointé par) d
int obtenir_cle(dico* d);

//Ajoute une association (clé valeur) au dictionnaire (pointé par) d sans redimensionner le dictionnaire.
void ajoute_entree_base(dico* d, int64_t c, int v);

//Redimensionne la taille de la table de hachage du dictionnaire (pointé par) d 
//de sorte à ce que sa nouvelle taille soit t.
void redimensionne(dico* d, int t);

//Ajoute une association (clé,valeur) au dictionnaire (pointé par) d.
//Si la table de hachage est trop plein, on double sa taille.
void ajoute_entree(dico* d, int64_t k, int v);

//Supprime une association (clé,valeur) du dictionnaire (pointé par) d.
//Si la table de hachage est trop peu pleine on divise sa taille par 2.
void supprime_entree(dico* d, int64_t k);

//Renvoie la liste des clés du dictionnaire (pointé par) dico.
liste liste_cles(dico* d);