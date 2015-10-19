#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <sstream>


using namespace std;


void carregaRegistradores(ifstream *Reg, int8_t *ra,int8_t *rb,int8_t *rx,int8_t *pc);
void carregaMemoria(ifstream *memoria, int8_t *dados);
void imprimeMemoria(int8_t *dados);
void imprimeRegistradores(int8_t ra, int8_t rb, int8_t rx, int8_t pc, uint8_t rem, int8_t rdm, int8_t ri);
int8_t barramentoALU(int8_t x, int8_t y, char ALU1, char ALU2, char ALU3);
int8_t mux12(char S1, char S2, int8_t ra, int8_t rb, int8_t rx);
int8_t mux34(char S3, char S4, int8_t pc, int8_t rxd, int8_t rdm);
int8_t mux7(int8_t alux, int8_t pc, char S7);
int8_t rxd(int8_t rdm, int8_t rx);
void manipulaMemoria(uint8_t rem, int8_t *rdm, char w, char r, int8_t *memoria);
int ativo(char s);
int8_t mux56(char S5, char S6, int8_t rdm, uint8_t rem, int8_t rxd);

int main(int argc, char const *argv[]){
    if(argc <4){
        cout << "Erro na chamada. use ./ramses registradores.txt memoria.txt instrucoes.txt";
        return -1;
    }else{
    	ifstream Registradores(argv[1]);
    	ifstream Memoria(argv[2]);
    	int8_t memoriaDados[256];
    	ofstream saida("saida.txt");
    	string::size_type sz;
    	ifstream Instrucoes(argv[3]);
    	int8_t rA,rB,rX,PC;
    	int8_t ri=0;
    	int8_t rdm=0;
    	int count=0, op=0,t;
    	uint8_t rem=1;
    	string str;
    	queue<string> inst;
    	carregaRegistradores(&Registradores,&rA,&rB,&rX,&PC);
    	printf("\n\n");
    	carregaMemoria(&Memoria,memoriaDados);
		do{
			printf("\n1 - Direto\n2 - Passo a passo\nEscolha sua opcao de execucao: ");
			scanf("%d",&op);
		}while(op!=1 && op!=2);
		switch(op){
			
			case 1: while(Instrucoes.good()){
						Instrucoes >> str;
						if((ativo(str[0])+ativo(str[1])+ativo(str[2])+ativo(str[14])+ativo(str[17])+ativo(str[18])+ativo(str[21]))>1){
							cout << "Instrução inválida, favor verificar linha " << count <<endl;
						}else if(ativo(str[0])){	// bit de cagra RA
							rA = barramentoALU(mux12(str[3],str[4],rA,rB,rX),rdm,str[5],str[6],str[7]);
						}else if(ativo(str[1])){	// bit de cagra RB
							rB = barramentoALU(mux12(str[3],str[4],rA,rB,rX),rdm,str[5],str[6],str[7]);
						}else if(ativo(str[2])){	// bit de cagra RX
							rX = barramentoALU(mux12(str[3],str[4],rA,rB,rX),rdm,str[5],str[6],str[7]);
						}else if(ativo(str[14])){ 	// bit de cagra PC
							PC = mux56(str[11],str[12],rdm,rem,rxd(rdm,rX));
						}else if(ativo(str[17])){ // bit de cagra RI
							ri = rdm;
						}else if(ativo(str[18])){ // bit de cagra REM
							rem = mux34(str[15],str[16], PC, rxd(rdm,rX),rdm);
						}else if(ativo(str[21])){	///bit de carga de RDM
							rdm = mux7(mux12(str[3],str[4],rA,rB,rX),PC,str[22]);
						}else if(ativo(str[19]) || ativo(str[20])){
							manipulaMemoria(rem,&rdm,str[20],str[19],memoriaDados);
						}
						if(ativo(str[13])){
							PC++;
						}
					}
					system("clear");
					imprimeRegistradores(rA,rB,rX,PC,rem,rdm,ri);
					imprimeMemoria(memoriaDados);
				break;
			case 2: while(Instrucoes.good()){
						Instrucoes >> str;
						if((ativo(str[0])+ativo(str[1])+ativo(str[2])+ativo(str[14])+ativo(str[17])+ativo(str[18])+ativo(str[21]))>1){
							cout << "Instrução inválida, favor verificar linha " << count <<endl;
						}else if(ativo(str[0])){	// bit de cagra RA
							rA = barramentoALU(mux12(str[3],str[4],rA,rB,rX),rdm,str[5],str[6],str[7]);
						}else if(ativo(str[1])){	// bit de cagra RB
							rB = barramentoALU(mux12(str[3],str[4],rA,rB,rX),rdm,str[5],str[6],str[7]);
						}else if(ativo(str[2])){	// bit de cagra RX
							rX = barramentoALU(mux12(str[3],str[4],rA,rB,rX),rdm,str[5],str[6],str[7]);
						}else if(ativo(str[14])){ 	// bit de cagra PC
							PC = mux56(str[11],str[12],rdm,rem,rxd(rdm,rX));
						}else if(ativo(str[17])){ // bit de cagra RI
							ri = rdm;
						}else if(ativo(str[18])){ // bit de cagra REM
							rem = mux34(str[15],str[16], PC, rxd(rdm,rX),rdm);
						}else if(ativo(str[21])){	///bit de carga de RDM
							rdm = mux7(mux12(str[3],str[4],rA,rB,rX),PC,str[22]);
						}else if(ativo(str[19]) || ativo(str[20])){
							manipulaMemoria(rem,&rdm,str[20],str[19],memoriaDados);
						}
						if(ativo(str[13])){
							PC++;
						}
						system("clear");
						imprimeRegistradores(rA,rB,rX,PC,rem,rdm,ri);
						imprimeMemoria(memoriaDados);
						printf("\n\nDigite qualquer tecla para continuar...\n\n");
						scanf("%d",&t);
						
					}
				break;
			}
    	
    }
	return 0;
}
int ativo(char s){
	if(s=='1')
		return 1;
	else
		return 0;
}

