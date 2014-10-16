#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "trie_word_number.h"




int main()
{

    char *nameFile="dico.txt";

int nb_iteration =10;
int i;

for(i=0; i<nb_iteration;i++)
{
    tester (nameFile);

    printf("\n\n =========== iteration %d  okay",i);
}



    /// steps to use the cose to approximate search.
/*
    Trie_R_Trie *my_TRT = build_prefix_tree(nameFile);

    printf("\n \n constructio okay okay - any key to continue... ");
    getchar();

    char *queryWord="AB";
    int size_queryword=strlen(queryWord);

    printf("\n\n befor approximate search ...................... \n ");

    approximate_search(my_TRT,queryWord,size_queryword);

    printf("\n\n after approximate search ...................... ");
*/

    printf("\n\n");
    return 0;
}

