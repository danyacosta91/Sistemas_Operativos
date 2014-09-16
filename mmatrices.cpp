#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
using namespace std;

const unsigned _tam1 = 1000;
const unsigned _tam2 = 2000;
const unsigned _tam3 = 3000;
int** _m1;
int** _m2;
int** _m3;

void *multiplicar(void *);

main()
{
	srand(time(NULL));
	_m1 = new int* [_tam1];
	_m2 = new int* [_tam2];
	_m3 = new int* [_tam1];
	pthread_t hilos[_tam1];

	for (int i = 0; i < _tam1; ++i)
		_m1[i] = new int[_tam2];
	for (int i = 0; i < _tam2; ++i)
		_m2[i] = new int[_tam3];
	for (int i = 0; i < _tam1; ++i)
		_m3[i] = new int[_tam3];
	for (int i = 0; i < _tam1; ++i){
		for (int j = 0; j < _tam2; ++j)
			_m1[i][j] = rand()%1000;
	}
	for (int i = 0; i < _tam2; ++i){
		for (int j = 0; j < _tam3; ++j)
			_m2[i][j] = rand()%1000;
	}
	for (int i = 0; i < _tam1; ++i){
		for (int j = 0; j < _tam3; ++j)
			_m3[i][j] = 0;
	}
	
	for (int i = 0; i < _tam1; i++){
		pthread_create( &hilos[i], NULL, multiplicar, (void *) &i);
		pthread_join( hilos[i], NULL);
	}
	
	exit(0);
}

void *multiplicar(void *renglon) {
	int valor = 0;
	for (int i = 0; i < _tam2; ++i){
		for (int j = 0; j < _tam2; ++j){
			for (int k = 0; k < _tam2; ++k)
				valor += (_m1[(int)renglon][j] * _m2[j][(int)renglon]);
		}
		_m3[(int)renglon][i] = valor;
		valor = 0;
	}
}
