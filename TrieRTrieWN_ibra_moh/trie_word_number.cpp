#include "trie_word_number.h"

#define NB_CHAR 52	// [a-zA-Z]
#define TAILLE_MAX_MOT 256

void update_word_number_recursive(node* my_node_in);

int* tab_word_leaf_linker=NULL;
int nb_element_LLinker=0;

int* RT_tab_word_leaf_linker=NULL;
int RT_nb_element_LLinker=0;

static char** tab_dic=NULL; // un tableau de chaine de caract�re dynamyque
static int nombreMots = 0; // nombre de mots dans notres fichier

node* node_way[TAILLE_MAX_MOT];// tableau qui contient les node passer dans le trie
int idx_node_way=-1;

node* RT_node_way[TAILLE_MAX_MOT];// tableau qui contient les node passer dans le trie
int RT_idx_node_way=-1;

// maper les char [A-Za-z] ==>[0-51]
static inline int get_num_char(char my_char)
{
    if(my_char>=65 && my_char<=90) // A and Z
    {
        return my_char-65;
    }
    else if(my_char>=97 && my_char<=122) // a and Z
    {
        return (my_char-97)+(26);
    }

    return -1; // error.

}

// allouer et initialiser a new node
static inline node* allocate_new_node(char my_char)
{
    node* new_node=(node*)malloc(sizeof(node));

    new_node->element = my_char; // un char de mot
    new_node->nb_left=-1;
    new_node->nb_right=-1;
    new_node->nb_word = -1;
    new_node->pos_in_tab_linker = -1;

    int i=0;
    for(i=0;i<NB_CHAR;i++)
    {
        new_node->tab[i]=NULL;
    }

    // the same thing
    /// memset(new_node->tab,0,NB_CHAR_ARN);

    return new_node;
}

// ajouter le mot a l'arbre et sauvgarder sa position dans tab_dic
// inpute : l'arbre, mot, sa taille, et sa position dans tab_dic
static inline void add_string(node* node_racine,char *string, int size_string,int pos_word)
{
    int i=0;
    int num_char=-1; // [0..51]<==>[A-Za-z]


    node* my_node=node_racine;


    // ajouter le mot � l'arbre caracter par caracter.
    for(i=0;i<size_string;i++)
    {
        num_char = get_num_char( string[i]);

        // si le char n'existe pas parmet les fills dc add new node
        if( my_node->tab[ num_char ]==NULL)
        {
            my_node->tab[ num_char ] = allocate_new_node(string[i]);
            my_node=my_node->tab[ num_char ];
        }
        else // donc on a un fils qui a ce char, donc on recupaire le node fils de cette branche et continue le traitement
        {
            my_node=my_node->tab[ num_char ];
        }
    }

    my_node->nb_word = pos_word; // sauvgarder la position de mot (sa position dans tab_dic)

    return;
}

// compte le nombre de mots dans le fichier
// il sufi de compter le nombre de ligne dans le fichier
// car on met dans chaque ligne un mot.
static int nbMotsInFileDicotionary(FILE *file_dico)
{
    int nombreMots=0;
    char mot[TAILLE_MAX_MOT]; //

    // On compte le nombre de ligne lue avec "fgets"
    // la methode de de compter les ntr�es \n ne marche pas car le dernier ligne ne contien pas un \n.


    while (fgets(mot, TAILLE_MAX_MOT, file_dico) != NULL ) // On lit le fichier tant qu'on ne re�oit pas d'erreur (NULL)
    {
        nombreMots++;
    }

    // On recommence � lire le fichier depuis le d�but.
    // donc on repositionne la tete de lecteur au d�but.
    rewind(file_dico);

    return nombreMots;

}

// le tab_dic contien les mots dans un order selon l'aparition dans le fichier, donc un order quelquan
// lorsque les mots sont ajouter a l'arbre, l'order va etre changer si on les parcueur de node gauche a droite.
// dans l'arbre on va relier tous les node final de gauche a droite,
// comme sa pour un node donn�e on peut acceder a tous ces fils depuit le plus gauche vers le plus a droite
//
// pour cela on va utiliser un tableau qui va contenir les numero des mot dans l'order gauch-->droite
// et dans chaque node on va sauvgarder la position dans le tableau la plus a gauche et la plus a droite
// comme sa on simuler le truc des pointeur avec les adresse

