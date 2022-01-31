
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "projet2019.h"

static size_t nb_blocs(size_t o){
	size_t count=1;
	size_t tmp = sizeof(align_data);
	while(o>tmp){
		count++;
		tmp+=sizeof(align_data);
	}
	return count;
}

static void* createNodeFirst(head *h, node *n, ptrdiff_t next, ptrdiff_t prev, size_t len, align_data *data){
	if(h->nbElem==0){
		n->next=0;
		n->previous=0;
		h->first=(align_data*)n-(align_data*)h;
		h->last=h->first;
	}else{
		n->next=next;
		n->previous=prev;
		node *nextNode = (node*)((align_data *)n+n->next);
		nextNode->previous=(align_data*)n-((align_data*)h+h->first);
		h->first=(align_data*)n-(align_data*)h;
	}
	n->len=len;
	size_t size = len-nb_blocs(sizeof(node));
	for(size_t i=0;i<size;i++){
		*((align_data *)n+i+nb_blocs(sizeof(node)))=data[i] ;
	}
	h->nbElem++;
	return n;
}

static void* createNodeLast(head *h, node *n, ptrdiff_t next, ptrdiff_t prev, size_t len, align_data *data){
	if(h->nbElem==0){
		n->next=0;
		n->previous=0;
		h->first=(align_data*)n-(align_data*)h;
		h->last=h->first;
	}else{
		n->next=next;
		n->previous=prev;
		node *prevNode = (node*)((align_data *)n+n->previous);
		prevNode->next=(align_data*)n-((align_data*)h+h->last);
		h->last=(align_data*)n-(align_data*)h;
	}
	n->len=len;
	size_t size = len-nb_blocs(sizeof(node));
	for(size_t i=0;i<size;i++){
		*((align_data *)n+i+nb_blocs(sizeof(node)))=data[i] ;
	}
	h->nbElem++;
	return n;
}
static void * nodeExists(head *h, node* n){
	node *currN = (node*)((align_data*)h+h->first);
	for(ptrdiff_t nodeLeft=h->nbElem; nodeLeft>=0; nodeLeft--){
		if(currN==n){
			return currN;
		}currN=(node*)((align_data*)currN+currN->next);
	}return NULL;
}
static void* createNodeIn(head *h, node *prevN, node *currN, node *nextN, size_t len, align_data *data){
	if(prevN==nextN){
		node* firstN=(node*)((align_data*)h+h->first);
		node* lastN=(node*)((align_data*)h+h->last);
		if(prevN==firstN){
			return createNodeFirst(h, currN, ((align_data*)h+h->first)-(align_data*)currN, 0, len, data);
		}else if(prevN==lastN){
			return createNodeLast(h, currN, 0, ((align_data*)h+h->last)-(align_data*)currN, len, data);
		}
	}
	prevN->next=(align_data*)currN-(align_data*)prevN;
	nextN->previous=(align_data*)currN-(align_data*)nextN;
	currN->previous=(align_data*)prevN-(align_data*)currN;
	currN->next=(align_data*)nextN-(align_data*)currN;
	currN->len=len;
	size_t size = len-nb_blocs(sizeof(node));
	for(size_t i=0;i<size;i++){
		*((align_data *)currN+i+nb_blocs(sizeof(node)))=data[i] ;
	}
	h->nbElem++;
	return currN;
}

void * ld_creer( size_t nboctets ){
	size_t maxBloc = nb_blocs(nboctets)-1;
	if(nboctets%sizeof(align_data)==0){
		maxBloc = nb_blocs(nboctets);
	}
	size_t minBloc = nb_blocs(sizeof(head));
	size_t libreBloc = nb_blocs(sizeof(entete_tranche));
	if(libreBloc==0){
		libreBloc=1;
	}
    if(maxBloc>=minBloc){
    	size_t memSize = maxBloc*sizeof(align_data);
		//Allocation de memoire
		head* ptr=malloc(memSize);
		if(ptr == NULL){
		 perror("Error : invalid malloc"); exit(1);
		}
		ptr->memory=&(ptr->memory);
		ptr->first=0;
		ptr->last=0;
		ptr->libre=minBloc;
		ptr->nbLibre=1;
		ptr->nbElem=0;
		ptr->size=memSize;
		entete_tranche *t = ((entete_tranche*)((char*)ptr+(minBloc*sizeof(align_data))));
		t->suivant=0;
		t->nb_blocs=maxBloc-minBloc;
		return ptr->memory;
    }else {
    	perror("Error : unsatisfying nboctets"); exit(1);
    }
}

