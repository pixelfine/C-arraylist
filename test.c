#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "projet2019.h"
#include "test.h"
#include "listes_generiques.h"
//#include <time.h>
#include <sys/time.h>

static size_t nb_blocs(size_t o){
	size_t count=1;
	size_t tmp = sizeof(align_data);
	while(o>tmp){
		count++;
		tmp+=sizeof(align_data);
	}
	return count;
}

static size_t lg_getSize(liste l){
	size_t size=0;
	liste q;
	q = l->suivant;
	while( !est_tete(q) ){
		size++;
	    q = suivant(q);
	}
	return size;
}

static node* ld_getIndex(head* h, int index){
	node *currN=(node*)((align_data *)h+h->first);
	for(int i=0; i<index; i++){
		currN=(node *)((align_data*)currN+currN->next);
	}return currN;
}

static liste lg_getIndex(liste l, int index){
	liste q;
	int count=0;
	q = l->suivant;
	while( !est_tete(q) ){
		if(index==count){
			return q;
		}
		count++;
	    q = suivant(q);
	}
	return NULL;
}


//1 pour tout print, 2 pour print les tranches libre, 3 pour les nodes
void debugg(head *h, int num){
	printf("\n\n_____________DEBUG___________\n\n");
	printf("head :\n");
	printf("Adress : %p\tData : %p\t Val : h\tBloc : 0\n", &h, h);
	printf("Adress : %p\tData : %d\t Val : h->first\n", &h->first,(int) h->first);
	printf("Adress : %p\tData : %d\t Val : h->last\n", &h->last, (int)h->last);
	printf("Adress : %p\tData : %d\t Val : h->libre\n", &h->libre, (int)h->libre);
	printf("Adress : %p\tData : %d\t Val : h->nbLibre\n", &h->nbLibre, (int)h->nbLibre);
	printf("Adress : %p\tData : %d\t Val : h->nbElem\n", &h->nbElem, (int)h->nbElem);
	printf("Adress : %p\tData : %d\t Val : h->size\n\n", &h->size, (int)h->size);
	if(num==1||num==2){
	if(h->nbLibre!=0){
		printf("Parcours Libre : \n");
		entete_tranche *l = (entete_tranche *)((align_data *)h+h->libre);
		ptrdiff_t nextL = l->suivant;
		ptrdiff_t blocL=h->libre;
		do{
			printf("Adress : %p\tData : %p\t Val : l\tBloc : %d\n", &l, l, (int)blocL);
			printf("Adress : %p\tData : %d\t Val : l->suivant\n", &l->suivant, (int)l->suivant);
			printf("Adress : %p\tData : %d\t Val : l->nb_blocs\n\n", &l->nb_blocs, (int)l->nb_blocs);
			blocL+=l->suivant;
			l=(entete_tranche *)((align_data *)l+l->suivant);
			nextL=l->suivant;
		}while (nextL!=0);
		if(h->nbLibre>1){
			printf("Adress : %p\tData : %p\t Val : l\tBloc : %d\n", &l, l, (int)blocL);
			printf("Adress : %p\tData : %d\t Val : l->suivant\n", &l->suivant, (int)l->suivant);
			printf("Adress : %p\tData : %d\t Val : l->nb_blocs\n\n", &l->nb_blocs, (int)l->nb_blocs);
		}
	}
	}
	if(num==1||num==3){
	if(h->nbElem!=0){
	printf("Parcours Node : \n");
		node *n = (node*)((align_data*)h+h->first);
		ptrdiff_t nextN = n->next;
		ptrdiff_t blocN=h->first;
		do{
			printf("\nAdress : %p\tData : %p\t Val : n\tBloc : %d\n", &n, n, (int)blocN);
			printf("Adress : %p\tData : %d\t Val : n->next\n", &n->next, (int)n->next);
			printf("Adress : %p\tData : %d\t Val : n->prev\n", &n->previous, (int)n->previous);
			printf("Adress : %p\tData : %d\t Val : n->len\n", &n->len, (int)n->len);
			for(int i=0; i<n->len-nb_blocs(sizeof(node));i++){
				printf("Adress : %p\tData : %d\t Val : align_data\n", ((align_data *)n+i+nb_blocs(sizeof(node))), (int)((align_data *)n+i+nb_blocs(sizeof(node)))->a);
			}
			blocN+=n->next;
			n=(node*)((align_data*)n+n->next);
			nextN=n->next;
		}while(nextN!=0);
		if(h->nbElem>1){
			printf("\nAdress : %p\tData : %p\t Val : n\tBloc : %d\n", &n, n,(int) blocN);
			printf("Adress : %p\tData : %d\t Val : n->next\n", &n->next, (int)n->next);
			printf("Adress : %p\tData : %d\t Val : n->prev\n", &n->previous, (int)n->previous);
			printf("Adress : %p\tData : %d\t Val : n->len\n", &n->len, (int)n->len);
			for(int i=0; i<n->len-nb_blocs(sizeof(node));i++){
				printf("Adress : %p\tData : %d\t Val : align_data\n", ((align_data *)n+i+nb_blocs(sizeof(node))), (int)((align_data *)n+i+nb_blocs(sizeof(node)))->a);
			}
		}
	}
	}
}
void printInfo(head* liste, void * item, int num){
	printf("\n\n___________printInfo______________\n\n");
	if(num==1){
		entete_tranche *l = item;
		printf("Adress : %p\tData : %p\t Val : l\tBloc : %d\n", &l, l,(int)( (align_data *)item-(align_data *)liste));
		printf("Adress : %p\tData : %d\t Val : l->suivant\n", &l->suivant, (int)l->suivant);
		printf("Adress : %p\tData : %d\t Val : l->nb_blocs\n\n", &l->nb_blocs, (int)l->nb_blocs);
	}else{
		node *n=item;
		printf("\nAdress : %p\tData : %p\t Val : n\tBloc : %d\n", &n, n,(int)( (align_data *)item-(align_data *)liste));
		printf("Adress : %p\tData : %d\t Val : n->next\n", &n->next, (int)n->next);
		printf("Adress : %p\tData : %d\t Val : n->prev\n", &n->previous, (int)n->previous);
		printf("Adress : %p\tData : %d\t Val : n->len\n", &n->len, (int)n->len);
	}
}


