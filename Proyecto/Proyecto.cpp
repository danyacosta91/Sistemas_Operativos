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
    chdir( _rutaMKDIR.c_str() );
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
    bool _bandera = true;
    for( auto it = tokens.begin(); it != tokens.end(); it++ ){
        val = *it;
        cont = 0;
        for( int i = 0; i < val.length(); i++ ){
            if( cont < line.length() && line.at(cont) == val.at(i) ){
            	if( cont != i ){
            		cont = 0;
            		break;
            	}
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
    _TComandos.push_back("gedit");_TComandos.push_back("ls");_TComandos.push_back("shutdown");
    _TComandos.push_back("ps");_TComandos.push_back("rmS01");_TComandos.push_back("halt");
    _TComandos.push_back("mkdirS01");_TComandos.push_back("rmdirS01");_TComandos.push_back("reboot");
    _TComandos.push_back("whichS01");_TComandos.push_back("catS01");_TComandos.push_back("find");
    _TComandos.push_back("exit");_TComandos.push_back("cdS01");_TComandos.push_back("alias");
    _TComandos.push_back("cat");_TComandos.push_back("grep");_TComandos.push_back("bzip2");
    _TComandos.push_back("kill");_TComandos.push_back("arch");_TComandos.push_back("cal");
    _TComandos.push_back("uname");_TComandos.push_back("date");_TComandos.push_back("chmod");
    _TComandos.push_back("chown");_TComandos.push_back("cmp");_TComandos.push_back("date");
    _TComandos.push_back("du");_TComandos.push_back("whereis");_TComandos.push_back("mount");
    _TComandos.push_back("gzip");_TComandos.push_back("tar");_TComandos.push_back("zip");
    _TComandos.push_back("echo");_TComandos.push_back("dump");_TComandos.push_back("egrep");
    _TComandos.push_back("env");_TComandos.push_back("eject");_TComandos.push_back("dirs");
    _TComandos.push_back("gcc");_TComandos.push_back("g++");_TComandos.push_back("last");
    _TComandos.push_back("man");_TComandos.push_back("pstree");_TComandos.push_back("time");
    _TComandos.push_back("users");_TComandos.push_back("vim");_TComandos.push_back("vi");
    _TComandos.push_back("w");_TComandos.push_back("who");_TComandos.push_back("whoami");
    _TComandos.push_back("xkill");_TComandos.push_back("whatis");_TComandos.push_back("wall");
    initscr ();
    //cbreak ();

    do{
        erase();
        printw (_get.c_str());
        tecla = getch ();
        if( tecla == 9 ){
            _tmp = sePuedeCompletar(_line, _TComandos);
            if( strcmp( _tmp.c_str(), "") != 0 ){
                erase();
                _line = _tmp;
                _get = "Mi_sh>" + _tmp;
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
                        _get = "Mi_sh>";
                    }else{
                        _get = "Falta de argumentos!\nMi_sh>";
                    }
                }else if( strcmp(_args[0].c_str(),"rmdirS01") == 0 ){
                    if( _args.size() > 1 ){
                        _rutaMKDIR = _PWD + "/"+ _args[1];
                        pthread_create( &_hilo, NULL, rmDIR, (void *) &_opc);
                        pthread_join( _hilo, NULL);
                        _get = "Mi_sh>";
                    }else{
                        _get = "Falta de argumentos!\nMi_sh>";
                    }
                }else if( strcmp(_args[0].c_str(),"cdS01") == 0 ){
                    if( _args.size() > 1 ){
                        _rutaMKDIR = _PWD + "/"+ _args[1];
                        pthread_create( &_hilo, NULL, cdDIR, (void *) &_opc);
                        pthread_join( _hilo, NULL);
                        _get = "Mi_sh>";
                    }else{
                        _rutaMKDIR = _PWD;
                        pthread_create( &_hilo, NULL, cdDIR, (void *) &_opc);
                        pthread_join( _hilo, NULL);
                        _get = "Mi_sh>";
                    }
                }else if( strcmp(_args[0].c_str(),"rmS01") == 0 ){
                    if( _args.size() > 1 ){
                        _rutaMKDIR = _args[1];
                        pthread_create( &_hilo, NULL, rmEXEC, (void *) &_opc);
                        pthread_join( _hilo, NULL);
                        _get = "Mi_sh>";
                    }else{
                        _get = "Falta de argumentos!\nMi_sh>";
                    }
                }else if( strcmp(_args[0].c_str(),"whichS01") == 0 ){
                    if( _args.size() > 1 ){
                        _opc = buscarCommand( _args[1], seglist );
                        if( _opc != -1 ){
                            _get = seglist[_opc] + "\nMi_sh>";
                        }else{
                            _get = "Comando no encontrado!\nMi_sh>";
                        }
                    }else{
                        _get = "Falta de argumentos!\nMi_sh>";
                    }
                }else if( strcmp(_args[0].c_str(),"catS01") == 0 ){
                    if( _args.size() > 1 ){
                        if( _args.size() == 2 ){
                            _tmp = leerArchivo( _args[1] );
                            _get = _tmp + "\nMi_sh>";
                        }else{
                            _commando = "";
                            for( int i = 1; i < _args.size(); i++ )
                                _commando += _args[i] + " ";
                            pthread_create( &_hilo, NULL, catEXEC, (void *) &_opc);
                            pthread_join( _hilo, NULL);
                            _get = "Mi_sh>";
                        }
                    }else{
                        _get = "Falta de argumentos!\nMi_sh>";
                    }
                }else{
                    _opc = buscarCommand( _args[0], seglist );
                    if( _opc != -1 ){
                        _tmp = "Mi_sh>" + _line + "\n";
                        _commando = _line;
                        pthread_create( &_hilo, NULL, execCommand, (void *) &_opc);
    					pthread_join( _hilo, NULL);
                        clear();
                        _ruta = leerArchivo("arch.txt");
                        _tmp += _ruta;
                        _get = _tmp + "Mi_sh>";
                    }else{
                        _get = "Comando no encontrado!\nMi_sh>";
                    }
                }
            }
            _line = "";
        }else if( tecla == 127 ){
            if( _line.length() > 0 ){
                _line = _line.substr( 0, _line.length()-1 );
                _get += "\nMi_sh>" + _line;
            }
        }else{
        	_line += tecla;
        	_get += tecla;
        }
    }while( strcmp(_line.c_str(), "exit") != 0 );
    endwin ();
    
    return 0;
}