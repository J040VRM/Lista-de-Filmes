#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50


struct Filme {
    char nome[50];
    char genero[50];
    int anoDeLancamento;
    int classificacao;
};

// Função para atribuir valores para as características do filme
struct Filme criarFilme(const char nome[], const char genero[], int anoDeLancamento, int classificacao) {
    struct Filme novoFilme; 

    strcpy(novoFilme.nome, nome);
    strcpy(novoFilme.genero, genero);
    novoFilme.anoDeLancamento = anoDeLancamento;
    novoFilme.classificacao = classificacao;

    return novoFilme;
}

//Função de remoção de um filme da lista
void removerFilme(struct Filme filmes[], int *contadorFilmes) {
    char nomeFilme[50];
    int encontrado = 0;

    printf("Digite o nome do filme que deseja remover: ");
    fgets(nomeFilme, 50, stdin);
    nomeFilme[strcspn(nomeFilme, "\n")] = '\0'; // Determina o comprimento da string e remove o caractere de nova linha

    // Compara as strings de nome dos filmes
    for (int i = 0; i < *contadorFilmes; i++) {
        if (strcmp(filmes[i].nome, nomeFilme) == 0) {
            // Deslocar os filmes para preencher o espaço do filme removido
            for (int j = i; j < *contadorFilmes - 1; j++) {
                filmes[j] = filmes[j + 1];
            }
            (*contadorFilmes)--;
            encontrado = 1;
            printf("O filme \"%s\" foi removido com sucesso.\n", nomeFilme);
            break;
        }
    }

    if (!encontrado) {
        printf("O filme \"%s\" não foi encontrado na lista.\n", nomeFilme);
    }

    // Reescrever todas as informações restantes no arquivo
    FILE *arquivo;
    arquivo = fopen("filmes.txt", "w"); // Abrir o arquivo para escrita (sobrescrever)

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < *contadorFilmes; i++) {
        fprintf(arquivo, "%s;%s;%d;%d\n", filmes[i].nome, filmes[i].genero, filmes[i].anoDeLancamento, filmes[i].classificacao);
    }

    fclose(arquivo);
    //4printf("Informações atualizadas no arquivo.\n");
}


void imprimirDetalhesFilme(struct Filme filme) {
    printf("\nNome do filme: %s\n", filme.nome);
    printf("Pertence ao genero: %s\n", filme.genero);
    printf("lançado em: %d\n", filme.anoDeLancamento);
    printf("Classificacao: +%d\n", filme.classificacao);
    printf("\n---\n");
}

//Menu secundario para determinar um filtro escolhido pelo usuario
void mostrarFilmesFiltrados(struct Filme filmes[], int contadorFilmes) {
  int filtro;
  char valorFiltro[50];

  printf(":: - Escolha o filtro - ::\n");
  printf("1. Gênero\n");
  printf("2. Data de Lançamento\n");
  printf("3. Classificação\n");
  printf("Digite o número do filtro desejado: ");
  scanf("%d", &filtro);
  getchar(); // Limpar o buffer do teclado

  printf("Digite o valor do filtro: ");
  fgets(valorFiltro, 50, stdin);
  valorFiltro[strcspn(valorFiltro, "\n")] = '\0'; // Remover o caractere de nova linha

  switch (filtro) {
      case 1:
          printf("\nFilmes com o gênero \"%s\":\n", valorFiltro);
          for (int i = 0; i < contadorFilmes; i++) {
              if (strcmp(filmes[i].genero, valorFiltro) == 0) {
                  imprimirDetalhesFilme(filmes[i]);
              }
          }
        break;
      case 2:
          printf("\nFilmes lançados em %s:\n", valorFiltro);
          for (int i = 0; i < contadorFilmes; i++) {
              if (filmes[i].anoDeLancamento == atoi(valorFiltro)) { // funcao atoi, converte de string para inteiro
                  printf("%s\n", filmes[i].nome);
              }
          }
          break;
      case 3:
          printf("\nFilmes com classificação %s:\n", valorFiltro);
          for (int i = 0; i < contadorFilmes; i++) {
              if (filmes[i].classificacao == atoi(valorFiltro)) {
                  printf("%s\n", filmes[i].nome);
              }
          }
          break;
      default:
          printf("Opção de filtro inválida.\n");
          break;
  }
}

