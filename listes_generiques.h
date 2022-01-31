/*
 * Sourced from Diderot University
 */

#ifndef LISTES_GENERIQUES_H_
#define LISTES_GENERIQUES_H_
/* Liste doublement chainees circulaire avec tete.
 * (La tete - premier element de la liste,
 * la tete ne contient pas de donnees.
 *
 * Chaque noeud possède un pointeur
 * vers les donnees.
 */
struct elem{
  struct elem *suivant;    /* pointeur vers le suivant */
  struct elem *precedent;  /* pointeur vers le precedent */
  void *val;               /* pointeur vers les donnees */
};

typedef struct elem elem;
typedef elem *liste;


liste creer_liste();
int est_vide(liste l);
liste inserer_apres(liste e, void *d);
liste inserer_avant(liste e, void *d);
int est_tete(liste l);
void *supprimer_element(liste l);
liste vider_liste(liste l);
void detruire_liste(liste l);
void *valeur(liste l);
liste suivant(liste l);
liste precedent(liste l);
void imprimer_long(liste l);

#endif /* LISTES_GENERIQUES_H_ */
