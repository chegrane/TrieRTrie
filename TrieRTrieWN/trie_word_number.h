#ifndef SUFFIXE_TREE_H
#define SUFFIXE_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "bit_vector.h"


#ifndef __cplusplus
#define bool int
#define true 1
#define false 0
#endif


typedef struct node
{
    char element;

    int nb_left;
    int nb_right;
    int nb_word;    // la position de mot dans le tableau tab_dic (qui contient tous les mot de dic)
    int pos_in_tab_linker; // position de mot dans le tab_linker, le num (nb_word) dans tab_word_leaf_linker

    struct node* tab[52]; // we have 26 char min and 26 chars Maj, dc 52 possible child.

}node;

typedef struct Trie_R_Trie
{
    struct node* trie;
    struct node* R_trie;

}Trie_R_Trie;


Trie_R_Trie* build_prefix_tree(char *nameFile);

node* find_string_node(node* node_racine,char *queryWord, int size_queryWord);
char* get_string_from_node(node *node_in);

// just for test and debuging.
void prinf_tree_recursive(node* my_node_in);
void print_all_word_from_prefix(node* node_in);
void print_all_word_from_prefix_tab_Linker(node* node_in);
void print_all_word_from_prefix_RT_tab_Linker(node* node_in);

void approximate_search(Trie_R_Trie *my_TRT, char *queryWord, int size_queryWord);
void intersection_get_result(node* node_T,node* node_RT);
void intersection_Racine_get_result(node* node_T,node* node_RT, int size_word);


#endif // SUFFIXE_TREE_H
