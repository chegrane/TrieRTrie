
#include "test.h"

#define TAILLE_MAX_MOT 256

/// :::::::::::::::::::::::::::::::::     les prototypes des fonctions  static
static inline char* insert_char_word(char* word_in,int sizeWord_in,char SL,int pos_INSER);
static inline char* del_char_word(char* word_in,int sizeWord_in,int pos_DEL);


/// :::::::::::::::::::::::::::::::::     les variables  static
// vide

/// :::::::::::::::::::::::::::::::::     les fonction


// initialiser le generateur de nombre aleatoire.
inline void initialise_genirateur_rand()
{
    time_t t; // une variable time; pour initialiser le generateur de pseudo aleatoire.

    // initialise le generateur de nombre pseudo aléatoire.
    // avec le temps actuel de la machine.
    // la fonction time(&t) renvoie le temps de la machine.
    srand( (unsigned) time(&t));
    ///srand(RANDOM_INITIALIZE);
}

inline int randomeErorr_1_test()
{
    return ( rand() % (NB_CASE_ERROR_1) );
}

inline int randome_PosErorr_1_test(int size)
{
    size++; // car lorsque on fait "%" les valeur sont de 0-->size-1, donc on ajoute 1, pour que les resultat seront à size.

    return ( rand() % (size) );
}

inline char *randomeWord_test(char *originale_word)
{
    int size_originale_word=strlen(originale_word);
    int case_error=randomeErorr_1_test();
    int pos_erreur=0;

    char *word_withe_erreur=NULL;
    char *word_withe_delete=NULL;

    switch(case_error)
    {
    case SUBSTITUTION_1:
    {
        pos_erreur=randome_PosErorr_1_test(size_originale_word-1);
        word_withe_erreur=(char *)malloc((size_originale_word+1)*sizeof(char));
        strcpy(word_withe_erreur,originale_word);

        /// word_withe_erreur[pos_erreur]= char_error;
        if(word_withe_erreur[pos_erreur]==char_error)
        {
            word_withe_erreur[pos_erreur]=char_error_q; // char 'q'
        }
        else
        {
            word_withe_erreur[pos_erreur]=char_error; // char 'z'
        }

        return word_withe_erreur;
    }

    case INSERT_1:
    {
        pos_erreur=randome_PosErorr_1_test(size_originale_word);
        return (insert_char_word(originale_word,size_originale_word,char_error,pos_erreur));
    }

    case DELETE_1:
    {
        pos_erreur=randome_PosErorr_1_test(size_originale_word-1);

        word_withe_delete = del_char_word(originale_word, size_originale_word, pos_erreur);

        return (word_withe_delete);
    }

    default : return NULL;
    }

    return NULL;
}

inline int exist_indice_in_table(int *tab_indiceTestWord,int indiceTable)
{
    int i=0;
    for(i=0;i<NB_QUERY_WORD_TEST;i++)
    {
        if(tab_indiceTestWord[i]==indiceTable) return 1;
    }

    return 0;
}