// linker tous les leafs de gauche vers la droite
// mettre les numero des mot (fin de leaf) dans un tableau
// et sauvgarder le numero de la case de tableau qui contien l'information dans le node leaf.
// dc sauvgardi le numero from leaf dans tableau et dit au leaf win khbina le numero de mot :p
void leafs_linker(node *racine)
{
    node* my_node=racine;

    //  si on a un node qui contien un mot (il se peut etre intermidiare ou final)
    // dans le cas intermidi�re, ce node il sera plasser avant tous ces fils.
    if(my_node->nb_word!=-1)
    {

        tab_word_leaf_linker[nb_element_LLinker]=my_node->nb_word; // sauvgarder le numero de mot
        my_node->pos_in_tab_linker = nb_element_LLinker;  // position de mot dans le tab_linker
        nb_element_LLinker++;
    }

    // else , on pas dans un node final, donc on fait appel au meme fonction pour traiter tous les nodes fils.
    int i=0;
    for(i=0;i<NB_CHAR;i++)
    {
        if(my_node->tab[i]!=NULL)
        {
            leafs_linker(my_node->tab[i]);
        }
    }
}

// meme chose pour leafs_linker, mais cette fonction est pour reverse Trie.
void RT_leafs_linker(node *RT_racine)
{
    node* RT_my_node=RT_racine;

    if(RT_my_node->nb_word!=-1)
    {
        RT_tab_word_leaf_linker[RT_nb_element_LLinker]=RT_my_node->nb_word; // sauvgarder le numero de mot
        RT_my_node->pos_in_tab_linker = RT_nb_element_LLinker;  // position de mot dans le tab_linker
        RT_nb_element_LLinker++;
    }

    // else , on pas dans un node final, donc on fait appel au meme fonction pour traiter tous les nodes fils.
    int i=0;
    for(i=0;i<NB_CHAR;i++)
    {
        if(RT_my_node->tab[i]!=NULL)
        {
            RT_leafs_linker(RT_my_node->tab[i]);
        }
    }
}

// afficher tous les mot sortant de node_in d'une fa�on recursive.
void print_all_word_from_prefix(node* node_in)
{
    node* my_node = node_in;

    if(my_node->nb_word!=-1)
    {
        printf("\n %s",tab_dic[my_node->nb_word]);
    }

    // si le node � des node fils
    int i=0;
    for(i=0;i<NB_CHAR;i++)
    {
        if(my_node->tab[i]!=NULL)
        {
            print_all_word_from_prefix(my_node->tab[i]);
        }
    }
}


// afficher tous les mot sotant de node_in
// derectement depuit le tableau qui relier tous les leafs.
void print_all_word_from_prefix_tab_Linker(node* node_in)
{
    int left = node_in->nb_left;
    int right = node_in->nb_right;

    int i=0;
    for(i=left;i<right+1;i++)
    {
        printf("\n %s",tab_dic[tab_word_leaf_linker[i]]);
    }
}

// meme chose que la precednt mais pr les mot depuit reverse Trie.
void print_all_word_from_prefix_RT_tab_Linker(node* node_in)
{
    int left = node_in->nb_left;
    int right = node_in->nb_right;

    int i=0;
    for(i=left;i<right+1;i++)
    {
        printf("\n %s",tab_dic[RT_tab_word_leaf_linker[i]]);
    }
}

