#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
using namespace std;

bool buscar( string ruta, string comando ){
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(ruta.c_str())) != NULL){
		while( (ent = readdir(dir)) != NULL ){
			if( strcmp(ent->d_name,comando.c_str()) == 0 ){
				return true;
			}
		}
		closedir(dir);
	}else
		return false;
}

int main(){
	string ruta1 = "/usr/bin", ruta2 = "/bin";
	bool bandera = false, b1 = false, b2 = false;
	DIR *dir;
	struct dirent *ent;
	if ( buscar( ruta1,"ps" ) ){
		bandera = true;
		b1 = true;
	}else if( buscar( ruta2,"ps" ) ){
		bandera = true;
		b2 = true;
	}

	if( bandera ){
		if( b1 )
			cout << ruta1 << endl;
		else if( b2 )
			cout << ruta2 << endl;
	}else
		cout << "COMANO NO ENCONTRADO!" << endl;
	return 0;
}