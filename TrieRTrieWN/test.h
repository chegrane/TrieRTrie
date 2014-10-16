#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <windows.h>
#include <time.h>

#include "trie_word_number.h"

#define NB_QUERY_WORD_TEST 100  /// nombre de query word pour le teste

/// #define  char_error '*' /// le char error a inserer ou substituer (car les dictionnaire contient que de l'alphabet)
#define  char_error 'z' /// on a un tab de 52 char [A-Za-z], on a pris le 'z' comme error car il est le moit frequant dans la langue englais regard : (http://en.wikipedia.org/wiki/Letter_frequency)
                        /// bon on peut verifier dans le mot ci le char == z donc il suffi just de mettre un autre char example : 'q'
#define  char_error_q 'q' // ci le char == z, donc on utilise le 2eme char comme on est sur que on une erreur.
#define NB_CASE_ERROR_1 3  /// dans le cas '1' error, on a 3 type d'error.
#define RANDOM_INITIALIZE 100 /// un nombre pour initialiser le generateur des nombre aleatoire.(on utilise sa a la place de temps pour tomber dans des petit nombre bon je pense).

#define MILLI_S 1000.0           /// 1 milli second (ms)      ----> pow(10,-3) s
#define MICRO_S 1000000.0        /// 1 micro second (micro s) ----> pow(10,-6) s
#define NANO_S  1000000000.0     /// 1 nano second (ns)       ----> pow(10,-9) s
#define BICO_S  1000000000000.0  /// 1 bico second (bs)       ----> pow(10,-12) s


enum {SUBSTITUTION_1=0,INSERT_1=1,DELETE_1=2}; /// les types des error dans le cas simple '1' error


inline void initialise_genirateur_rand();
inline int randomeErorr_1_test();
inline int randome_PosErorr_1_test(int size);
inline char *randomeWord_test(char *originale_word);
inline int exist_indice_in_table(int *tab_indiceTestWord,int indiceTable);

int tester(char *nameFile);

#endif // TEST_H_INCLUDED