// construire le Trie et le revsre Trie et relier leur leafs et sauvgrder les numero des mot dans un tableau.
// mettre tous les mot dans un tableau (tab_dic)
// et ajouter chaque mot a l'arbre et sauvgarder la position de mot qui dans tab_dic dans l'arbre
Trie_R_Trie *build_prefix_tree(char *nameFile)
{

    node* racine = allocate_new_node(0); // crier la racine de l'arbre (avec un char null)
    node* RT_racine = allocate_new_node(0); // crier la racine de reverse Trie (meme chose que trie)

    FILE *file_dico = NULL; // Le pointeur de fichier qui va contenir notre fichier
    char word[(TAILLE_MAX_MOT)]; // un mot, une variable tomporaire.
    char *word_reverse; // un mot, une variable tomporaire.
    int i=0;

    file_dico = fopen(nameFile, "r"); // On ouvre le dictionnaire en lecture seule


    int size_word=0;

    nombreMots=nbMotsInFileDicotionary(file_dico);// une fonction qui donne le nombre de mot dans le dictionnaire

    // allouer l'espace memoire un tableau qui les mots de dictionaire.
    tab_dic=(char**)malloc(nombreMots * sizeof(char *));

    // --------------------------------------------------
    // lire les mots depuit le fichier un par un, et les mettre dans le trie

    while (fgets(word, TAILLE_MAX_MOT, file_dico) != NULL )
    {

        size_word = strlen(word);

        // car on lit toutes la ligne; on supprime le dernier caract�re
        // '\n'; car on a dit que les mots sont dans le fichier dans chaque ligne.
        if(word[size_word-1]=='\n')
        {
            word[size_word-1]='\0';
            size_word--;
        }

        // cette instruction qui alloue l'espace nicissaire pour un mot,
        // l'espace est la taille de mot + 1 (pour le caract�re de fin de mot '\0').
        tab_dic[i]=(char *)malloc((size_word+1)*sizeof(char));

        // copier le mot dans le tableau.
        strcpy(tab_dic[i],word);

        add_string(racine,word,size_word,i);// donn�e : l'arbre, mot, sa taille, et sa position dans tab_dic

        // reverser le mot et ensuit l'inserer dans l'arbre avec le mem numro de mot
        word_reverse = strrev(word); // reverse the word
        add_string(RT_racine,word_reverse,size_word,i);

        i++;
    }

    // --------------------------------------------------


    tab_word_leaf_linker=(int*)malloc(nombreMots*sizeof(int));
    RT_tab_word_leaf_linker=(int*)malloc(nombreMots*sizeof(int));

    for(i=0;i<nombreMots;i++)
    {
        tab_word_leaf_linker[i]=-1;
        RT_tab_word_leaf_linker[i]=-1;
    }
    nb_element_LLinker = 0;
    RT_nb_element_LLinker = 0;


    leafs_linker(racine); // relier tous les  node final de gauche a droite (relier les numero des mots dans l'ordre)
    RT_leafs_linker(RT_racine);



    update_word_number_recursive(racine); // mettre pour chaque node interne le fils le plus a gauche et le plus a droite dans le tableau tab_word_leaf_linker.
    update_word_number_recursive(RT_racine);

    Trie_R_Trie * my_TRT = (Trie_R_Trie*)malloc(sizeof(Trie_R_Trie));

    my_TRT->trie = racine;
    my_TRT->R_trie = RT_racine;

    return my_TRT;
}

void affiche_node_table(node** tab, int size_table)
{
    int i=0;
    printf("\n ----- size_table = %d\n",size_table);
    for(i=0;i<size_table;i++)
    {
        printf("%u , ",tab[i]);
    }
}

