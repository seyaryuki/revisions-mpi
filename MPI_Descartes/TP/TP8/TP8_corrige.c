#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>

#include "dicos.h"

struct ttt {
    int k;
    int n;
    int* grille;
};
typedef struct ttt ttt;

//Question 1.
int numero_case(ttt* jeu, int ligne, int colonne)
{
    return ligne*jeu->n +colonne;
}


//Question 2.
void numeros_lc(ttt* jeu, int position, int* l, int* c)
{
    *l = position / jeu->n;
    *c = position % jeu->n;
}


//Question 3.
ttt* initialiser_jeu(int n, int k)
{
    ttt* jeu = (ttt*)malloc(sizeof(ttt));
    int* grille = (int*) malloc(sizeof(int)*n*n);
    for (int i = 0; i < n*n; i++)
    {
        grille[i] = 0; //Initialisation d'une grille vide.
    }

    jeu->n = n;
    jeu->k = k;
    jeu -> grille = grille;
    return jeu;
}


//Question 4. On libère la grille en premier.
void liberer_jeu(ttt* jeu)
{
    free(jeu->grille);
    free(jeu);
}


//Question 5. Il suffit de traiter le contenu de chaque case de la grille.
int* nb_symboles(ttt* jeu)
{
    int n = jeu->n;
    int* nbs = (int*)malloc(sizeof(int)*3);
    for (int i = 0; i < 3; i++)
    {
        nbs[i] = 0;
    }
    for (int j = 0; j < n*n ; j++)
    {
        int symbole = jeu->grille[j];
        assert (0 <= symbole && symbole <= 2);
        nbs[symbole]++;
    }
    return nbs;
}


//Question 6. On observe combien de cases sont occupées par chacun des joueurs à l'aide de nb_symboles.
int joueur_courant(ttt* jeu)
{
    int* repartition =  nb_symboles(jeu);
    if (repartition[1] == repartition[2]) //Les joueurs ont autant joué l'un que l'autre.
    //Comme on sait que joueur 1 a commencé, c'est à lui de jouer.
    {
        free(repartition);
        return 1;
    }
    else
    {
        free(repartition);
        return 2;
    }
}


//Question 7. 
bool grille_remplie(ttt* jeu)
{
    int* repartition =  nb_symboles(jeu);
    if (repartition[0] == 0) //Si aucune case n'est vide, la grille est remplie.
    {
        free(repartition);
        return true;
    }
    free(repartition);
    return false;

}


//Question 8. Attention, les vérifications sur la licéité de la case choisie pour jouer ne sont pas faites ici d'après l'énoncé : il faudra les prendre en compte dans la fonction finale.
void jouer_coup(ttt* jeu, int l, int c)
{
    int position = numero_case(jeu,l,c);
    jeu->grille[position] = joueur_courant(jeu);
}


//Question 9.
//Considérer les quatre autres directions serait redondant.


//Question 10.
bool alignement(ttt* jeu, int position, int direction, int joueur)
{
    int k = jeu->k;
    int n = jeu->n;
    int dc;
    int dl;

    //Ce bloc permet d'associer à chaque direction d la façon de modifier la ligne et la colonne d'une case c lorsqu'on se déplace dans la direction d. Dans chacun des cas, dc est la quantité à ajouter à la colonne de la case c et dl la quantité à ajouter à la ligne de c pour obtenir les coordonnées de la case adjacente à c dans la direction d. 
    switch ( direction )
    {
        case 0:
            dc = 1; dl = 0;
            break;
        case 1:
            dc = 1; dl = -1;
            break;
        case 2:
            dc = 0; dl = -1;
            break;
        case 3:
            dc = -1; dl = -1;
            break;
    }

    //On aurait pu ici utiliser numeros_lc.
    int ligne = position / n;
    int colonne = position % n;

    //On se déplace de k cases depuis c dans la direction d.
    for (int j = 0; j < k; j++)
    {
        if (colonne < 0 || colonne >= n || ligne < 0 || ligne >= n)
        //Cas où on sort de la grille  : pas d'alignement.
        {
            return false;
        }
        else
        {
            int i = ligne * n + colonne;
            if (jeu->grille[i] != joueur)
            //L'un des k symboles depuis c dans la direction d n'appartient pas au joueur : pas d'alignement.
            {
                return false;
            }
            colonne = colonne + dc;
            ligne = ligne + dl;
        }
    }
    //On a vérifié avec succès que les k symboles à partir de c dans la direction d appartiennent au joueur voulu : il y a un alignement donc le joueur a gagné.
    return true;
}


