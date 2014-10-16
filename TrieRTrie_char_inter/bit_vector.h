#ifndef BIT_VECTOR_H
#define BIT_VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long * alloc_bit_table(unsigned long nbits,unsigned long * _rounded_nwords);

void free_bit_table(unsigned long * bit_table);

void clear_bit_table(unsigned long * bit_table, unsigned long size_table);

void mark_bit(unsigned long * bit_vec,unsigned long pos);
void mark_bit_0(unsigned long * bit_vec,unsigned long pos);

unsigned long isMarked_bit(unsigned long * bit_vec,unsigned long pos);

unsigned long isMarked_1_and_mark_0(unsigned long *bit_vec,unsigned long pos);

void print_bit_table(unsigned long * bit_table,unsigned long size_bit_table);

#endif // BIT_VECTOR_H