// chercher le chemin dans le trie et sauvgarder les node que on a passer par jusqu'� la fin ou la position d'erreur.
node* find_string_node_way(node *node_racine, char *queryWord, int size_queryWord)
{

    /// printf("\n\n in find_string_node_way .... any key to continue..."); getchar();
    int i=0;
    int num_char=0;

    memset(node_way,0,TAILLE_MAX_MOT); // initialize le tableau qui va contenir les nodes par les quelle on passe dans l'arbre dans la recherche de mot.
    idx_node_way = 0;

    /// if(size_queryWord==0) return NULL; // if == 0 we return soit la racine soit NULL

    /*
    printf("\n\n find_string_node for : ");
    for(i=0;i<size_queryWord;i++)
    {
        printf("%c",queryWord[i]);
    }
    printf("\n...."); getchar();
    */


    node* my_node=node_racine;

    node_way[idx_node_way]=node_racine; // ajouter le node racine a la liste
    idx_node_way++;


    for(i=0;i<size_queryWord;i++)
    {
        num_char = get_num_char(queryWord[i]);

        /// printf("\n i=%d , char = %c , num_char=%d ...",i,queryWord[i],num_char); getchar();

        // si le char n'existe pas parmet les fills
        if( my_node->tab[ num_char ]==NULL)
        {
            return NULL; // returner une liste vide
        }
        else // donc on a un fils qui a ce char, donc on recupaire le node fils de cette branche et continue le traitement
        {
            /// printf("\n my_node->tab[ %d ]= %p \n",num_char,my_node->tab[ num_char ]);
            my_node=my_node->tab[ num_char ]; // recupirer le node de num_char actuel.

            node_way[idx_node_way] = my_node; // ajouter le node de char atuel a la liste des nodes
            idx_node_way++;

        }
    }


    return my_node;
}


// chercher le chemin dans le trie et sauvgarder les node que on a passer par jusqu'� la fin ou la position d'erreur.
// cette fonction est pour le mot inverse
// ci la meme fonction que la precedent saufe que on sauvgarde les node dans "RT_node_way", bon je sais que je peut faire sa dans la meme fonction avec des pointeur � l'enrtie de la fonction
node* RT_find_string_node_way(node *node_racine, char *queryWord, int size_queryWord)
{
    int i=0;
    int num_char=0;

    memset(RT_node_way,0,TAILLE_MAX_MOT);
    RT_idx_node_way = 0;

    /// if(size_queryWord==0) return NULL; // if == 0 we return soit la racine soit NULL
/*
    printf("\n\n find_string_node for : ");
    for(i=0;i<size_queryWord;i++)
    {
        printf("%c",queryWord[i]);
    }
    printf("\n...."); getchar();
    */


    node* my_node=node_racine;

    RT_node_way[RT_idx_node_way]=node_racine; // ajouter le node racine a la liste
    RT_idx_node_way++;


    for(i=0;i<size_queryWord;i++)
    {
        num_char = get_num_char(queryWord[i]);

        /// printf("\n i=%d , char = %c , num_char=%d ...",i,queryWord[i],num_char); getchar();


        // si le char n'existe pas parmet les fills
        if( my_node->tab[ num_char ]==NULL)
        {
            return NULL; // returner une liste vide
        }
        else // donc on a un fils qui a ce char, donc on recupaire le node fils de cette branche et continue le traitement
        {
            /// printf("\n my_node->tab[ %d ]= %p \n",num_char,my_node->tab[ num_char ]);

            my_node=my_node->tab[ num_char ];

            RT_node_way[RT_idx_node_way] = my_node;
            RT_idx_node_way++;

        }
    }


    return my_node;
}


// chercher le mot et retourner son node. et NULL si il n'exist pas.
node* find_string_node(node *node_racine, char *queryWord, int size_queryWord)
{
    int i=0;
    int num_char=0;

    /// if(size_queryWord==0) return NULL; // if == 0 we return soit la racine soit NULL

    node* my_node=node_racine;

    for(i=0;i<size_queryWord;i++)
    {
        num_char = get_num_char(queryWord[i]);

        // si le char n'existe pas parmet les fills
        if( my_node->tab[ num_char ]==NULL)
        {
            return NULL; // returner une liste vide
        }
        else // donc on a un fils qui a ce char, donc on recupaire le node fils de cette branche et continue le traitement
        {
            my_node=my_node->tab[ num_char ];
        }
    }


    return my_node;
}