//Question 11. Pour chaque case c et chacune des 4 directions d, on teste s'il existe un alignement pour le joueur concerné à partir de c dans la direction d.
bool est_final(ttt* jeu, int joueur)
{
    int n = jeu->n;
    for (int i = 0; i < n*n; i++)
    {
        for (int d = 0; d < 4; d++)
        {
            if (alignement(jeu, i, d, joueur))
            {
                return true;
            }
        }
    }
    return false;
}


//Question 12.
int64_t encoder_jeu(ttt* jeu)
{
    int64_t cle = 0;
    int n = jeu->n;
    for (int i=0; i<n*n; i++)
    {
        cle = 3*cle + jeu->grille[i];
    }
    return cle;
}


//Question 13. C'est une traduction de l'algorithme présenté au début de cette partie.
//Dans le cas où la configuration n'est pas finale, on calcule le numéro d'attracteur des configurations voisines récursivement et on en déduit le numéro d'attracteur de la configuration mère. Pour explorer lesdites configurations voisines, on parcourt toutes les cases de la grille et pour chaque case vide, on joue le coup correspondant puis on l'annule après avoir calculé le numéro d'attracteur de la configuration fille.
int attracteur(ttt* jeu, dico* d)
{
    //On récupère l'identifiant de la configuration et le joueur qui doit y jouer.
    int64_t cle = encoder_jeu(jeu);
    int n = jeu->n;
    int joueur = joueur_courant(jeu);

    if (!appartient_dico_cle(d,cle))
    //Cas où on ne connaît pas encore le numéro de l'attracteur de la configuration.
    {
        if (est_final(jeu,joueur)) {ajoute_entree(d, cle, joueur);} //Cas où la configuration est finale gagnante pour l'un.
        else if (est_final(jeu, 3-joueur)) {ajoute_entree(d, cle, 3-joueur);} //Cas où la configuration est finale gagnante pour l'autre.
        else if (grille_remplie(jeu)) {ajoute_entree(d, cle, 0);} //Cas où la configuration est finale sans être gagnante ni pour l'un ni pour l'autre (puisque ces cas sont écartés préalablement) : la partie est nulle.
        else //Cas où la configuration n'est pas finale, commentaires ci-dessus.
        {
            int nb_attracteurs[3] ={0,0,0};
            for (int i = 0; i < n*n; i++)
            {
                if (jeu->grille[i] == 0)
                {
                    jeu->grille[i] = joueur; //on joue le coup
                    int numero_gagnant = attracteur(jeu,d);
                    jeu->grille[i] = 0; //on défait le coup
                    nb_attracteurs[numero_gagnant]++;
                    //if (numero_gagnant == joueur) {break;} //Louis-Max nous suggère qu'on peut ici élaguer !
                }
            }
            if (nb_attracteurs[joueur] != 0) {ajoute_entree(d,cle,joueur);}
            else if (nb_attracteurs[0] != 0) {ajoute_entree(d,cle,0);}
            else {ajoute_entree(d,cle,3-joueur);}
            }
    }
    //A présent, on connaît la valeur cherchée : soit on la connaissait déjà avant de rentrer dans la première conditionnelle, soit on vient de la calculer.
    return valeur_associee(d,cle);
}


//Question 14. On calcule le numéro d'attracteur de la configuration courante et on cherche une configuration voisine qui a le même numéro. Il en existe nécessairement une par définition d'un attracteur.
int coup_optimal(ttt* jeu, dico* d)
{
    int a = attracteur(jeu, d);
    int n = jeu->n;
    int joueur = joueur_courant(jeu);
    for (int i = 0; i < n*n; i++)
    {
        if (jeu->grille[i] == 0)
        {
            jeu->grille[i] = joueur;
            int nouveau_a = attracteur(jeu,d);
            jeu->grille[i] = 0;
            if (a == nouveau_a)
            {
                return i;
            }
        }
    }
    //Si on sort de la fonction via ce return, c'est que le calcul des attracteurs est erroné.
    //Ce return sert à éviter le warning informant que cette fonction doit renvoyer un entier : le compilateur ne peut pas savoir qu'on ne l'atteindra jamais.
    printf("Attention, ce return est signe d'une erreur");
    return (-1);
}