void *ld_first(void *liste){
	head *h = liste;
	if(h->nbElem==0){
		return NULL;
	}else return (void *)(node*)((align_data*)h->memory+(h->first));
}

void *ld_last(void *liste){
	head *h = liste;
	if(h->nbElem==0){
		return NULL;
	}else return (void *)((node*)((align_data*)h->memory+h->last));
}

void *ld_next(void *liste, void *current){
	if(liste==current){
		return ld_first(liste);
	}else{
		node *currN=current;
		return (void *)((node*)((align_data*)currN+currN->next));
	}
}

void *ld_previous(void *liste, void *current){
	if(liste==current){
		return ld_first(liste);
	}else{
		node *currN = current;
		return (void *)((node*)((align_data*)currN+currN->previous));
	}
}

void ld_destroy(void *liste){
	free(liste);
}


size_t ld_get(void *liste, void *current, size_t len, void *val){
	align_data* v = val;
	node *curr= current;
	size_t size = curr->len-nb_blocs(sizeof(node));
	size = (nb_blocs(len)>size)? size:nb_blocs(len);
	for(size_t i =0;i<size;i++){
		*((align_data*)v+i)=*((align_data*)curr->data+i);
	}
	return size*sizeof(align_data);
}



void * ld_insert_first(void *liste, size_t len, void *p_data){
	head *h = liste;
	if(h->nbLibre>0 && len>=nb_blocs(sizeof(node))){
		entete_tranche *prevL=NULL;
		entete_tranche *currL = (entete_tranche *)((align_data*)h+h->libre);
		entete_tranche *nextL = (entete_tranche *)((align_data*)currL+currL->suivant);
		node *pos;
		for(ptrdiff_t libreLeft=h->nbLibre; libreLeft>=0; libreLeft--){
			if(currL->nb_blocs>=len){
				pos=(node *)currL;
				if(currL->nb_blocs>len){
					if(libreLeft==h->nbLibre){h->libre+=len;}
					if(currL!=nextL){currL->suivant-=len;}
					if(prevL!=currL && prevL!=NULL){prevL->suivant+=len;}
					currL=memmove((entete_tranche *)((align_data *)currL+len), currL, sizeof(entete_tranche));
				}else{
					if(libreLeft==h->nbLibre){h->libre+=currL->suivant;}
					else{prevL->suivant+=currL->suivant;}
					h->nbLibre--;
				}
				currL->nb_blocs-=len;
				node *n =  createNodeFirst(h, pos, ((align_data*)h+h->first)-(align_data*)pos, 0, len, p_data);
				h->first=(align_data*)n-(align_data*)h;
				return n;
				break;
			}
			prevL=currL;
			currL=(entete_tranche *)((align_data*)currL+currL->suivant);
			nextL=(entete_tranche *)((align_data*)nextL+nextL->suivant);
		}
	}return NULL;
}

void * ld_insert_last(void *liste, size_t len, void *p_data){
	head *h = liste;
	if(h->nbLibre>0 && len>=nb_blocs(sizeof(node))){
		entete_tranche *prevL=NULL;
		entete_tranche *currL = (entete_tranche *)((align_data*)h+h->libre);
		entete_tranche *nextL = (entete_tranche *)((align_data*)currL+currL->suivant);
		node *pos;
		for(ptrdiff_t libreLeft=h->nbLibre; libreLeft>=0; libreLeft--){
			if(currL->nb_blocs>=len){
				pos=(node *)currL;
				if(currL->nb_blocs>len){
					if(libreLeft==h->nbLibre){h->libre+=len;}
					if(currL!=nextL){currL->suivant-=len;}
					if(prevL!=currL && prevL!=NULL){prevL->suivant+=len;}
					currL=memmove((entete_tranche *)((align_data *)currL+len), currL, sizeof(entete_tranche));
				}else{
					if(libreLeft==h->nbLibre){h->libre+=currL->suivant;}
					else{prevL->suivant+=currL->suivant;}
					h->nbLibre--;
				}
				currL->nb_blocs-=len;
				node *n =  createNodeLast(h, pos, 0, ((align_data*)h+h->last)-(align_data*)pos, len, p_data);
				h->last=(align_data*)n-(align_data*)h;
				return n;
				break;
			}
			prevL=currL;
			currL=(entete_tranche *)((align_data*)currL+currL->suivant);
			nextL=(entete_tranche *)((align_data*)nextL+nextL->suivant);
		}
	}return NULL;
}

