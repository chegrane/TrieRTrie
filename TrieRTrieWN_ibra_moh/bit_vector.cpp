#include "bit_vector.h"

#define bits_per_byte 8
#define word_size (sizeof(unsigned long)*bits_per_byte)

void print_bit_table(unsigned long * bit_table,unsigned long size_bit_table)
{
    int i=0;
    int nb_bites = size_bit_table * word_size;

    printf("\n\n printf all bite (%d) of bit vecotr :\n",nb_bites);
    for(i=0;i<nb_bites;i++)  //  all position
    {
        if (isMarked_bit(bit_table,i))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
}

// allouer l'espace memoire pour le bitVecteur et retourner le buffer
// et aussi retourn le nombre de mot dans le bitVecteur
unsigned long * alloc_bit_table(unsigned long nbits,unsigned long * _rounded_nwords)
{

    // compter le nombre de mot_memoire aroundi vers l'entier superieur (taille de mot memoire est word_size == 32 bites)
    unsigned long rounded_nwords=(nbits+word_size-1)/word_size;

    unsigned long *out_buffer=(unsigned long *)calloc(rounded_nwords,sizeof(unsigned long)); // allocation avec initialisation a 0

    *(_rounded_nwords)=rounded_nwords;

    return out_buffer;

}

// free space memory
void free_bit_table(unsigned long * bit_table)
{
    free(bit_table);
    bit_table = NULL;

    return;
}

void clear_bit_table(unsigned long * bit_table, unsigned long size_table)
{
    /// memset(bit_table,0,size_table); // memset don't work to re-initialise the bit_table to (0)!!!??? i don't know why?

    int i=0;

    for(i=0;i<size_table;i++)
    {
        bit_table[i]=0;
    }
}

// mettre dans le bitVecteur '1' pour dire qu'on a un elemeent
void mark_bit(unsigned long * bit_vec,unsigned long pos)
{
    unsigned long bit_pos = pos % word_size; // trouver la position de bite dans le word memoire
    unsigned long word_pos = pos/word_size;  // trouver le numero de mot memoire

    // donc le pos de bite = numero de mot memoir + position dans ce mot memoire

    bit_vec[word_pos] |= (1<<bit_pos);

    return;
}



void mark_bit_0(unsigned long * bit_vec,unsigned long pos)
{
    unsigned long bit_pos = pos % word_size; // trouver la position de bite dans le word memoire
    unsigned long word_pos = pos/word_size;  // trouver le numero de mot memoire

    // donc le pos de bite = numero de mot memoir + position dans ce mot memoire

    bit_vec[word_pos] &= (~(1<<bit_pos));

    return;
}


// verifier dans bitVecteur si tab[pos] == 1
unsigned long isMarked_bit(unsigned long * bit_vec,unsigned long pos)
{
    unsigned long bit_pos = pos % word_size;

    unsigned long word_pos = pos/word_size;

    return bit_vec[word_pos]&(1<<bit_pos); //  donc 1 ou 0  , true ou false
}

unsigned long isMarked_1_and_mark_0(unsigned long * bit_vec,unsigned long pos)
{
    unsigned long res;

    unsigned long bit_pos = pos % word_size;
    unsigned long word_pos = pos/word_size;

    res= bit_vec[word_pos]&(1<<bit_pos); //  donc 1 ou 0  , true ou false

    bit_vec[word_pos] &= (~(1<<bit_pos));

    return res;
}
