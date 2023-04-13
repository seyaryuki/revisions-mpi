struct maillon {
    int val;
    struct maillon* suivant;
};

typedef struct maillon maillon;
typedef maillon* liste;

struct maillon_couple {
    int cle;
    int valeur;
    struct maillon_couple* suivant;
};

typedef struct maillon_couple maillon_couple;
typedef maillon_couple* liste_couples;

typedef struct dico {
    int capacite;
    int nb_elements;
    liste_couples* donnees;
} dico;

void liberer_liste(liste l);

liste cons(int x, liste l);

void liberer_liste_couples(liste_couples l);

liste_couples cons_couple(int k, int v, liste_couples l);

void liberer_dico(dico* d);

dico* creer_dico(void);

int taille_dico(dico* d);

int hash(int taille_dico, int k);

bool appartient_liste_cle(liste_couples l, int k);

bool appartient_dico_cle(dico* d, int k);

int valeur_associee(dico* d, int k);

int obtenir_cle(dico* d);

void ajoute_entree_base(dico* d, int c, int v);

void redimensionne(dico* d, int t);

void ajoute_entree(dico* d, int k, int v);

void supprime_entree(dico* d, int k);

liste liste_cles(dico* d);