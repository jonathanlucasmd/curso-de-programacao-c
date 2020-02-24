#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <time.h>
#include "myconio.h"

const char USUARIO[] = "monge";
const char SENHA[] = "123456";

typedef struct ataques{
    char nome[10];
    char tipo[5];
    int danobasico;
    int powerpoints;
    char efeito[10];
}ataque;

typedef struct pokemons{
    char nome[20];
    int codp;
    char tipop[2][10];
    char especie[20];
    char classe[20];
    int pontosdevida;
    ataque *atk;
    //struct pokemons *evolucao;
    char evolucao[50];
}pokemon;

typedef struct itens{
    char nome[20];
    char descricao[200];
}item;

typedef struct treinadores{
    char nome[15];
    pokemon meuspokemons[166];
    int qntdpokemons[166];
    item meusitens[50];
}treinador;


pokemon* matrizdepokemons[26][35] = {0};

void printPokemon(pokemon* poke){
    if(poke == NULL) return;
    printf("Espécie: %s\nCódigo: %d\nEvolução: %s\nTipoA: %s\nTipoB: %s\nAtk: %d\nPV: %d\n",
        poke->especie,
        poke->codp,
        poke->evolucao,
        poke->tipop[0],
        poke->tipop[1],
        poke->atk,
        poke->pontosdevida
    );
}

void carregarDados(){
    pokemon *novopokemon = NULL;

    FILE* arquivo = fopen("pokemons.csv", "r");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo de dados\n");
        exit(EXIT_FAILURE);
    }

    while(!feof(arquivo)){
        char linha[100]={0};
        if(!fgets(linha, 100, arquivo)) break;
        if(!strcmp(linha,"\n")){
            fprintf(stderr, "Erro ao ler linha do arquivo de dados\n");
            fclose(arquivo);
            exit(EXIT_FAILURE);
        }

        novopokemon = (pokemon*)calloc(1,sizeof(pokemon));

        char* especie = strchr(linha,';');
        char* cod = strchr(especie+1,';');
        char* tipoa = strchr(cod+1,';');
        char* tipob = strchr(tipoa+1,';');
        char* evolucao = strchr(tipob+1,'\n');
        int tamespecie = especie-linha;
        int tamcod = cod- ++especie;
        int tamtipoa = tipoa- ++cod;
        int tamtipob = tipob- ++tipoa;
        int tamevolucao = evolucao- ++tipob;

        char codtemp[] = {0};
        strncpy(novopokemon->especie, linha, tamespecie);
        strncpy(codtemp, especie, tamcod);
        novopokemon->codp = atoi(codtemp);
        strncpy(novopokemon->tipop[0], cod, tamtipoa);
        strncpy(novopokemon->tipop[1], tipoa, tamtipob);
        strncpy(novopokemon->evolucao, tipob, tamevolucao);

        int i=0;
        for (i = 0; i < 35 && matrizdepokemons[novopokemon->especie[0]-65][i] != NULL ; i++);
        if(i<35){
            matrizdepokemons[novopokemon->especie[0]-65][i] = novopokemon;
        }else{
            fprintf(stderr, "Erro ao gravar dados, vetor cheio\n");
            fclose(arquivo);
            exit(EXIT_FAILURE);
        }
    }
    fclose(arquivo);
}

void preencher(){
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            pokemon *novo = (pokemon *) malloc(sizeof(pokemon));
            matrizdepokemons[i][j] = novo;
            matrizdepokemons[i][j]->codp = (i+1)*(j+1);
        }
    }
}

void listarPokemons(){
    for (int i = 0; i < 26; i++)
    {
        int j=0;
        while (matrizdepokemons[i][j] != NULL){
            printf("Codigo: %d \n", matrizdepokemons[i][j]->codp);
            j++;
        }
    }
}

int login(){
    clrscr();
    char ch = ' ';
    char login[15] = {0};
    char senha[10] = {0};
    int indicedelinha = 8;
    int tamanhodasenha = 0;
    printf("Login: ");
    scanf("%s", &login); getch();
    printf("Senha: ");
    while (ch != '\n'){
        ch = getch();
        if(ch == 127){
            if(indicedelinha > 8){
                indicedelinha--;
                gotoxy(indicedelinha,2);
                putchar(' ');
                gotoxy(indicedelinha,2);
            }
            if(tamanhodasenha<10){
                senha[tamanhodasenha-1]='\000';
            }
            if(tamanhodasenha>0){
                tamanhodasenha--;
            }
        }else if(ch != '\n'){
            putchar('*');
            indicedelinha++;
            if(tamanhodasenha<10){
                senha[tamanhodasenha] = ch;
            }
            tamanhodasenha++;
        }
    }
    putchar('\n');
    if(strcmp(login,USUARIO) || strncmp(senha,SENHA, strlen(senha))){
        printf("Usuario ou senha inválidos\nPressione qualquer tecla\n");
        getch();
        return 0;
    }else{
        return 1;
    }   
}

pokemon* buscarPokemon(char *nomepokemon){
    if(nomepokemon[0] >= 97 && nomepokemon[0] < 123){
        nomepokemon[0] = (char)(nomepokemon[0])-32;
    }
    if(nomepokemon[0] < 65 && nomepokemon[0] >= 91){
        printf("Não encontrado\n");
        return NULL;
    }
    for (int i = 0; i < 10; i++){
        if(matrizdepokemons[(int)nomepokemon[0]-65][i] != NULL &&
            !strcmp(nomepokemon, matrizdepokemons[(int)nomepokemon[0]-65][i]->especie)){
            return matrizdepokemons[(int)nomepokemon[0]-65][i];
        }        
    }
    printf("Não encontrado\n");
    printf("Pressione qualquer tecla para voltar");
    getch();
    return NULL;
}

void pokedex(){
    clrscr();
    char opc = ' ';
    char sair ='f';
    char temp[15] = {0};
    while (sair == 'f')
    {
        clrscr();
        printf("Pokedex\n");
        printf("1 - Listar pokemons\n");
        printf("2 - Buscar pokemon\n");
        printf("3 - Ver meus pokemons\n");
        printf("4 - Meus Itens\n");
        printf("5 - Trocar pokemon\n");
        printf("6 - Sair\n");
        opc = getchar(); getch();
        switch (opc){
            case '1':
                listarPokemons();
                printf("Pressione qualquer tecla para voltar ");
                getch();
                break;
            case '2':
                printf("Digite a especie: ");
                scanf("%s", &temp); getch();
                printPokemon(buscarPokemon(temp));
                getch();
                break;
            case '6':
                sair = 't';
                break;
            default:
                break;
        }
    }
}

void menu(){
    clrscr();
    char sair = 'f';
    char opc;
    //preencher();
    while (sair == 'f')
    {
        clrscr();
        printf("Menu\n");
        printf("1 - Pokedex\n");
        printf("2 - Ver treinadores\n");
        printf("3 - Batalhar\n");
        printf("4 - Sair\n");
        printf("5 - Encerrar\n");
        opc = getchar(); getch();
        switch (opc){
            case '1':
                pokedex();
                break;
            case '4':
                printf("Saindo....\n");
                printf("Pressione qualquer tecla\n");
                getch();
                sair = 't';
                break;
            case '5':
                printf("Até a próxima...\nPressione qualquer tecla\n");
                getch();
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char const *argv[]){   
    carregarDados(); 
    while (1){
        if(login()){
            menu();
        }
    }
    return 0;
}
