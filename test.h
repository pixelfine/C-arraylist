/*
 * test.h
 *
 *  Created on: 16 nov. 2019
 *      Author: Clinu
 */

#ifndef TEST_H_
#define TEST_H_

#include "projet2019.h"

//1 pour tout print, 2 pour print les tranches libre, 3 pour les nodes
void debugg(head *h, int num);
void printInfo(head* liste, void * item, int num);
void test();
void testlg();
void randTest(int op, int k);

#endif /* TEST_H_ */