//Escreve o filme [i] em um arquivo, para que fique salvo na memoria
void escreverFilmeEmArquivo(struct Filme filme) {
    FILE *arquivo;
    arquivo = fopen("filmes.txt", "a"); 

    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    size_t length = strlen(filme.nome);
    if (filme.nome[length - 1] == '\n') {
        filme.nome[length - 1] = '\0';
    }

    size_t length2 = strlen(filme.genero);
    if (filme.genero[length2 - 1] == '\n') {
        filme.genero[length2 - 1] = '\0';
    }
  
    fprintf(arquivo, "%s;%s;%d;%d\n", filme.nome,filme.genero, filme.anoDeLancamento, filme.classificacao);

    fclose(arquivo);

    //printf("Detalhes do filme foram escritos no arquivo.\n");
}

//inicializa a lista de filmes, obtendo as informações do arquivo .txt
void lerFilmesDoArquivo(struct Filme filme[], int *contadorFilmes) {
    FILE *arquivo;
    char linha[100]; 

    arquivo = fopen("filmes.txt", "r"); //abre no modo read

    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

  
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        
        char nome[50], genero[50];
        int anoDeLancamento, classificacao;

        sscanf(linha, "%49[^;];%49[^;];%d;%d", nome, genero, &anoDeLancamento, &classificacao);//separa pelo ; 

        
        struct Filme novoFilme = criarFilme(nome, genero, anoDeLancamento, classificacao);
        filme[*contadorFilmes] = novoFilme;
        (*contadorFilmes)++; 
    }

    
    fclose(arquivo);

    //printf("Filmes do arquivo foram carregados com sucesso.\n");
}

//
int main() {
    int contadorFilmes = 0, escolha;

    struct Filme filme[MAX]; 
    lerFilmesDoArquivo(filme, &contadorFilmes);
    while (1) {
      printf("\n:: - Menu - ::\n");
      printf("(1) Adicionar Filme\n");
      printf("(2) Mostrar Filmes registrados\n");
      printf("(3) Remover Filmes\n");
      printf("(4) Filtrar\n");
      printf("(5) Sair\n");

      printf("\nEscolha uma opção: ");
      scanf("%d", &escolha);

      getchar(); 

      switch (escolha) {
        case 1:
          system("clear");
          printf(" - Você escolheu a opção 1. -\n\n");

          char nome[50], genero[50];
          int anoLancamento, classificacao;

          printf("-> Digite o nome do filme: ");
          fgets(nome, 50, stdin);

          printf("-> Digite a qual genero ele pertence: ");
          fgets(genero, 50, stdin);

          printf("-> Ano de lançamento: ");
          scanf("%d",&anoLancamento);

          printf("-> Classificação indicativa: ");
          scanf("%d",&classificacao);

          getchar(); 

          filme[contadorFilmes] = criarFilme(nome, genero, anoLancamento, classificacao);
          escreverFilmeEmArquivo(filme[contadorFilmes]);
          contadorFilmes++;
          printf("Aperte <enter> para prosseguir");
          scanf("pausa no comando");
          system("clear");
          break;
        case 2:
          system("clear");
          printf("Você escolheu a opção 2.\n");
          for (int i = 0; i < contadorFilmes; i++){
            imprimirDetalhesFilme(filme[i]);
          }   
          printf("Aperte <enter> para prosseguir");
          scanf("pausa no comando");
          system("clear");
          break;
        case 3:
          system("clear");
          printf("Você escolheu a opção 3.\n");
          removerFilme(filme, &contadorFilmes);
          printf("Aperte <enter> para prosseguir");
          scanf("pausa no comando");
          system("clear");
          break;
        case 4:
          system("clear");
          printf("Você escolheu a opção 4.\n");
          mostrarFilmesFiltrados(filme,contadorFilmes);
          printf("Aperte <enter> para prosseguir");
          scanf("pausa no comando");
          system("clear");
          break;
        case 5:
          printf("Encerrando o programa.\n");
          return 0;
        default:
          printf("Opção inválida. Por favor, escolha novamente.\n");
          printf("Aperte <enter> para prosseguir");
          scanf("pausa no comando");
          system("clear");
        break;
    }
  }

    return 0;
}