// recupirer le mot qui est represnter par le node_in (ce node doit etre final)
char* get_string_from_node(node *node_in)
{
    if( (node_in==NULL) || (node_in->nb_word==-1))
    {
        return NULL;
    }


    // it's mean that we are at the leaf position
    // or at node intermidi�re qui a une mot, et aussi des fils comme dans le cas IBRA, IBRAHIM, IBRAmim

    /// return "yes yes :) ";
    /// return tab_dic[tab_word_leaf_linker[ node_in->pos_in_tab_linker]];

    // we can use this (ci la meme chose):
    return tab_dic[node_in->nb_word];
}

// ----------------------------------------------------------
// ----------------------------------------------------------
// recupirer le node fils le plus a gauche et returner son position dans le tableau.
// -1 si on a pas de fils.
int get_first_child(node *my_node_in)
{
    int i=0;
    for(i=0;i<NB_CHAR;i++)
    {
        if(my_node_in->tab[i]!=NULL) return i;
    }

    return -1;
}

// recupirer le node fils le plus a droite et returner son position dans le tableau.
// -1 si on a pas de fils.
int get_last_child(node *my_node_in)
{
    int i=0;
    for(i=NB_CHAR-1;i>=0;i--)
    {
        if(my_node_in->tab[i]!=NULL) return i;
    }

    return -1;
}


// remplire les node intern par la position de fils le plus a gauche et le fils le plus a droite
// dans le tableaau "tab_word_leaf_linker" qui relier tous les leafs de gauche vers la droite.
// chaque node re�oie les info de son fils le plus a gauche, et son fils le plus a droite.
// donc on doit avant traiter les node fils ensuit on remante vers la racine.
void update_word_number_recursive(node* my_node_in)
{

    node* my_node=my_node_in;

    // pour tous les fils on fait appel a la fonction pour obtenir l'info, jusqu' au dernier fils et retourne.
    int i=0;
    for(i=0; i<NB_CHAR;i++)
    {
        if(my_node->tab[i]!=NULL)
        {
            update_word_number_recursive(my_node->tab[i]);
        }
    }

    // recupirer le node fils le plus a gauche
    int left = get_first_child(my_node);
    int right = get_last_child(my_node); // le fils right, si on a un seul fils donc il sera le meme que le gauche

    if(left!=-1) // il a des fils  (au moin un seul), on peut tester avec (right!=-1) ci la meme chose.
    {

        // si le node intermidi�re, donc il est avant tous ces fils, dc il sera lui meme le fils le plus agauche.
        if(my_node->nb_word!=-1)
        {
            my_node->nb_left = my_node->pos_in_tab_linker;
        }
        else
        {
            my_node->nb_left = my_node->tab[left]->nb_left;
        }


        my_node->nb_right = my_node->tab[right]->nb_right;

    }
    else // pas de fils, donc il est une leaf :p, donc sa position sera elle meme sa position gauche et doite.
    {
        my_node->nb_left = my_node->pos_in_tab_linker;
        my_node->nb_right = my_node->pos_in_tab_linker;
    }


    return;
}

// ----------------------------------------------------------
// ----------------------------------------------------------

void prinf_tree_recursive(node* my_node_in)
{

    int i=0;

    node* my_node=my_node_in;

    printf("\n - %c : les fils sont",my_node->element);


    for(i=0; i<NB_CHAR;i++)
    {
        if(my_node->tab[i]!=NULL)
        {
            prinf_tree_recursive(my_node->tab[i]);
        }
    }

    return;
}

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

unsigned long size_biteVecteur =0;
unsigned long *biteVecteur=NULL;


char* reverse_string(char *my_string, int size_string)
{
    char *revers_word=(char*)malloc(sizeof(size_string+1));
    memset(revers_word,0,size_string+1);

    int i=0;
    int j=0;

    for(i=size_string-1; i>=0; i--)
    {
        revers_word[j]=my_string[i];
        j++;
    }

    return revers_word;
}