void * ld_insert_before(void *liste, void *n, size_t len, void *p_data){
	head *h = liste;
	node *nextN=nodeExists((head*)liste, (node*) n);
	if(h->nbLibre>0 && nextN!=NULL && len>=nb_blocs(sizeof(node))){
		entete_tranche *prevL=NULL;
		entete_tranche *currL = (entete_tranche *)((align_data*)h+h->libre);
		entete_tranche *nextL = (entete_tranche *)((align_data*)currL+currL->suivant);
		node *pos;
		for(ptrdiff_t libreLeft=h->nbLibre; libreLeft>=0; libreLeft--){
			if(currL->nb_blocs>=len){
				pos=(node *)currL;
				if(currL->nb_blocs>len){
					if(libreLeft==h->nbLibre){h->libre+=len;}
					if(currL!=nextL){currL->suivant-=len;}
					if(prevL!=currL && prevL!=NULL){prevL->suivant+=len;}
					currL=memmove((entete_tranche *)((align_data *)currL+len), currL, sizeof(entete_tranche));
				}else{
					if(libreLeft==h->nbLibre){h->libre+=currL->suivant;}
					else{prevL->suivant+=currL->suivant;}
					h->nbLibre--;
				}
				currL->nb_blocs-=len;
				node *prevN=(node*)((align_data*)nextN+nextN->previous);
				node *currN=pos;
				currN =  createNodeIn(h, prevN, currN, nextN, len, p_data);
				return currN;
				break;
			}
			prevL=currL;
			currL=(entete_tranche *)((align_data*)currL+currL->suivant);
			nextL=(entete_tranche *)((align_data*)nextL+nextL->suivant);
		}
	}return NULL;
}

void * ld_insert_after(void *liste, void *n, size_t len, void *p_data){
	head *h = liste;
	node *prevN=nodeExists((head*)liste, (node*) n);
	if(h->nbLibre>0 && prevN!=NULL && len>=nb_blocs(sizeof(node))){
		entete_tranche *prevL=NULL;
		entete_tranche *currL = (entete_tranche *)((align_data*)h+h->libre);
		entete_tranche *nextL = (entete_tranche *)((align_data*)currL+currL->suivant);
		node *pos;
		for(ptrdiff_t libreLeft=h->nbLibre; libreLeft>=0; libreLeft--){
			if(currL->nb_blocs>=len){
				pos=(node *)currL;
				if(currL->nb_blocs>len){
					if(libreLeft==h->nbLibre){h->libre+=len;}
					if(currL!=nextL){currL->suivant-=len;}
					if(prevL!=currL && prevL!=NULL){prevL->suivant+=len;}
					currL=memmove((entete_tranche *)((align_data *)currL+len), currL, sizeof(entete_tranche));
				}else{
					if(libreLeft==h->nbLibre){h->libre+=currL->suivant;}
					else{prevL->suivant+=currL->suivant;}
					h->nbLibre--;
				}
				currL->nb_blocs-=len;
				node *currN=pos;
				node *nextN=(node*)((align_data*)prevN+prevN->next);
				currN =  createNodeIn(h, prevN, currN, nextN, len, p_data);
				return currN;
				break;
			}
			prevL=currL;
			currL=(entete_tranche *)((align_data*)currL+currL->suivant);
			nextL=(entete_tranche *)((align_data*)nextL+nextL->suivant);
		}
	}return NULL;
}

