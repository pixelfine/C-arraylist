
#ifndef PROJET2019_H_
#define PROJET2019_H_

#include <stdint.h>
#include <stddef.h>

typedef union{
	intmax_t a;			//8 bytes
	void *adr;			//8 bytes
	long double c; 		//16 bytes
}align_data;			//16 bytes

typedef struct{
	ptrdiff_t next; 	//8 bytes
	ptrdiff_t previous; //8 bytes
	size_t len; 		//8 bytes
	align_data data[];	//"_ bytes"
}node;                  //32 bytes

typedef struct{
	ptrdiff_t suivant;
	size_t nb_blocs;
} entete_tranche;



typedef struct{
	void *memory; //pointeur vers la memoire
	ptrdiff_t first; //ptrdiff_t ou pointeur
	ptrdiff_t last; //ptrdiff_t ou pointeur
	ptrdiff_t libre; //ptrdiff_t ou pointeur si la liste de tranches
	size_t nbLibre;
	size_t nbElem;
	size_t size; //size en octet
	//toute autre information
} head;


void * ld_creer( size_t nboctets );
void *ld_first(void *liste);
void *ld_last(void *liste);
void *ld_next(void *liste, void *current);
void ld_destroy(void *liste);
size_t ld_get(void *liste, void *current, size_t len, void *val);
void * ld_insert_first(void *liste, size_t len, void *p_data);
void * ld_insert_last(void *liste, size_t len, void *p_data);
void * ld_insert_before(void *liste, void *n, size_t len, void *p_data);
void * ld_insert_after(void *liste, void *n, size_t len, void *p_data);
void * ld_delete_node(void *liste, void *n);
size_t ld_total_free_memory(void *liste);
size_t ld_total_useful_memory(void *liste);
void *ld_add_memory(void *liste, size_t nboctets);
void *ld_compactify(void *liste);


#endif /* PROJET2019_H_ */
