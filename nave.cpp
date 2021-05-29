#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
using namespace std;

//Definiendo los numeros de las teclas direccionales
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80


void gotoxy(int x, int y){ //funcion para poder mover el cursor
	HANDLE hCon; //Identificador de la ventana
	hCon = GetStdHandle(STD_OUTPUT_HANDLE); //Recuperar el identificador de la consola. Se dan parametros segun el tipo de control
							//que queremos tener en la ventana
	
	COORD dwPos; //Dando coordenadas en la pantalla
	dwPos.X = x;
	dwPos.Y = y;
	
	SetConsoleCursorPosition(hCon, dwPos); //Permite dar una posicion al cursos y mandar a imprimir a esas coordenadas
								//Tiene dos parametros, un identificador llamado HANDLE
	
}

void OcultarCursor(){
	HANDLE hCon; //Identificador de la ventana
	hCon = GetStdHandle(STD_OUTPUT_HANDLE); //Recuperar el identificador de la consola. Se dan parametros segun el tipo de control que queremos tener en la ventana
	
	CONSOLE_CURSOR_INFO cci; //Para controlar la apariencia del cursor mostrado en pantalla
	cci.dwSize = 2; //dependiendo del valor que se le asigna, es el tamaño del cursor 
	cci.bVisible = FALSE; //si es verdadero querra decir que el cursor se muestra en pantalla, si es falso no se muestra
	
	SetConsoleCursorInfo(hCon, &cci); //para controlar las caracteristicas del cursor mostrado en pantalla
}

void pintar_limites(){
	for(int i = 2; i < 78; i++){ //lineas horizontales
		gotoxy(i, 3); printf("%c", 205);
		gotoxy(i, 33); printf("%c", 205);
	}
	
	for(int i = 4; i < 33; i++){ //lineas verticales
		gotoxy(2, i); printf("%c", 186);
		gotoxy(77, i); printf("%c", 186);
	}
	
	//pintando las esquinas
	gotoxy(2, 3); printf("%c", 201);
	gotoxy(2, 33); printf("%c", 200);
	gotoxy(77, 3); printf("%c", 187);
	gotoxy(77, 33); printf("%c", 188);
}

class NAVE{
	int x, y; //coordenadas para la nave
	int corazones;
	int vidas;
	
public:
	NAVE(int _x, int _y, int _corazones, int _vidas): x(_x), y(_y), corazones(_corazones), vidas(_vidas){} //metodo constructor
	
	int X(){
		return x;
	}
	
	int Y(){
		return y;
	}
	
	void COR() { 
		corazones--;
	}
	
	int VID() {
		return vidas;
	}
	
	void pintar();
	void borrar();
	void mover();
	void pintar_corazones();
	void morir();
};

void NAVE::pintar(){
	gotoxy(x, y); 	 printf("  %c", 30);
	gotoxy(x, y+1);; printf(" %c%c%c", 40, 207, 41);
	gotoxy(x, y+2);  printf("%c%c %c%c", 30, 190, 190, 30);
	
}

void NAVE::borrar(){
	gotoxy(x, y); 	printf("        ");
	gotoxy(x, y+1); printf("        ");
	gotoxy(x, y+2); printf("        ");

}

void NAVE::mover(){
		if(kbhit()){	//kbhit() es una funcion que detecta si se ha presionado una tecla
			char tecla = getch();	//getch() guarda la letra que se presione
			
			borrar();
			
			if(tecla == IZQUIERDA && x > 3) x--;
			if(tecla == DERECHA && x + 6 < 77) x++;
			if(tecla == ARRIBA && y > 4) y--;   //las coordenadas Y estan invertidas, entonces para ir hacia arriba es --
			if(tecla == ABAJO && y + 3 < 33) y++;	//se le suma para que suba ++
			
			if(tecla == 'e') corazones--;
			pintar();
			pintar_corazones();
		}
}

void NAVE::pintar_corazones(){
	gotoxy(50, 2); printf("VIDAS %d", vidas);
	gotoxy(64, 2); printf("Salud");
	gotoxy(70, 2); printf("       ");
	for(int i = 0; i < corazones; i++){
		
		gotoxy(70 + i, 2); printf("%c", 3);
	}
}

