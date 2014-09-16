#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <sstream>
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

void run( string comando ){
	string ruta1 = "/usr/bin", ruta2 = "/bin";
	bool bandera = false, b1 = false, b2 = false;
	DIR *dir;
	struct dirent *ent;
	if ( buscar( ruta1,comando.c_str() ) ){
		bandera = true;
		b1 = true;
	}else if( buscar( ruta2,comando.c_str() ) ){
		bandera = true;
		b2 = true;
	}else if( strcmp(comando.c_str(),"exit") == 0 ){
		bandera = true;
	}

	string full1 = ruta1+"/"+comando;
	string full2 = ruta2+"/"+comando;
	if( bandera ){
		if( b1 ){
			if( fork() == 0 )
				execl( full1.c_str(),full1.c_str(),NULL );
		}
		else if( b2 ){
			if( fork() == 0 )
				execl( full2.c_str(),full2.c_str(),NULL );
		}
	}else
		cout << "COMANDO NO ENCONTRADO!" << endl;
}

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters){
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos){
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

int main(){
	//if( fork() == 0 ){
	vector<string> seglist;
	char splitChar = ' ';
		string lst = "", command = "", segment;
		do{
			cout << "mi_sh>";
			getline( cin, lst );
			istringstream split(lst);
			for( string each; getline( split, each, splitChar); seglist.push_back( each ) );
			command = seglist[0];
			run( lst );
		}while( strcmp(command.c_str(),"exit") != 0 );
	//}
	return 0;
}