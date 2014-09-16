#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <pthread.h>
#include <fstream>
using namespace std;

string _commando;
string _rutaMKDIR;
string _PWD;

void *execCommand( void *comando ){
	_commando += " | tee arch.txt";
	system( _commando.c_str() );
	_commando = "";
}

void *mkDIR( void *comando ){
    _commando = "mkdir " + _rutaMKDIR;
    system( _commando.c_str() );
    _commando = "";
}

void *rmDIR( void *comando ){
    _commando = "rm -R " + _rutaMKDIR;
    system( _commando.c_str() );
    _commando = "";
}

void *rmEXEC( void *comando ){
    _commando = "rm " + _rutaMKDIR;
    system( _commando.c_str() );
    _commando = "";
}

void *cdDIR( void *comando ){
    _commando = "cd " + _rutaMKDIR;
    system( _commando.c_str() );
    _commando = "";
}

void *catEXEC( void *comando ){
    _rutaMKDIR = "cat " + _commando;
    system( _rutaMKDIR.c_str() );
    _rutaMKDIR = "";
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

bool buscar( string ruta, string comando ){
    DIR *dir;
    struct dirent *ent;
    bool ret = false;
    if ((dir = opendir(ruta.c_str())) != NULL){
        while( (ent = readdir(dir)) != NULL ){
            if( strcmp(ent->d_name,comando.c_str()) == 0 )
                ret = true;
        }
        closedir(dir);
    }
    return ret;
}

int buscarCommand( string _line, vector<string>& tokens ){
    string _tmp;
    int cont = 0;
    for( auto it = tokens.begin(); it != tokens.end(); it++ ){
        _tmp = *it;
        if( buscar( _tmp, _line ) )
            return cont;
        cont++;
    }
    return -1;
}

string sePuedeCompletar(string line, vector<string>& tokens){
    int cont = 0;
    string val = "";
    for( auto it = tokens.begin(); it != tokens.end(); it++ ){
        val = *it;
        cont = 0;
        for( int i = 0; i < val.length(); i++ ){
            if( cont < line.length() && line.at(cont) == val.at(i) ){
                cont++;
            }
        }
        if( cont > 0 && (cont/val.length()-1)*100 >= 50 )
            return val;
    }
    return "";
}

string leerArchivo(string archivo){
    string ret = "";
    ifstream arch (archivo);
    string cadena = "";

    if (!arch.is_open())
        exit(0);
    else{
        while( getline (arch,cadena) ){
            ret += cadena + "\n";
            cadena = "";
        }
    }
    arch.close();
    return ret;
}

int main()
{
    char tecla;
    string _line = "";
    string _tmp = "";
    string _path = getenv("PATH");
    _PWD = getenv("PWD");
    vector<string> seglist;
    vector<string> _args;
    Tokenize( _path, seglist, ":" );
    int _opc = 0;
    string _ruta;
    pthread_t _hilo;

    string _get = "Mi_sh>";
    vector<string> _TComandos;
    _TComandos.push_back("gedit");_TComandos.push_back("ls");
    _TComandos.push_back("ps");_TComandos.push_back("cat");
    _TComandos.push_back("mkdirS01");_TComandos.push_back("rmdirS01");
    _TComandos.push_back("cdS01");_TComandos.push_back("rmS01");
    _TComandos.push_back("whichS01");_TComandos.push_back("catS01");
    initscr ();
    //cbreak ();

    printw (_get.c_str());
    tecla = getch ();
    do{
        erase();
        _line += tecla;
        _get += tecla;
        printw (_get.c_str());
        tecla = getch ();
        if( tecla == 9 ){
            _tmp = sePuedeCompletar(_line, _TComandos);
            if( strcmp( _tmp.c_str(), "") != 0 ){
                erase();
                _line = _tmp;
                _get = "Mi_sh>" + _tmp;
                printw (_get.c_str());
                tecla = getch ();
            }
        }else if( tecla == 10 ){
            _args.clear();
            Tokenize( _line, _args, " " );
            if( strcmp(_args[0].c_str(),"exit") != 0 ){
                if( strcmp(_args[0].c_str(),"mkdirS01") == 0 ){
                    if( _args.size() > 1 ){
                        _rutaMKDIR = _PWD + "/"+ _args[1];
                        pthread_create( &_hilo, NULL, mkDIR, (void *) &_opc);
                        pthread_join( _hilo, NULL);
                        clear();
                        _line = "";
                        _get = "Mi_sh>";
                        printw (_get.c_str());
                        tecla = getch ();
                    }else{
                        clear();
                        _get = "Falta de argumentos!\nMi_sh>";
                        printw(_get.c_str());
                        tecla = getch ();
                        _line = "";
                    }
                }else if( strcmp(_args[0].c_str(),"rmdirS01") == 0 ){
                    if( _args.size() > 1 ){
                        _rutaMKDIR = _PWD + "/"+ _args[1];
                        pthread_create( &_hilo, NULL, rmDIR, (void *) &_opc);
                        pthread_join( _hilo, NULL);
                        clear();
                        _line = "";
                        _get = "Mi_sh>";
                        printw (_get.c_str());
                        tecla = getch ();
                    }else{
                        clear();
                        _get = "Falta de argumentos!\nMi_sh>";
                        printw(_get.c_str());
                        tecla = getch ();
                        _line = "";
                    }
                }else if( strcmp(_args[0].c_str(),"rmS01") == 0 ){
                    if( _args.size() > 1 ){
                        _rutaMKDIR = _args[1];
                        pthread_create( &_hilo, NULL, rmEXEC, (void *) &_opc);
                        pthread_join( _hilo, NULL);
                        clear();
                        _line = "";
                        _get = "Mi_sh>";
                        printw (_get.c_str());
                        tecla = getch ();
                    }else{
                        clear();
                        _get = "Falta de argumentos!\nMi_sh>";
                        printw(_get.c_str());
                        tecla = getch ();
                        _line = "";
                    }
                }else if( strcmp(_args[0].c_str(),"whichS01") == 0 ){
                    if( _args.size() > 1 ){
                        _opc = buscarCommand( _args[1], seglist );
                        if( _opc != -1 ){
                            clear();
                            _line = "";
                            _get = seglist[_opc] + "\nMi_sh>";
                            printw (_get.c_str());
                            tecla = getch ();
                        }else{
                            clear();
                            _get = "Comando no encontrado!\nMi_sh>";
                            printw(_get.c_str());
                            tecla = getch ();
                            _line = "";
                        }
                    }else{
                        clear();
                        _get = "Falta de argumentos!\nMi_sh>";
                        printw(_get.c_str());
                        tecla = getch ();
                        _line = "";
                    }
                }else if( strcmp(_args[0].c_str(),"catS01") == 0 ){
                    if( _args.size() > 1 ){
                        if( _args.size() == 2 ){
                            _tmp = leerArchivo( _args[1] );
                            clear();
                            _line = "";
                            _get = _tmp + "\nMi_sh>";
                            printw (_get.c_str());
                            tecla = getch ();
                        }else{
                            _commando = "";
                            for( int i = 1; i < _args.size(); i++ )
                                _commando += _args[i] + " ";
                            pthread_create( &_hilo, NULL, catEXEC, (void *) &_opc);
                            pthread_join( _hilo, NULL);
                            clear();
                            _line = "";
                            _get = "Mi_sh>";
                            printw (_get.c_str());
                            tecla = getch ();
                        }
                    }else{
                        clear();
                        _get = "Falta de argumentos!\nMi_sh>";
                        printw(_get.c_str());
                        tecla = getch ();
                        _line = "";
                    }
                }else{
                    _opc = buscarCommand( _args[0], seglist );
                    if( _opc != -1 ){
                        _tmp = "Mi_sh>" + _line + "\n";
                        erase();
                        _commando = _line;
                        pthread_create( &_hilo, NULL, execCommand, (void *) &_opc);
    					pthread_join( _hilo, NULL);
                        clear();
                        _ruta = leerArchivo("arch.txt");
                        _tmp += _ruta;
                        //printw(_tmp.c_str());
                        _line = "";
                        _get = _tmp + "\nMi_sh>";
                        printw (_get.c_str());
                        tecla = getch ();
                    }else{
                        clear();
                        _get = "Comando no encontrado!\nMi_sh>";
                        printw(_get.c_str());
                        tecla = getch ();
                        _line = "";
                    }
                }
            }
        }else if( tecla == 127 ){
            if( _line.length() > 0 ){
                _tmp = "";
                for( int i = 0; i < _line.length()-2; i++ )
                    _tmp += _line.at(i);
                erase();
                _line = _tmp;
                _get = "Mi_sh>" + _tmp;
                printw (_get.c_str());
                tecla = getch ();
            }
        }
    }while( strcmp(_line.c_str(), "exit") != 0 );
    endwin ();
    
    return 0;
}