void manipulaMemoria(uint8_t rem, int8_t *rdm, char w, char r, int8_t *memoria){
	if(ativo(w)){
		memoria[rem] = *rdm;
	}else if(ativo(r)){
		*rdm = memoria[rem];
	}
}

int8_t mux34(char S3, char S4, int8_t pc, int8_t rxd, int8_t rdm){
	if(!ativo(S4) && !ativo(S3)){
		return pc;
	}else if(!ativo(S4) && ativo(S3)){
		return rxd;
	}else if(ativo(S4) && !ativo(S3)){
		return rdm;
	}else{
		return 0;
	}
}


int8_t mux7(int8_t alux, int8_t pc, char S7){
	if(!ativo(S7))
		return alux;
	else
		return pc;
}

int8_t rxd(int8_t rdm, int8_t rx){
	return rdm+rx;
}

int8_t mux12(char S1, char S2, int8_t ra, int8_t rb, int8_t rx){
	if(!ativo(S2) && !ativo(S1)){
		return ra;
	}else if(!ativo(S2) && ativo(S1)){
		return rb;
	}else if(ativo(S2) && !ativo(S1)){
		return rx;
	}else{
		return 0;
	}
}

int8_t mux56(char S5, char S6, int8_t rdm, uint8_t rem, int8_t rxd){
	if(!ativo(S6) && !ativo(S5)){
		return rdm;
	}else if(!ativo(S6) && ativo(S5)){
		return rxd;
	}else if(ativo(S6) && !ativo(S5)){
		return rem;
	}else{
		return 0;
	}
}	

int8_t barramentoALU(int8_t x, int8_t y, char ALU1, char ALU2, char ALU3){
	if(ativo(ALU1) && ativo(ALU3) && ativo(ALU3)){
		return y;
	}else if(!(ativo(ALU1)) && !(ativo(ALU3)) && !(ativo(ALU3))){
			return x+y;
		}else if(!(ativo(ALU1)) && !(ativo(ALU3)) && (ativo(ALU3))){
			return x-y;
		}else if(!(ativo(ALU1)) && (ativo(ALU3)) && !(ativo(ALU3))){
			return x&y;
		}else if(!(ativo(ALU1)) && (ativo(ALU3)) && (ativo(ALU3))){
			return x|y;
		}else if((ativo(ALU1)) && !(ativo(ALU3)) && !(ativo(ALU3))){
			return ~x;
		}else if((ativo(ALU1)) && !(ativo(ALU3)) && (ativo(ALU3))){
			return x-2*x;
		}else if((ativo(ALU1)) && (ativo(ALU3)) && !(ativo(ALU3))){
			return x >> 1;
		}else{
			return 0;
		}
}

void imprimeRegistradores(int8_t ra, int8_t rb, int8_t rx, int8_t pc, uint8_t rem, int8_t rdm,int8_t ri){
	printf("RA: %4d\nRB: %4d\nRX: %4d\nPC: %4d\nREM: %4d\nRDM:%4d\nRI: %4d\n\n",ra,rb,rx,pc,rem,rdm,ri);
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