int tester (char *nameFile)
{

    // on doit initialiser la fonction generateur de nombre random.
    initialise_genirateur_rand();

    // nameFile : le nom de fichier qui contien les mot de notre dictionnaie.

    FILE * File_CSV=NULL;  // les resulta des teste seront ecrit dans un fichier result
    File_CSV=fopen("result.csv","a"); // ovrire le fichier en mode add

    if (File_CSV == NULL) // Si on n'a PAS réussi à ouvrir le fichier
    {
        printf("\n Impossible de crier le fichier ");
        return 0;
    }


    /// double Size_Dic_Exact_Mo = 0; // on a pas besoin de sa ici :p
    /// double size_Dic_1Error_Mo = 0;

    double Time_Creat_Dic_1error_Mill_s = 0;

    double TimeSearch_Exac_1000query_Mic_s = 0;
    double TimeSearch_1Error_1000query_Mic_s = 0;


    /// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    int i=0; // compteur.

    LARGE_INTEGER start, end, freq;
    double elapsed;

    QueryPerformanceFrequency(&freq);

    ///::::::::::::::::::::::::::::::::::::::::::::: creation de dictionnaire for 1 error:
    printf("\n\n ................ creation de dictionnaire for 1 error:");

    QueryPerformanceCounter(&start);     //   Lancement de la mesure

    Trie_R_Trie *my_TRT = build_prefix_tree(nameFile);

    QueryPerformanceCounter(&end);       // Arret de la mesure

    elapsed = (MILLI_S * (end.QuadPart - start.QuadPart)) / freq.QuadPart; // Conversion en millsecondes
    printf("\n %.0f MILLI-secondes  Time to creat DIC.\n", elapsed);
    Time_Creat_Dic_1error_Mill_s = elapsed;
    ///getchar();


    ///:::::::::::::::::::::::::::::::::::::::::::::::::::  la recherche dans le dictionnaire

    // 1  : creation de tableau qui contien les mots test  (1000 random mots test avec random error)

    printf("\n before creation de tableau des testss.");

    FILE *file_dico = NULL; // Le pointeur de fichier qui va contenir notre fichier
    char mot[TAILLE_MAX_MOT]; // un mot, une variable tomporaire.


    file_dico = fopen(nameFile, "r"); // On ouvre le fichier en lecture seule

    // On vérifie si on a réussi à ouvrir le dictionnaire
    if (file_dico == NULL) // Si on n'a PAS réussi à ouvrir le fichier
    {
        printf("\n Impossible de charger le dictionnaire de mots");
        return 0; // On retourne 0 pour indiquer que la fonction a échoué
        // A la lecture du return, la fonction s'arrête immédiatement.
    }

    // tableau qui va contenir les query word, pour faire le teste (temps d'execution).
    char **tab_queryWord = NULL;
    char **tab_queryWord_exact = NULL;
    tab_queryWord = (char **)malloc(NB_QUERY_WORD_TEST*sizeof(char *));
    tab_queryWord_exact = (char **)malloc(NB_QUERY_WORD_TEST*sizeof(char *));

    int *tab_size_queryWord =(int *)malloc(NB_QUERY_WORD_TEST*sizeof(int)); // tab va contenir les legnht des mots
    int *tab_size_queryWord_exact =(int *)malloc(NB_QUERY_WORD_TEST*sizeof(int));


    // calculer le nombre de mot dans le fichier
    // pour extraire 1000 mot d'une façon aleatoire pour faire les teste.
    int nb_word=0;
    while (fgets(mot, TAILLE_MAX_MOT, file_dico) != NULL ) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
    {
        nb_word++;
    }
    rewind(file_dico);

    // lire les mots depuit le fichier un par un, et les mettre dans
    // le tableau "hashTable".
    int nb_word_readed=0;
    int indiceTable=0;

    // crier un tableau qui va contenir des mot alleatoire du notre dictionnaire
    // pour faire le teste.
    int tab_indiceTestWord[NB_QUERY_WORD_TEST]={0};
    // recupirer les indice aleatoire et les mettre dans le tableau, un nombre aleatoire doit etre unique dans le tableau.
    for(i=0;i<NB_QUERY_WORD_TEST;)
    {
        indiceTable= rand() % (nb_word-1); // -1 pour eviter le dernier mot qui ne contient pas de "\n"

        if(exist_indice_in_table(tab_indiceTestWord,indiceTable)==0)
        {
            tab_indiceTestWord[i]=indiceTable;
            i++;
        }
    }

    indiceTable=0;
    int sizeWord = 0;
    // recupirer les mots qui corespond au indice aleatoire sauvgardie precedement.
    while (fgets(mot, TAILLE_MAX_MOT, file_dico) != NULL /*&& nb_word_readed<NB_QUERY_WORD_TEST */) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
    {
        sizeWord=strlen(mot);

        // car on lit toutes la ligne; on supprime le dernier caractère
        // '\n'; car on a dit que les mots sont dans le fichier dans chaque ligne.
        if(mot[sizeWord-1]=='\n')
        {
            mot[sizeWord-1]='\0';
            sizeWord--;
        }

        if(exist_indice_in_table(tab_indiceTestWord,nb_word_readed)==1)
        {
            tab_queryWord[indiceTable]=randomeWord_test(mot);
            tab_size_queryWord[indiceTable] = strlen(tab_queryWord[indiceTable]);// on recalcule la taille car on sais pas si on a fait substituion,deletion ou insertion

            tab_queryWord_exact[indiceTable]=(char*)malloc((sizeWord+1)*sizeof(char));
            strcpy(tab_queryWord_exact[indiceTable],mot);

            tab_size_queryWord_exact[indiceTable] = sizeWord; // the same sizeword

            indiceTable++;
        }

        nb_word_readed++;
    }

    // fermer le fichier.
    fclose(file_dico);


    printf("\n after creation de tableau des testss.");

    /// ::::::::::::::::::::::::::::::::::::::::::::::: la recherche exacte :
    printf("\n\n exacte research : .............................");

    QueryPerformanceCounter(&start);     //   Lancement de la mesure

    for(nb_word_readed=0;nb_word_readed<NB_QUERY_WORD_TEST;nb_word_readed++)
    {
        approximate_search(my_TRT,tab_queryWord_exact[nb_word_readed],tab_size_queryWord_exact[nb_word_readed]);
    }

    QueryPerformanceCounter(&end);       // Arret de la mesure

    elapsed = (MICRO_S * (end.QuadPart - start.QuadPart)) / freq.QuadPart; /// Conversion en millsecondes
    printf("\n %.0f MICRO-secondes one word.\n", elapsed/NB_QUERY_WORD_TEST);
    printf("\n %.0f MICRO-secondes %d word.\n", elapsed,NB_QUERY_WORD_TEST);
    TimeSearch_Exac_1000query_Mic_s = elapsed;

    ///getchar();


    /// ::::::::::::::::::::::::::::::::::::::: la recherche approchée avec "1" error
    printf("\n\n approximate research '1' error : .............................");
    QueryPerformanceCounter(&start);     ///   Lancement de la mesure

    for(nb_word_readed=0;nb_word_readed<NB_QUERY_WORD_TEST;nb_word_readed++)
    {
        /// printf("\n idx = %d , word = %s , size_ftab = %d , size_strl =%d",nb_word_readed,tab_queryWord[nb_word_readed],tab_size_queryWord[nb_word_readed],strlen(tab_queryWord[nb_word_readed]));
        approximate_search(my_TRT,tab_queryWord[nb_word_readed],tab_size_queryWord[nb_word_readed]);
    }

    QueryPerformanceCounter(&end);       /// Arret de la mesure

    elapsed = (MICRO_S * (end.QuadPart - start.QuadPart)) / freq.QuadPart; /// Conversion en millsecondes
    printf("\n %.0f MICRO-secondes one word.\n", elapsed/NB_QUERY_WORD_TEST);
    printf("\n %.0f MICRO-secondes %d words.\n",elapsed,NB_QUERY_WORD_TEST);

    TimeSearch_1Error_1000query_Mic_s=elapsed;

    ///getchar();

    ///::::::::::::::::::::::::::::::::::::
    ///::::::::::::::::::::::::::::::::::::
  /*
    printf("\n\n before free space tab");
    for(nb_word_readed=0;nb_word_readed<NB_QUERY_WORD_TEST;nb_word_readed++)
    {
        free(tab_queryWord[nb_word_readed]);
        ///free(tab_queryWord_exact[nb_word_readed]);
    }
    free(tab_queryWord);
    ///free(tab_queryWord_exact);

    printf("\n\n after free space tab");
*/
    ///::::::::::::::::::::::::::::::::::::
    ///::::::::::::::::::::::::::::::::::::


    /// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    /// ::::::::::::::::: ecrir les resultat dans fichier csv

    /// printf("\n ecrir les resultat ou non   o/n : ");
    /// char lire;
    /// scanf("%c",&lire);

    /// if(lire!='n')
    {
        fprintf(File_CSV," %f ; %f ; %f \n",Time_Creat_Dic_1error_Mill_s,TimeSearch_Exac_1000query_Mic_s,TimeSearch_1Error_1000query_Mic_s);
    }

    fclose(File_CSV);

    return 0;

    ///  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

// dans cette classe on a besoin de crier un espace memoire pour chaque Mot (pas comme "approximate_search")
static inline char* insert_char_word(char* word_in,int sizeWord_in,char SL,int pos_INSER)
{
#ifdef DEBUG
    if(pos_INSER<0 || pos_INSER >sizeWord_in )
        return NULL;
#endif

    ///char word_out[TAILLE_MAX_MOT];
    char* word_out=NULL;
    word_out=(char *)malloc((sizeWord_in+2)*sizeof(char)); // +2 : un pour le char en plus et l'autre pour '\0'

    int i=0;//compteur

    for(i=0;i<pos_INSER;i++)
    {
        word_out[i]=word_in[i];
    }

    word_out[i]=SL;

    for(;i<sizeWord_in;i++)
    {
        word_out[i+1]=word_in[i];
    }

    word_out[i+1]='\0';

    return word_out;

}

static inline char* del_char_word(char* word_in,int sizeWord_in,int pos_DEL)
{
#ifdef DEBUG
    if(pos_DEL<0 || pos_DEL>=sizeWord_in)
        return NULL;
#endif

    ///char word_out[TAILLE_MAX_MOT];
    char* word_out=NULL;

    word_out=(char *)malloc((sizeWord_in)*sizeof(char)); // on va supprimer un char.

    int i=0;//compteur

    for(i=0;i<pos_DEL;i++)
    {
        word_out[i]=word_in[i];
    }

    i++;

    for(;i<sizeWord_in;i++)
    {
        word_out[i-1]=word_in[i];
    }
    word_out[i-1]='\0';

    return word_out;
}
