#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

#define QUANTUM 2  //usado no RR

struct Processo{
    int tempoChegada;
    int tempoPico;
    int tempoRestante; //somente usado no RR
};

vector <Processo> LeArquivo(FILE *arquivo){

    arquivo = fopen("arquivo.txt", "rt"); //rt leitura
    int tempoChegada, tempoPico;

    vector <Processo> p;

    while(!feof(arquivo)){
        fscanf(arquivo,"%d %d", &tempoChegada, &tempoPico); 
        
        if(feof(arquivo)) break;   
        
        Processo aux;
        aux.tempoChegada = tempoChegada;
        aux.tempoPico = tempoPico;
        aux.tempoRestante = tempoPico;
        p.push_back(aux);

    }

    //p.erase(p.end()-1); // apaga ultimo elemento do vector q ta sendo passado duas vezes 

    fclose(arquivo);    

    return p;
}


void FCFS(vector <Processo> listaProcessos){    
    float tempoRetorno = 0, tempoResposta = 0, tempoEspera = 0, tempoExec = 0;
    int tamLista = listaProcessos.size();

    while(listaProcessos.size()!= 0){
        Processo aux = listaProcessos[0];
        listaProcessos.erase(listaProcessos.begin());

        if(aux.tempoChegada > tempoExec){
            tempoExec = aux.tempoChegada;
        }

        tempoRetorno = tempoRetorno + tempoExec + aux.tempoPico - aux.tempoChegada;
        tempoResposta = tempoResposta + tempoExec - aux.tempoChegada;
        tempoEspera = tempoEspera + tempoExec - aux.tempoChegada;    
        
        tempoExec = tempoExec + aux.tempoPico;

    }

    float retornoMedio = (tempoRetorno/tamLista);
    float respostaMedia = (tempoResposta/tamLista);
    float esperaMedia = (tempoEspera/tamLista);

    cout << "FCFS " << retornoMedio << " " << respostaMedia << " " << esperaMedia << endl;    
   
}

bool ComparaPico(Processo x, Processo y){
    return(x.tempoPico <= y.tempoPico);
}

void SJF(vector <Processo> listaProcessos){
    float tempoEspera = 0, tempoRetorno = 0, tempoResposta = 0, tempoExec = 0;
    int tamLista = listaProcessos.size();
    int quantidadeProcessos = tamLista;

    vector <Processo> filaProntos;

    while(quantidadeProcessos!=0){
        for(int i=0 ; i<listaProcessos.size() ; i++){
            if(listaProcessos[i].tempoChegada <= tempoExec){
                filaProntos.push_back(listaProcessos[i]);
                listaProcessos.erase(listaProcessos.begin()+i);
                i--;
            }
        }
        sort(filaProntos.begin(), filaProntos.end(), ComparaPico);

        Processo aux = filaProntos[0];
        filaProntos.erase(filaProntos.begin());

   
        tempoResposta += tempoExec - aux.tempoChegada;
        tempoEspera += tempoExec - aux.tempoChegada;        
        tempoExec += aux.tempoPico;  
        tempoRetorno += tempoExec - aux.tempoChegada;    

        quantidadeProcessos--;
    }

    float retornoMedio = tempoRetorno / tamLista;
    float respostaMedia = tempoResposta / tamLista;
    float esperaMedia = tempoEspera / tamLista;

    cout << "SJF " << retornoMedio << " " << respostaMedia << " " << esperaMedia << endl;

}

void RR(vector <Processo> listaProcessos){
    float tempoEspera = 0, tempoRetorno = 0, tempoResposta = 0, tempoExec = 0;
    int tamLista = listaProcessos.size();
    int quantidadeProcessos = tamLista;
    int i;
    int tamanhoProntos;

    vector <Processo> filaProntos;

    Processo aux;
    aux.tempoChegada = 0; 
    aux.tempoPico = 0;
    aux.tempoRestante = 0;

    while(quantidadeProcessos!=0){
        for (i = 0; i < listaProcessos.size(); i++){
            if(listaProcessos[i].tempoChegada == tempoExec){
                filaProntos.push_back(listaProcessos[i]);
                listaProcessos.erase(listaProcessos.begin()+i);
                i--;
            }
            else if(listaProcessos[i].tempoChegada < tempoExec){
                tempoEspera += tempoExec - listaProcessos[i].tempoChegada;
                filaProntos.push_back(listaProcessos[i]);
                listaProcessos.erase(listaProcessos.begin()+i);
                i--;
            }
        }

        if(aux.tempoRestante>0){ //se o processo ainda nao acabou sua execução
            filaProntos.push_back(aux);
        }

        aux = filaProntos[0];
        filaProntos.erase(filaProntos.begin());

        if(aux.tempoRestante == aux.tempoPico){ //primeira execucao do meu processo
            tempoResposta += tempoExec - aux.tempoChegada;
        }

        tempoExec += QUANTUM;
        aux.tempoRestante -=  QUANTUM;
        tamanhoProntos = filaProntos.size();
        tempoEspera += (QUANTUM * tamanhoProntos);

        if(aux.tempoRestante<=0){
            tempoExec += aux.tempoRestante;
            tempoEspera += (aux.tempoRestante * tamanhoProntos);
            tempoRetorno += tempoExec - aux.tempoChegada; 
            quantidadeProcessos--;
        }


    }

    float retornoMedio = tempoRetorno / tamLista;
    float respostaMedia = tempoResposta / tamLista;
    float esperaMedia = tempoEspera / tamLista;
    
    //cout << tempoExec << endl;
    cout << "RR " << retornoMedio << " " << respostaMedia << " " << esperaMedia << endl;

}

int main(){
    FILE *arquivo;
    vector <Processo> a;
    vector <Processo> b;
    vector <Processo> c;

    a = LeArquivo(arquivo);
    b = LeArquivo(arquivo);
    c = LeArquivo(arquivo);
    
    FCFS(a);
    SJF(b);
    RR(c);
    
    //cout << a.size() << endl;
    return 0;
}