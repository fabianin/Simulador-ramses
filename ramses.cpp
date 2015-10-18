#include <iostream>
#include <queue>
#include <stack>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <sstream>
#define ATIVO(A) A=='1'


using namespace std;


void carregaRegistradores(ifstream *Reg, int8_t *ra,int8_t *rb,int8_t *rx,int8_t *pc);
void carregaMemoria(ifstream *memoria, int8_t *dados);
void imprimeMemoria(int8_t *dados);
void imprimeRegistradores(int8_t ra, int8_t rb, int8_t rx, int8_t pc, uint8_t rem, int8_t rdm);
int8_t barramentoALU(int8_t x, int8_t y, char ALU1, char ALU2, char ALU3);
int8_t mux12(char S1, char S2, int8_t ra, int8_t rb, int8_t rx);
int8_t aluX(char S1, char S2, int8_t ra, int8_t rb, int8_t rx);
int8_t mux34(char S3, char S4, int8_t pc, int8_t rxd, int8_t rdm);
int8_t mux7(int8_t alux, int8_t pc, char S7);
int8_t rxd(int8_t rdm, int8_t rx);
void manipulaMemoria(uint8_t rem, int8_t *rdm, char w, char r, int8_t *memoria);

int main(int argc, char const *argv[]){
    if(argc <4){
        cout << "Erro na chamada. use ./ramses registradores.txt memoria.txt instrucoes.txt";
        return -1;
    }else{
    	ifstream Registradores(argv[1]);
    	ifstream Memoria(argv[2]);
    	int8_t memoriaDados[256];
    	ifstream Instrucoes(argv[3]);
    	int8_t rA,rB,rX,PC;
    	int8_t rdm=0;
    	uint8_t rem=0;
    	string str;
    	queue<string> inst;
    	carregaRegistradores(&Registradores,&rA,&rB,&rX,&PC);
    	printf("\n\n");
    	carregaMemoria(&Memoria,memoriaDados);
    	imprimeRegistradores(rA,rB,rX,PC,rem,rdm);
    	imprimeMemoria(memoriaDados);
    	while(Instrucoes.good()){
			Instrucoes >> str;
			inst.push(str);
		}
    	
    }
	return 0;
}

void manipulaMemoria(uint8_t rem, int8_t *rdm, char w, char r, int8_t *memoria){
	if(ATIVO(w)){
		memoria[rem] = *rdm;
	}else if(ATIVO(r)){
		*rdm = memoria[rem];
	}
}

int8_t mux34(char S3, char S4, int8_t pc, int8_t rxd, int rdm){
	if(!ATIVO(S3) && !ATIVO(S4)){
		return pc;
	}else if(!ATIVO(S3) && ATIVO(S4)){
		return rxd;
	}else if(ATIVO(S3) && !ATIVO(S4)){
		return rdm;
	}else{
		return 0;
	}
}


int8_t mux7(int8_t alux, int8_t pc, char S7){
	if(!ATIVO(S7))
		return alux;
	else
		return pc;
}

int8_t rxd(int8_t rdm, int8_t rx){
	return rdm+rx;
}

int8_t mux12(char S1, char S2, int8_t ra, int8_t rb, int8_t rx){
	if(!ATIVO(S1) && !ATIVO(S2)){
		return ra;
	}else if(!ATIVO(S1) && ATIVO(S2)){
		return rb;
	}else if(ATIVO(S1) && !ATIVO(S2)){
		return rx;
	}else{
		return 0;
	}
}

int8_t mux56(char S5, char S6, int8_t rdm, uint8_t rem, int8_t rxd){
	if(!ATIVO(S5) && !ATIVO(S6)){
		return rdm;
	}else if(!ATIVO(S5) && ATIVO(S6)){
		return rxd;
	}else if(ATIVO(S5) && !ATIVO(S6)){
		return rem;
	}else{
		return 0;
	}
}	

int8_t barramentoALU(int8_t x, int8_t y, char ALU1, char ALU2, char ALU3){
	if(ATIVO(ALU1) && ATIVO(ALU3) && ATIVO(ALU3)){
		return y;
	}else if(!(ATIVO(ALU1)) && !(ATIVO(ALU3)) && !(ATIVO(ALU3))){
			return x+y;
		}else if(!(ATIVO(ALU1)) && !(ATIVO(ALU3)) && (ATIVO(ALU3))){
			return x-y;
		}else if(!(ATIVO(ALU1)) && (ATIVO(ALU3)) && !(ATIVO(ALU3))){
			return x&y;
		}else if(!(ATIVO(ALU1)) && (ATIVO(ALU3)) && (ATIVO(ALU3))){
			return x|y;
		}else if((ATIVO(ALU1)) && !(ATIVO(ALU3)) && !(ATIVO(ALU3))){
			return ~x;
		}else if((ATIVO(ALU1)) && !(ATIVO(ALU3)) && (ATIVO(ALU3))){
			return x-2*x;
		}else if((ATIVO(ALU1)) && (ATIVO(ALU3)) && !(ATIVO(ALU3))){
			return x >> 1;
		}else{
			return 0;
		}
}

void imprimeRegistradores(int8_t ra, int8_t rb, int8_t rx, int8_t pc, uint8_t rem, int8_t rdm){
	printf("RA: %d\nRB: %d\nRX: %d\nPC: %d\nREM: %d\nRDM: %d\n\n\n",ra,rb,rx,pc,rem,rdm);
}

void carregaRegistradores(ifstream *Reg, int8_t *ra,int8_t *rb,int8_t *rx,int8_t *pc){
	
	
	int aux;
    *Reg >> aux;
    *ra= (int8_t) aux;
    *Reg >> aux;
    *rb = (int8_t) aux;
    *Reg >> aux;
    *rx = (int8_t) aux;
    *Reg >> aux;
    *pc = (int8_t) aux;
}

void carregaMemoria(ifstream *memoria, int8_t *dados){
	int aux = 0;
	for(int i = 0; i < 256; i++){
		*memoria >> aux;
		dados[i] = (int8_t)aux;
	}
	
}

void imprimeMemoria(int8_t *dados){
	for(int i = 0; i< 256; i++){
		if(i%16 == 0)
			printf("\n\n");
		printf("%4d ",dados[i]);
	}
	printf("\n\n");
}