// cette fonction fait la verification (edit distance) pour un seul node, pour eviter l'intersecton avec le node racine
// on a les 1 cas : queryWord = ABCD
// l'erreur est � la fin (example : ABC*) , dc on est dans le node C
// on a :
// 1) deletion : on verife si on a un mot dans le node final (le node de C), donc on trouve ABC.
// 2) substitution : tous les fils de ce node sont des resultat.
// insertion : on a pas insertion dans ce cas car :
// insertion a la fin : xABCD, ABCDx , donc on trouve le mot exact ABCD, et x sera conciderer comme insertion
// insertion au milieu : AxBCD, ABxCD,ABCxD  et ce cas sera inclu dans l'intersection des nodes.
void edit_distance_at_end(node* node_in,char* queryWord, int size_queryWord, int pos_search)
{
    /// printf("\n\n ------------- in edit distanc :");
    /// printf("\n node = %p",node_in);
    /// printf("\n size_queryWord = %d, pos_search =%d , size_queryWord-pos_search = %d", size_queryWord,pos_search , size_queryWord-pos_search);

    int i=0;



    // delete: verifier dans le meme node si on a un mot.
    if(node_in->nb_word !=-1)
    {
        /// printf("\n solution deletion : %s",tab_dic[node_in->nb_word]);
    }

    // substitution : tous les fils sont des resultat de substitution
    for(i=0; i<NB_CHAR;i++)
    {
        if(node_in->tab[i]!=NULL && node_in->tab[i]->nb_word!=-1)
        {
            /// printf("\n solution subs : %s",tab_dic[node_in->tab[i]->nb_word]);
        }
    }

}

// le queryword = ABCD
// insertion a la fin : xABCD, ABCDx , donc on trouve le mot exact ABCD, et x sera conciderer comme insertion
void insert_after_exact(node* node_in)
{
    int i=0;

    // tous les fils de node (D) sont des resultat de insertion , sa resemble � un substitution avec un mot de taille 5 example(abcd* et on trouve abcdx)
    for(i=0; i<NB_CHAR;i++)
    {
        if(node_in->tab[i]!=NULL && node_in->tab[i]->nb_word!=-1)
        {
            /// printf("\n solution insertion : %s",tab_dic[node_in->tab[i]->nb_word]);
        }
    }
}