void NAVE::morir(){
	if(corazones == 0){
		borrar();
		gotoxy(x, y); 	  printf("   **   "); //pintando explosion de la nave
		gotoxy(x, y + 1); printf("  ****  ");
		gotoxy(x, y + 2); printf("   **   ");
		Sleep(300);
		
		borrar();
		gotoxy(x, y); 	  printf(" * ** *"); //pintando explosion de la nave
		gotoxy(x, y + 1); printf("  **** ");
		gotoxy(x, y + 2); printf(" * ** *");
		Sleep(300);
		
		borrar();
		
		vidas--;
		corazones = 3;
		pintar_corazones();
		pintar();

	}
}

class AST{
	int x, y;
public:
	AST(int _x, int _y): x(_x), y(_y){}
	void pintar();
	void mover();
	void choque(NAVE &N);
	int X(){
		return x;
	}
	
	int Y(){
		return y;
	}
};

void AST::pintar(){
	gotoxy(x, y); printf("%c", 190);
}

void AST::mover(){ //generando posiciones al azar para los asteroides 
	gotoxy(x, y); printf(" ");
	y++;
	if(y > 32){
		x = rand() % 71 + 4;
		y = 4;
	}
	pintar();
}

void AST::choque(NAVE &N){
	if(x >= N.X() && x < N.X() + 6 && y >= N.Y() && y <= N.Y() + 2){
		
		N.COR();
		N.borrar();
		N.pintar();
		N.pintar_corazones();
		x = rand() % 71 + 4;
		y = 4;
	}
}

class BALA{
	int x, y;
public:
	BALA(int _x, int _y): x(_x), y(_y) {}
	
	int X(){
		return x;
	}
	
	int Y() {
		return y;
	}
	
	void mover();
	bool fuera();
};

void BALA::mover(){
	gotoxy(x, y); printf(" ");
	y--; 
	gotoxy(x, y); printf("*");
}

bool BALA::fuera(){
	if(y == 4) return true;
	return false;
}

int main() {
	
	OcultarCursor();
	pintar_limites();
	NAVE N(37,30, 3, 3);
	N.pintar();
	N.pintar_corazones();
	
	list<AST*> A;
	list<AST*>::iterator itA;
	for(int i = 0; i < 5; i++){
		A.push_back(new AST(rand() % 75 + 3, rand() % 5 + 4));
	}
	
	
	list<BALA*> B;
	list<BALA*>::iterator it;
	
	bool game_over = false;
	int puntos = 0;
	while(!game_over){
		
		gotoxy(4, 2); printf("Puntos %d", puntos);
		
		if(kbhit()){
			
			char tecla = getch();
			if(tecla == 's')
			B.push_back(new BALA(N.X() + 2, N.Y() - 1));
		}
		
		for(it = B.begin(); it != B.end(); it++) {
			
			(*it)->mover();
			if((*it) -> fuera()){
				gotoxy((*it) -> X(), (*it) -> Y()); printf(" ");
				delete(*it);
				it = B.erase(it);
			}
		}
		
		for(itA = A.begin(); itA != A.end(); itA++){
			(*itA) -> mover();
			(*itA) -> choque(N);
	
		}
		
		for(itA = A.begin(); itA != A.end(); itA++){
			for(it = B.begin(); it != B.end(); it++){
				if((*itA) -> X() == (*it) -> X() && ( (*itA) -> Y() + 1 == (*it) -> Y() || (*itA) -> Y() + 1 == (*it) -> Y() ))
				{
					gotoxy((*it) -> X(), (*it) -> Y()); printf(" ");
					delete(*it);
					it = B.erase(it);
					
					A.push_back(new AST(rand() % 74 + 3, 4));
					gotoxy((*itA) -> X(), (*itA) -> Y()); printf(" ");
					delete(*itA);
					itA = A.erase(itA);
					
					puntos++;
				}
			}
	    }
		
		if(N.VID() == 0) game_over = true, printf("GAME OVER"); 
		N.morir();
		N.mover();
		Sleep(30); //Sleep() Detiene el bucle por 30 milisegundos 
	}
	return 0;
}


