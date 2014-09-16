#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
using namespace std;

const unsigned _tam1 = 500;
const unsigned _tam2 = 500;
const unsigned _tam3 = 500;
const unsigned _tamHilos = 16;
long ** _m1;
long** _m2;
long** _m3;
int _n;
int _fib;

void *multiplicar(void *);
void *fibonnacci(void *);

main()
{
	srand(time(NULL));
	_m1 = new long* [_tam1];
	_m2 = new long* [_tam2];
	_m3 = new long* [_tam1];
	pthread_t hilos[_tamHilos];
	pthread_t _fibonacciHilo;

	for (int i = 0; i < _tam1; ++i)
		_m1[i] = new long[_tam2];
	for (int i = 0; i < _tam2; ++i)
		_m2[i] = new long[_tam3];
	for (int i = 0; i < _tam1; ++i)
		_m3[i] = new long[_tam3];
	for (int i = 0; i < _tam1; ++i){
		for (int j = 0; j < _tam2; ++j)
			_m1[i][j] = rand()%3;
	}
	for (int i = 0; i < _tam2; ++i){
		for (int j = 0; j < _tam3; ++j)
			_m2[i][j] = rand()%3;
	}
	for (int i = 0; i < _tam1; ++i){
		for (int j = 0; j < _tam3; ++j)
			_m3[i][j] = 0;
	}
	
	cout << "Ingrese valor de n: " << endl;
	cin >> _n;
	pthread_create( &_fibonacciHilo, NULL, fibonnacci, (void *) _n);


	int cont = 0;
	for (int i = 0; i < _tamHilos; i++){
		pthread_create( &hilos[i], NULL, multiplicar, (void *) cont);		
		cont += 31;
	}
	pthread_join( _fibonacciHilo, NULL);
	for (int i = 0; i < _tamHilos; i++)
		pthread_join( hilos[i], NULL);

	/*
	cout << "A" << endl;
	for (int i = 0; i < _tam1; ++i){
		for (int j = 0; j < _tam2; ++j)
			cout << _m1[i][j] << " - ";
		cout << endl;
	}

	cout << "B" << endl;
	for (int i = 0; i < _tam2; ++i){
		for (int j = 0; j < _tam3; ++j)
			cout << _m2[i][j] << " - ";
		cout << endl;
	}

	*/
	for (int i = 0; i < _tam1; ++i){
		for (int j = 0; j < _tam3; ++j)
			_m3[i][j] = _fib * _m3[i][j];
	}

	cout << "FINAL" << endl;
	for (int i = 0; i < _tam1; ++i){
		for (int j = 0; j < _tam3; ++j)
			cout << _m3[i][j] << " / ";
		cout << endl;
	}
	
	free( _m1 );
	free( _m2 );
	free( _m3 );
	exit(0);
}

void *multiplicar(void *renglon) {
	long valor = 0;
	int cont = 0;
	if( (int)renglon < 465 ){
		for( int k = (int)renglon; k < ((int)renglon+31); k++ ){
			for (int i = 0; i < _tam2; ++i){
				for (int j = 0; j < _tam2; ++j){
					valor += (_m1[k][j] * _m2[j][cont]);
				}
				cont++;
				_m3[k][i] = valor;
				valor = 0;
			}
		}
	}else{
		for( int k = (int)renglon; k < ((int)renglon+35); k++ ){
			for (int i = 0; i < _tam2; ++i){
				for (int j = 0; j < _tam2; ++j){
					valor += (_m1[k][j] * _m2[j][cont]);
				}
				cont++;
				_m3[k][i] = valor;
				valor = 0;
			}
		}
	}
}

void *fibonnacci(void *index) {
	int first = 0, second = 1, next;
	for ( int c = 0 ; c <= (int)index; c++ ){
		if ( c <= 1 )
			next = c;
		else{
			next = first + second;
			first = second;
			second = next;
		}
	}
	_fib = next;
}