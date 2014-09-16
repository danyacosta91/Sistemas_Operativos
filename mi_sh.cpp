#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

int main(){
	//if( fork() == 0 ){
		string lst = "",command = "", args = "";
		do{
			cout << "mi_sh>";
			getline( cin, lst );
			command = lst.substr(0, lst.find(" "));
			args = lst.substr(lst.find(" "), strlen(lst.c_str())-1);
			cout << "ARGS"<< args << endl;
			if( strcmp(command.c_str(),"ps") == 0 ){
				if( strlen(args.c_str()) > 1 ){
					if( strcmp(command.c_str()," -help") == 0 )
						cout << "     AYUDA!\nCOMANDO    Descripcion\n-A      -  Select all processes"<<
						"\n-N   -  Negate selection\n-a  -  select all with a tty except session leaders\n-d    -  select all, but omit session leaders" <<
						"\n-N   -  Negate selection\n-a  -  select all with a tty except session leaders\n-d    -  select all, but omit session leaders" << endl;
					else if( fork() == 0 )
						execl("/bin/ps","/bin/ps",NULL);
						//execl( system("which "+command.c_str()), system("which "+command.c_str()), NULL);
				}else{
					if( fork() == 0 )
						execl("/bin/ps","/bin/ps",NULL);
				}
			}else if( strcmp(command.c_str(),"gedit&") == 0 ){
				if( fork() == 0 )
					execl("/usr/bin/gedit","/usr/bin/gedit","&",NULL);
			}else if( strcmp(command.c_str(),"gedit") == 0 ){
				if( fork() == 0 )
					execl("/usr/bin/gedit","/usr/bin/gedit",NULL);
			}else if( strcmp(command.c_str(),"exit") == 0 ){
			}else if( strcmp(command.c_str(),"-help") == 0 ){
				cout << "     AYUDA!\nCOMANDO    Descripcion\n-help   -  Ayuda\nps      -  Process Status\ngedit   -  Text editor\ngedit&  -  Text editor\nexit    -  exit mi_sh" << endl;
			}else
				cout << "COMMANDO NO ENCONTRADO!" << endl;
		}while( strcmp(command.c_str(),"exit") != 0 );
	//}
	return 0;
}