//Question 15. 
void afficher_grille(ttt* jeu)
{
    int n = jeu->n;
    char symbole[3] = {' ', 'X', 'O'};
    printf(" ");
    for (int i = 0; i < n; i++)
    {
        printf(" %d", i);
    }
    printf("\n");
    for (int l = 0; l < n; l++)
    {
        printf("%d|", l);
        for (int c = 0; c < n; c++)
        {
            int pos = n*l+c;
            printf("%c|",symbole[jeu->grille[pos]]);
        }
        printf("\n");
    }
    printf("\n");
}


//Question 16.
void jouer_partie(int n, int k)
{
    //Initialisation du jeu.
    ttt* jeu = initialiser_jeu(n,k);

    //Détermination de qui est joueur 1, qui est joueur 2.
    char decision = 'x';
    int ordinateur = -1;
    printf("Voulez-vous commencer ? (o/n) ");
    scanf("%c", &decision);
    if (decision == 'o') ordinateur = 2;
    else ordinateur = 1;

    //Création des variables destinées à tracer l'évolution du jeu.
    dico* d = creer_dico();
    int joueur = 1;
    int colonne = -1;
    int ligne = -1;
    int erreur = 0;

    while (!grille_remplie(jeu) && !est_final(jeu,1) && !est_final(jeu,2))
    //Tant que la partie n'est pas terminée.
    {
        afficher_grille(jeu);
        if (joueur == ordinateur)
        //Cas où l'ordinateur joue.
        {
            int i = coup_optimal(jeu,d);
            //On pourrait ici utiliser numeros_lc.
            colonne = i%n;
            ligne = i/n;
            printf("Je joue en ligne %d, colonne %d \n", ligne, colonne);
            jouer_coup(jeu,ligne, colonne);
        }
        else
        //Cas où l'humain joue.
        {
            printf("C'est à vous de jouer \n");
            printf("Saisissez la ligne : ");
            scanf("%d", &ligne);
            printf("Saisissez la colonne : ");
            scanf("%d", &colonne);
            //Vérification de la licéité de la case jouée par l'humain.
            //Si elle est illégale, on l'indique via le flag erreur et on interrompt la boucle while à l'aide de break.
            if (ligne < 0 || ligne >= n || colonne < 0 || colonne >=n)
            {
                erreur = 1;
                printf("Cette case ne fait pas partie de la grille \n");
                break;
            }
            int position = ligne*n + colonne;
            if (jeu->grille[position] != 0)
            {
                erreur = 1;
                printf("Cette case est déjà pleine \n");
                break;
            }
            jouer_coup(jeu,ligne,colonne);
        }
        joueur = joueur_courant(jeu);
    }
    //Affichage de la grille finale.
    afficher_grille(jeu);

    if (erreur)
    //Messages finaux et libérations dans le cas d'un coup illicite.
    {
        printf("Il faut revoir les règles du jeu. \n");
        liberer_dico(d);
        liberer_jeu(jeu);
    }
    else
    //Messages finaux et libérations si la partie s'est correctement terminée.
    {
        if (est_final(jeu,ordinateur)) printf("J'ai gagné ! \n");
        else 
        {
            if (est_final(jeu, 3-ordinateur)) printf("Vous avez gagné ! \n");
            else printf("Match nul \n");
        }
        liberer_dico(d);
        liberer_jeu(jeu);
    }
}

//Bonus : Ne vous fatiguez néanmoins pas trop, vous ne pourrez a priori améliorer les performances que d'un facteur constant. En effet, le facteur limitant dans cet algorithme est la taille des clés qu'il est possible de manipuler dans le dictionnaire. Même en autorisant des clés de 64 bits ainsi que l'énoncé le propose, on ne pourra pas aller au dela de grilles de taille 6 (puisque 3**(7**2) > 2**64) Comme les calculs répétés plusieurs fois consistent en des parcours de la grille et que celle ci ne pourra pas contenir plus de 36 cases, on s'attend donc à ce que les calculs inutiles n'augmentent la complexité que d'un facteur constant.

//En revanche, vous pouvez essayer d'implémenter un min-max avec heuristique et profondeur d'exploration bornée pour essayer d'obtenir une stratégie "bonne" à défaut d'être optimale sur des grilles de grandes taille.


int main()
{
    jouer_partie(3,3);
    return 0;
}