static void delete(head *h, node* n){
	node *prevN=(node *)((align_data *)n+n->previous);
	node *nextN=(node *)((align_data *)n+n->next);
	if(n!=nextN){
		prevN->next+=n->next;
	}else{prevN->next=0;}
	if(n!=prevN){
		nextN->previous+=n->previous;
	}else{
		nextN->previous=0;
	}
	size_t nbBloc = n->len;
	ptrdiff_t firstL = h->libre;
	ptrdiff_t dist = (align_data *)n- (align_data *)h;
	h->libre=dist;
	if(h->nbElem==1){
		h->first=0;
		h->last=0;
	}
	entete_tranche *l=(entete_tranche *)n;
	l->suivant=firstL-dist;
	l->nb_blocs=nbBloc;
	h->nbElem--;
	h->nbLibre++;
}


void * ld_delete_node(void *liste, void *n){
	head *h=liste;
	node *ptrN= (node*)n;
	if(h->nbElem!=0){
		int deleted=0;
		node *currN = (node*)((align_data*)h+h->first);
		ptrdiff_t nextN = currN->next;
		ptrdiff_t blocN=h->first;
		do{
			if(ptrN-currN==0){deleted++;break;}
			blocN+=currN->next;
			currN=(node*)((align_data*)currN+currN->next);
			nextN=currN->next;
		}while(nextN!=0);
		if(h->nbElem>1){
			if(ptrN-currN==0){deleted++;}
		}
		if(deleted>0){
			delete(h,currN);
			return liste;
		}else{
			return NULL;
		}
	}else{
		return NULL;
	}
}

size_t ld_total_free_memory(void *liste){
	head *h = liste;
	entete_tranche *l = (entete_tranche*)((align_data*)h+h->libre);
	size_t libreLeft=h->nbLibre;
	size_t espace =0;
	while(libreLeft!=0){
		espace+=l->nb_blocs;
		libreLeft--;
		l=(entete_tranche*)((align_data*)l+l->suivant);
	}
	return espace*sizeof(align_data);
}

size_t ld_total_useful_memory(void *liste){
	head *h = liste;
	entete_tranche *l = (entete_tranche*)((align_data*)h+h->libre);
	size_t libreLeft=h->nbLibre;
	size_t espace =0;
	while(libreLeft!=0){
		if(l->nb_blocs>=sizeof(node)){
			espace+=l->nb_blocs;
		}
		libreLeft--;
		l=(entete_tranche*)((align_data*)l+l->suivant);
	}
	return espace*sizeof(align_data);
}


/*
 * Je réalloue la mémoire + nb_octets.
 * J'actualise les données de head
 * Je créer un nouvel entete_tranche en premier libre de liste au dernier bloc du précédent pointer
 */
void *ld_add_memory(void *liste, size_t nboctets){
	size_t size = nb_blocs(nboctets)*sizeof(align_data);
	size_t oldSize = ((head*)liste)->size;
	ptrdiff_t last = nb_blocs(oldSize);
	head *h = realloc(liste, ((head*)liste)->size+size);
	if(h==NULL){
		return NULL;
	}
	h->size=size+oldSize;
	entete_tranche *oldFirstL = (entete_tranche *)((align_data*)h+h->libre);
	entete_tranche *l = (entete_tranche *)((align_data *)h+last);
	h->libre=last;
	h->nbLibre++;
	l->suivant = (align_data*)oldFirstL-(align_data*)l;
	l->nb_blocs=nb_blocs(nboctets);
	return h;
}

void *ld_compactify(void *liste){
	head *h = liste;
	size_t minBloc = nb_blocs(sizeof(head));
	size_t maxBloc = nb_blocs(h->size);
	head *h2 = malloc(h->size);
	h2->memory=&(h2->memory);
	h2->first=0;
	h2->last=0;
	h2->libre=minBloc;
	h2->nbElem=0;
	h2->nbLibre=1;
	h2->size=h->size;
	entete_tranche *currL=(entete_tranche*)((char*)h2+(minBloc*sizeof(align_data)));
	currL->suivant=0;
	currL->nb_blocs=maxBloc-minBloc;
	node *currN=(node*)((align_data*)h+h->first);
	for(ptrdiff_t nodeLeft = h->nbElem; nodeLeft>0; nodeLeft--){
		ld_insert_first(h2, currN->len, currN->data);
		currN=(node*)((align_data *)currN+currN->next);
	}
	free(liste);
	return h2;
}