// faire la recherche approuch�e avec k=1 error  (substitution,deletion, insertion)
// et afficher les resulat.
void approximate_search(Trie_R_Trie* my_TRT,char *queryWord, int size_queryWord)
{

    ///    char *revers_queryWord= strrev(queryWord);
    char *revers_queryWord = reverse_string(queryWord,size_queryWord);


    int i=0;
    int j=0;

    node* node_T;
    node* node_RT;

    // crier le biteVecteur
    biteVecteur = alloc_bit_table(nombreMots,&size_biteVecteur);

    // pour chaque position on cherche la partie gauche dans le trie, et la partie
    // droite dans le reverse trie, et on recup�re leur node et ensuit on fait
    // intersection pour trouver les mot solution



    // chercher le mot dans le trie et sauvgarder le node dans le tableau
    find_string_node_way(my_TRT->trie,queryWord,size_queryWord);

 /*   printf("\n apris first call find_string node_way ...."); getchar();
    printf("\n\n node_racine = %p \n\n",my_TRT->trie);

    for(i=0;i <idx_node_way;i++)
    {
        printf("\n--------------------------------------------");
        printf("\n\n i=%d ,  %p , str = %s \n",i,node_way[i],get_string_from_node(node_way[i]));
        print_all_word_from_prefix_tab_Linker(node_way[i]);
    }

    printf("\n\n ...... any key to continue ..... "); getchar();
    */

    /// if(node_T==NULL) continue;

    RT_find_string_node_way(my_TRT->R_trie,revers_queryWord,size_queryWord);

/*
    printf("\n apris second call Reverse find_string node_way ...."); getchar();
    printf("\n\n RT_node_racine = %p \n\n",my_TRT->R_trie);

    for(i=0;i <RT_idx_node_way;i++)
    {
        printf("\n--------------------------------------------");
        printf("\n\n i=%d ,  %p , str = %s \n",i,RT_node_way[i],get_string_from_node(RT_node_way[i]));
        print_all_word_from_prefix_RT_tab_Linker(RT_node_way[i]);
    }

    printf("\n\n ...... any key to continue ..... "); getchar();

*/
    /// if(node_RT==NULL) continue;


    // --------------------------------------------------------------------------
    //  traiter le cas o� on trouve un mot exact (ou queryword est sous mot d'un autre mot dans le trie)

    // si on trouve un mot exact dans le trie : ABCD
    if(idx_node_way>size_queryWord) // car idx_node_way represent la taille de tableau de node_way
    {
        /// printf("\n we are in if : idx_node_way = %d",idx_node_way);
        insert_after_exact(node_way[ idx_node_way-1]); // car
    }

    // si on trouve un mot exact dans le reverse trie : DCBA
    if(RT_idx_node_way-1==size_queryWord) // meme chose que RT_idx_node_way > size_queryWord
    {
        insert_after_exact(RT_node_way[RT_idx_node_way-1]);
    }

    // ---------------------------------------------------------------------------
    // P_2, P=P_1*P_2  donc : traiter le cas ou P_1 = vide ou P_2 = vide

    // i l'indice de premier part de mot, P_1, et j de 2eme part P_2, P=P_1*P_2

    // i=0 , j=size_queryWord-1;  example : *BCD, mean search in Riverse Trie avec revser word
    j=size_queryWord-1;

    if(j<RT_idx_node_way)
    {
        edit_distance_at_end(RT_node_way[j],revers_queryWord,size_queryWord,j);
    }

    // i=size_queryWord -1; j=0, example : ABC* , serach in Trie with queryword
    i=size_queryWord -1;
    if(i<idx_node_way)
    {
        edit_distance_at_end(node_way[i],queryWord,size_queryWord,i);
    }

    // -------------------------------------------------------------------------
    //     traiter les erreur qui sont entre le 1er char et le dernier char.

    // i commance de 1 --> size_queryWord -2, car on traiter les 2 extrimiter de gauch et droite
    if(idx_node_way>size_queryWord-1)
    {
        idx_node_way = size_queryWord-1;
    }

    /// printf("\n\n intersection boucle....");getchar();

    for(i=1;i<idx_node_way;i++)
    {

        j = size_queryWord-i-1;

        if(j<=0) break; // ce cas ce produit lorsque on a un mot exact (et biensur des resultat approuchee)

        /// printf("\n\n size i= %d , size_reverse j= %d ",i,j); getchar();

        if(j<RT_idx_node_way)
        {
            intersection_get_result(node_way[i],RT_node_way[j]);
        }

    }

}

// faire l'intersection entre les mots fils qui sort de node_T de trie et node_RT de reverse trie
// les mot resultat sont des solution
void intersection_get_result(node* node_T,node* node_RT)
{
    // recupierer laposition debut et fin  de tableau qui relier tous les leaf de node, dc la tous les mots fils de node.
    int left_T = node_T->nb_left;
    int right_T = node_T->nb_right;

    int left_RT = node_RT->nb_left;
    int right_RT = node_RT->nb_right;

    int i=0;

    // ajouter les num de node_T dans le bitVector
    for(i=left_T; i<right_T+1; i++)
    {
        mark_bit(biteVecteur, tab_word_leaf_linker[i]); // == num de word dans tab_dic
    }

    // verifier les num de node_RT si il sont � 1 dans le bit vector, donc on a une solution
    for(i=left_RT; i<right_RT+1; i++)
    {
        if(isMarked_bit(biteVecteur, RT_tab_word_leaf_linker[i]))
        {
            /// printf("\n solution : %s \n",tab_dic[RT_tab_word_leaf_linker[i]]);

            // on peut mettre les num dans un tableau et pui les afficher toutes ensemble.
        }
    }


    // clear the le tableau bitVector , on peut remmtre seulement les bit marki � 1 dans la 1er boucle avec les meme etape et avec la fonction mark_bit_0
    // mais cette fonction est plus rapide car : chaque 32 bite (unsigned long) est remis � "0" dans un seul coup
    clear_bit_table(biteVecteur,size_biteVecteur);
}
