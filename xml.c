

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Criando a estrutura do nó da pilha 
typedef struct no_{
    char tag[1000];
    struct no_ *prox;
}No;

//Função adicionar um novo nó para pilha
int adicionar(No **topo, char *tag){
    No *novo_no = (No *)malloc(sizeof(No));
    if(novo_no != NULL){
        //copia a tag para o novo nó 
        strcpy(novo_no->tag, tag);
        novo_no->prox = *topo;
        *topo = novo_no;
        return 1;
    }else{
        printf("Erro ao alocar memória.\n");
        
        exit(EXIT_FAILURE);
    }
}
//Função remover o topo da pilha
int remover(No **topo){
    if(*topo == NULL){
        return 0; //pilha vazia 
    }
    No *temp = *topo;
    *topo = (*topo)->prox;
    free(temp);

    return 1;
}
//Função para mostrar o conteúdo da pilha
void mostrar(No *topo){
    if(topo == NULL){
        printf("Pilha vazia\n");
    }else{
        while(topo != NULL){
            printf("%s\t", topo->tag);
            topo = topo->prox;
        }
        printf("\n");
    }
}

int main(){
    FILE *arquivo;
    char nome_arquivo[50];

    printf("Digite o nome do arquivo XML desejado:");
    scanf("%s", &nome_arquivo);

    arquivo = fopen(nome_arquivo, "r");
    //Verfica se o arquivo foi aberto com sucesso  
    if(arquivo == NULL){
        perror("Erro ao abrir o arquivo\n.");
        return 1; 
    }
    //inicializa a pilha como vazia 
    No *pilha = NULL;
    char linha[1000];
    int numero_linha = 0;

    //Lê cada linha do arquivo
    while(fgets(linha, sizeof(linha),arquivo) != NULL){
        numero_linha++;

        // Encontra o início e o fim de uma tag em cada linha
        char *inicio_tag = strchr(linha, '<');
        char *fim_tag = strchr(linha, '>');

        while(inicio_tag != NULL && fim_tag != NULL){
            //Extraindo a tag 
            int tamanhotag = fim_tag - inicio_tag - 1;
            char tag[1000];
            strncpy(tag, inicio_tag + 1, tamanhotag);
            tag[tamanhotag] = '\0';

            //Verificando se é uma tag de abertura ou fechamento 
            if(tag[0] != '/' ){
                //tag abertura
                adicionar(&pilha, tag);
            }else{
                //Tag de fechamento
                if(pilha == NULL){
                    printf("Foi encontrado um ERRO na linha %d do arquivo XML", numero_linha);
                    fclose(arquivo);
                    break;
                }

                //Comparando com a última tag de abertura 
                if (pilha != NULL && strcmp(pilha->tag, tag + 1) != 0) {
                    printf("Foi encontrado um ERRO na linha %d do arquivo XML", numero_linha);
                    fclose(arquivo);
                    break;
                }

                remover(&pilha);
            }

            //Continuando a busca por outras tags
            inicio_tag = strchr(fim_tag + 1, '<');
            fim_tag = strchr(fim_tag + 1, '>');
        }
    }

     // Verifica se a pilha está vazia
    if(pilha == NULL){
        printf("Arquivo XML encontrado corretamente.\n");
    }else{
        printf("Tag: %s\n", pilha->tag);
        remover(&pilha);
    }

    fclose(arquivo);
    return 0;

}