void test(){
	head *h=ld_creer(400);
	//align_data *al=malloc(3*sizeof(align_data));
	align_data al[3];
	al->a=12;
	(al+1)->a=17;
	(al+2)->a=35;
	ld_insert_last(h, 3, al);
	ld_insert_last(h, 3, al);
	h=ld_add_memory(h,160);
	node *n1 = ld_insert_last(h, 5, al);
	ld_insert_last(h, 3, al);
	ld_insert_after(h, n1, 4, al);
	ld_insert_before(h, n1, 4, al);
	ld_delete_node(h, n1);
	h=ld_compactify(h);
	debugg(h,1);
}


void testlg(){
  long t[] = {6, 9, -5, 123, -78, 56, -45, 1, -11, 56, 9, 6, 9};
  size_t n = sizeof(t)/sizeof(t[0]);
  liste l = creer_liste();
  if(l == NULL)
    exit(1);
  for(int i=0; i<n; i++){
    inserer_apres(l, &t[i]);
  }
  imprimer_long(l);
  liste q = l;
  for(int i=0; i<4; i++){
    q=suivant(q);
  }
  liste m = q->suivant;
  supprimer_element(q);
  supprimer_element(m->suivant);
  imprimer_long(l);
  detruire_liste(l);
}

void randTest(int op, int k){
	int oper[op];
	int pos[op];
	intmax_t val[op];
	align_data data[op];
	for(int i=0;i<op;i++){
		oper[i]=rand();
		pos[i]=rand();
		val[i]=rand();
		(data+i)->a=val[i];
	}

	size_t memSize=((op*nb_blocs(sizeof(node)))+(op*nb_blocs(sizeof(align_data))+nb_blocs(sizeof(head))))*sizeof(align_data);
	head* h = ld_creer(memSize);

	struct timeval stop, start;
	//mingw_gettimeofday(&start, NULL);
	gettimeofday(&start, NULL);
	for(int i=0;i<op;i++){
		if(oper[i]%k!=0){
			if(h->nbElem!=0){
				ld_insert_before(h, ld_getIndex(h, pos[i]%h->nbElem), 3, data+i);
			}
			else{
				ld_insert_first(h, 3, data+i);
			}
		}else{
			ld_delete_node(h, ld_getIndex(h, pos[i]%h->nbElem));
		}
	}
	//mingw_gettimeofday(&stop, NULL);
	gettimeofday(&stop, NULL);
	printf("ld_list took %lu us for %d operations\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec, op);
	//debugg(h,1);
	//free(h);

	liste l = creer_liste();
	if(l == NULL){
		exit(1);
	}
	size_t lg_size = lg_getSize(l);
	liste tmp;
	struct timeval stop2, start2;
	//mingw_gettimeofday(&start2, NULL);
	gettimeofday(&start2, NULL);
	for(int i=0;i<op;i++){
		if(oper[i]%k!=0){
			if(lg_size!=0){
				inserer_avant(lg_getIndex(l, pos[i]%lg_size), data+i);
				lg_size++;
			}else{
				inserer_apres(l, data+i);
				lg_size++;
			}
		}else{
			tmp=supprimer_element(lg_getIndex(l, pos[i]%lg_size));
			if(tmp!=NULL){
				lg_size--;
			}
		}
	}
	//mingw_gettimeofday(&stop2, NULL);
	gettimeofday(&stop2, NULL);
	printf("lg_list took %lu us for %d operations\n", (stop2.tv_sec - start2.tv_sec) * 1000000 + stop2.tv_usec - start2.tv_usec, op);
	//imprimer_long(l);
	//detruire_liste(l);
	printf("FIN\n");
}
