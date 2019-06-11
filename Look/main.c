#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#include <dos.h>
#include <dir.h>
#define TamCilindro 40
#define tamRequisicoes 8
char cilindro[TamCilindro];
int requisicoes[tamRequisicoes]={1,9,12,16,34,36,5,20};

int posiRequisicao = 0;
bool direcao = true; //up == true, down == false

int requisicoesFeitas = 0;


int menorRequisicao, maiorRequisicao;


int posicaoCilindro = 10;
void SetColor(int ForgC)
 {
 WORD wColor;

  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                 //Mask out all but the background attribute, and add in the forgournd color
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
 }
 return;
}



void printCilindro(){

    for(int i=0; i<TamCilindro; i++){
        printf(" %i ", i);
    }
    printf("\n");
    for(int i=0; i<TamCilindro; i++){

            if(cilindro[i]=='x'){
                    if(i == posicaoCilindro){
                        SetColor(4);
                    }else{
                        SetColor(2);
                    }

                if(i >= 10){
                    printf("[ X]");
                }else{
                    printf("[X]");
                }

            }else{

                if(i == posicaoCilindro){
                        SetColor(4);
                    }else{
                        SetColor(8);
                    }
                if(i >= 10){
                    printf("[  ]");
                }else{
                    printf("[ ]");
                }

            }
    }
    printf("\n\n");
}
void requisitaPosicao(int id){
    cilindro[id] = 'x';
    requisicoesFeitas++;

    requisicoes[posiRequisicao]=-1; //requisicao atendida
    //printCilindro();

}


void pegaUltimaRequisicao(){



    int maximum = requisicoes[0];
    int minimum = requisicoes[0];
  for(int c=1; c<tamRequisicoes;c++){

    if (requisicoes[c] > maximum)
    {
       maximum  = requisicoes[c];

    }

    if (requisicoes[c] < minimum)
    {
       minimum  = requisicoes[c];

    }
  }


  maiorRequisicao = maximum;
  menorRequisicao = minimum;

}

void mudaDirecao(int id){//verifica se muda e muda

    if(maiorRequisicao == id  || menorRequisicao == id){
        direcao = !direcao;

        posiRequisicao = 0;

    }
}


void verificaRequisicao(int id){

    bool contem = false;

    for(int i = 0; i<tamRequisicoes || contem; i++){
        if(requisicoes[i] == id){
            contem = true;
            posiRequisicao = i;
            break;
        }
    }

    if(contem){
        requisitaPosicao(id);
    }

}

void LOOK(){
    pegaUltimaRequisicao();
    while(tamRequisicoes > requisicoesFeitas){

        verificaRequisicao(posicaoCilindro);
        printCilindro();
        Sleep(500);
                mudaDirecao(posicaoCilindro);


        if(direcao){ //Direita
            posicaoCilindro++;

        }else{
            posicaoCilindro--;
        }
        //printf("\n POSICAO : %i", posicaoCilindro); //verifica se muda de direcao

    }

    printf(" \n\n\t==== FIM DAS REQUISICOES ===");


}

void voltaInicioCilindro(int id){//verifica se volta e volta

    if(maiorRequisicao == id){
        posicaoCilindro = menorRequisicao-1;

        posiRequisicao = 0;

    }
}
void C_LOOK(){

    pegaUltimaRequisicao();
    //printCilindro();

    while(tamRequisicoes > requisicoesFeitas){

        verificaRequisicao(posicaoCilindro);
        printCilindro();
        Sleep(500);
        voltaInicioCilindro(posicaoCilindro);

        if(direcao){ //Direita
            posicaoCilindro++;

        }else{
            posicaoCilindro--;
        }

        //printf("\n POSICAO : %i", posicaoCilindro); //verifica se muda de direcao

    }

    printf(" \n\n\t==== FIM DAS REQUISICOES ===");


}
int main()
{
    //LOOK();
    C_LOOK();
    return 0;
}
