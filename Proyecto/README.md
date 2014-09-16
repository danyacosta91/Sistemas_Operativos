SHELL LINUX

DESCRIPCIÓN
Proyecto de la clase: Sistemas Operativos I
Shell de linux, creada en NCurses, y con autocompletación de ciertos comandos.

INSTALACIÓN NCURSES
sudo apt-get install libncurses5-dev

COMPILACIÓN
g++ -std=c++0x Proyecto.cpp -lpthread -lncurses -o Proyecto
					o
g++ -std=c++11 Proyecto.cpp -lpthread -lncurses -o Proyecto