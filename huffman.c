#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define tamanhoOutros 25
#define tamanhoLista 256
#define tamanhoChar 2500

typedef struct No {
    unsigned char conteudo;
    int freq;
    struct No* esq, * dir, * prox; // colocamos que o nó tenha conexão com três nós,
    // o esq e o dir são referentes a árvore e o prox é referente a lista de prioridade
} No;

typedef struct {
    No* inicio;
    int tamanho;
} Lista;

void iniciarLista(Lista* lista) {
    lista->inicio = NULL;
    lista->tamanho = 0;
}

void iniciarTab(unsigned int tab[]) {
    for (int i = 0; i < tamanhoLista; i++)
        tab[i] = 0;
}
void preencherFrequencias(unsigned char conteudo[], unsigned int frequencias[]) {
    int i;
    for (int i = 0; conteudo[i] != '\0'; i++) {
        frequencias[conteudo[i]]++; // contar o tanto de vezes que tal caracter aparece
    }
}

void preencherLista(unsigned int frequencias[], Lista* lista)
{
    No* no;
    for (int i = 0; i < tamanhoLista; i++) {
        if (frequencias[i] > 0) { // se
            no = malloc(sizeof(No));
            if (no) {
                no->conteudo = i;
                no->freq = frequencias[i];
                no->dir = NULL;
                no->esq = NULL;
                no->prox = NULL;

                inserirOrdenado(lista, no);
            }
            else {
                printf("Erro em preencherLista!!!!!!!!!!!\n");
                break;
            }
        }
    }
}

No* removeInicio(Lista* lista) {
    No* aux = NULL;

    if (lista->inicio) {
        aux = lista->inicio; // aux recebe a raiz da lista
        lista->inicio = aux->prox; // raiz muda de raiz, já que o nó foi removido
        aux->prox = NULL; // tira o nó
        lista->tamanho--; // diminui o tamanho da lista
    }
}

No* montarArvore(Lista* lista) {
    No* no1, * no2, * junto;
    while (lista->tamanho > 1) {
        no1 = removeInicio(lista);
        no2 = removeInicio(lista);
        junto = malloc(sizeof(No));

        if (junto) {
            junto->conteudo = ' '; // não possui conteúdo, é só um ligador
            junto->freq = no1->freq + no2->freq; // soma das frequências dos dois nós
            junto->esq = no1;
            junto->dir = no2;
            junto->prox = NULL;

            inserirOrdenado(lista, junto); // vamos colocar o nó da árvore de volta na lista;
        }
        else {
            printf("\nErro ao montar árvore");
            break;
        }
    }
    junto = lista->inicio;
    removeInicio(lista);
    return junto;
}

void inserirOrdenado(Lista* lista, No* no)
{
    No* aux;

    if (lista->inicio == NULL) { // se a lista estiver vazia, adicionamos o nó no início
        lista->inicio = no;
    }

    else if (no->freq <= lista->inicio->freq) { // se a frequência do nó for menor ou igual
        no->prox = lista->inicio; // a frequência do byte no começo da lista
        lista->inicio = no;
    }

    else { // se a frequência do nó for maior que o nó do começo da lista
        aux = lista->inicio;
        while (aux->prox == NULL && aux->prox->freq < no->freq) { // vamos percorrer  lista até que o a frequência do nó
            aux = aux->prox; // seja maior do que a do aux, ou seja, até que a gente ache onde o nó se encaixa na fila de prioridade
        }
        // quando a posição for achada,
        no->prox = aux->prox; // colocaremos o próximo da posição atual como próximo do nó
        aux->prox = no; // e o nó se encaixará como próximo do nó
        // antes:  | aux | prox |   a  | b | c |...
        // depois: | aux |  nó  | prox | a | b |...
    }

    lista->tamanho++; // o tamanho da lista aumenta já que a inserção foi realizada nos três if else
}

unsigned char lista(FILE* arquivo)
{
    fopen(arquivo, "rb");
    unsigned char listaSemPrioridade[tamanhoLista];

    unsigned char umByte[9]; // um byte possui 8 bits, mais um para armazenar o '/0'
    int i = 0;

    if (arquivo) {
        while (fread(&umByte, sizeof(unsigned char), 1, arquivo)) {
            listaSemPrioridade[i] = umByte;
            i++;
        }
        fclose(arquivo);
    }
    return listaSemPrioridade;
}

void organizarLista(unsigned char* listaSP[])
{

}

int alturaArvore(No* raiz) {
    int esq, dir;
    if (raiz == NULL)
        return -1;
    else {
        esq = alturaArvore(raiz->esq) + 1;
        // por exemplo: raiz.esq é só tem uma folha, então, a altura da árvore é 2 do lado esquerdo
        dir = alturaArvore(raiz->dir) + 1;
        // por exemplo: raiz.dir tem duas folhas, então a altura do lado direito é 3

        if (esq > dir)
            return esq;
        else
            return dir; // retornamos o lado maior, então se forem 2 e 3 a altura da árvore,
                        // a altura total é 3
    }
}

char** alocarDicionario() {
    char** dicionario; // matriz dinâmica

    dicionario = malloc(sizeof(char*) * tamanhoLista);

    for (int i = 0; i < tamanhoLista; i++) {
        dicionario[i] = calloc(21, sizeof(char)); // aloca uma região de memória e a limpa
    }

    return dicionario;
}

void criarDicionario(char** dicionario, No* raiz, char* caminho) {
    printf("Entrada em criarDicionario()");
    char esq[tamanhoOutros], dir[tamanhoOutros];
    if (raiz->esq == NULL && raiz->dir == NULL) { // se chegamos em uma folha,
        printf("Entrada no if");
        strcpy(caminho, dicionario[raiz->conteudo]); // armazenamos o
    }

    else {
        printf("Entrada no else");
        strcpy(esq, caminho);
        strcpy(dir, caminho);

        strcat(esq, "0");
        strcat(dir, "1"); // se formos pra direita, armazenamos o 1 e depois entendemos que "andamos" pra direita

        criarDicionario(dicionario, raiz->esq, esq); // criamos outros dicionarios para os filhos do nó
        criarDicionario(dicionario, raiz->dir, dir);
    }
}

int toInt(char c)
{
    char string[2];

    string[0] = c;
    string[1] = '\0';

    return (int)strtol(string, NULL, 10);
}

char* codificar(char** dicionario, char texto[])
{
    char* codigo = calloc(tamanhoOutros, sizeof(char));
    for (int i = 0; texto[i] != '\0'; i++) {
        strcat(codigo, dicionario[texto[i]]);
    }

    return codigo;
}

void compactar(char arq[])
{
    char arquivo[tamanhoChar];
    for (int i = 0; arq[i] != '\0'; i++) {
        arquivo[i] = arq[i];
    }
    printf("Compactação iniciada !!!\n");
    FILE* open = fopen(arquivo, "rb"); // abrimos o arquivo para leitura

    if (!open) {
        printf("O arquivo não existe!!! (Erro em compactar)");
        return;
    }

    int c;
    char conteudo[tamanhoChar] = "";
    char temp[2];

    while ((c = fgetc(open)) != EOF)
    {
        temp[0] = c;
        temp[1] = '\0';
        strcat(conteudo, temp); // concatenamos o conteudo do arquivo com o final de string
    }

    if ((c == EOF) && (feof(open) == 0) && (ferror(open) != 0)) {
        // se tiver algum erro durante a leitura do arquivo,
        perror("Error: fgetc");
    }

    fclose(open);

    unsigned int tabFreq[tamanhoLista];
    iniciarTab(tabFreq);

    for (int i = 0; conteudo[i] != '\0'; i++) {
        tabFreq[conteudo[i]]++; // contando a frequência
    }
    printf("Tabela de frequência iniciada e preenchida!\n");

    Lista lista;
    iniciarLista(&lista);
    preencherLista(tabFreq, &lista); // preenchendo a lista de acordo com as frequências
    printf("Lista iniciada e preenchida!\n");

    No* arvore;
    arvore = montarArvore(&lista);
    printf("Árvore montada!\n");

    char** dicionario = alocarDicionario();
    printf("Dicionario alocado\n");
    criarDicionario(dicionario, arvore, "");
    printf("Dicionário montado\n");

    char* codificado = codificar(&dicionario, conteudo);
    printf("Conteúdo inserido no dicionário\n");

    int length = 0; // iniciação fora do for, pq precisamos da variavel para o outro for
    for (; arquivo[length] != '\0'; length++) {} // contar o tamanho da "string" do arquivo


    for (int i = length; i > 0; i--) {
        if (arquivo[i] == '.') {
            // ex.: C:\app\huffman.png => vamos parar no . e tirar, para só ficar o nome do arquivo, como C:\app\huffman
            for (int j = i; j < length; j++) {
                arquivo[j] = '\0'; // colocamos o '\0' para indicar o fim da string
            }
            break; // paramos o for
        }
    }

    strcat(arquivo, ".huff"); // concatenação do nome do arquivo para o compactado .huff
    FILE* novo;
    novo = fopen(arquivo, "wb"); // criação de um arquivo com o nome anteriormente passado

    for (int i = 0; i < tamanhoLista; i++) {
        fprintf(novo, "%d", tabFreq[i]);
    }

    fputc('\n', arquivo);

    for (int j = 0; codificado[j]; j++) {
        fputc(codificado[j], novo); // colocar o codificado no novo arquivo
    }

    fclose(novo);

    printf("Confira o seu novo arquivo em %s", arquivo);
}

unsigned int conteudoBit(unsigned char byte[], int pos) { // retorna
    if (sizeof(byte) <= pos)
        printf("ERRO!!!! A posição passada como parâmetro não pode ser maior que o tamanho da string");
    unsigned char bit = byte[pos];
    return bit;
}

void descompactar(char arq[])
{
    char arquivo[tamanhoChar];
    for (int i = 0; arq[i] != '\0'; i++) {
        arquivo[i] = arq[i];
    }
    FILE* open = fopen(arquivo, "rb"); // abrimos o arquivo para leitura

    if (!open) {
        printf("O arquivo não existe!!! (Erro em descompactar)");
        return;
    }

    int c;
    int tabFreq[tamanhoLista];
    iniciarTab(tabFreq);
    char conteudo[tamanhoChar] = "";
    char temp[2];

    for (int i = 0; (c = fgetc(open)) != '\n'; i++) { // até achar o \n previamente inserido pelo codificar
        tabFreq[i] = toInt(c);
    }

    while ((c = fgetc(open)) != EOF)
    {
        temp[0] = c;
        temp[1] = '\0';
        strcat(conteudo, temp); // concatenamos o conteudo com o final
    }

    if ((c == EOF) && (feof(open) == 0) && (ferror(open) != 0)) {
        // se tiver algum erro durante a leitura do arquivo,
        perror("Error: fgetc");
    }

    Lista lista;
    iniciarLista(&lista);
    preencherLista(tabFreq, &lista);

    No* arvore;
    arvore = montarArvore(&lista);

    char** dicionario = alocarDicionario();
    criarDicionario(dicionario, arvore, "");

    int length = 0; // iniciação fora do for, pq precisamos da variavel para o outro for
    for (; arquivo[length] != '\0'; length++) {} // contar o tamanho da "string" do arquivo

    for (int i = length; i > 0; i--) {
        if (arquivo[i] == '.') {
            // ex.: C:\app\huffman.huff => vamos parar no . e tirar o .huff, para ficar o nome original do arquivo, como C:\app\huffman.png
            for (int j = i; j < length; j++) {
                arquivo[j] = '\0'; // colocamos o '\0' para indicar o fim da string
            }
            break; // paramos o for
        }
    }

    FILE* novo;
    novo = fopen(arquivo, "wb"); // criação de um arquivo com o nome anteriormente passado

    for (int i = 0; i < tamanhoLista; i++) {
        fprintf(novo, "%d", tabFreq[i]);
    }

    unsigned char umByte[9];
    No* r = arvore;

    if (open) {
        char descompactando[tamanhoChar];
        while (fread(&umByte, sizeof(unsigned char), 1, open)) {
            for (int i = 7; i >= 0, i--;) {
                if (!conteudoBit(umByte, i)) // se o bit for 1
                    r = r->dir; // o ponteiro vai se mover para a direita da árvore
                else // se o bit for 0
                    r = r->esq; // o ponteiro vai se mover para a esquerda da árvore

                if (r->esq == NULL && r->dir == NULL) { // atingimos uma folha
                    fputc(r->conteudo, novo); // guardar ela em um novo arquivo descompactado
                }
            }
        }
        fclose(open);
        fclose(novo);
        printf("Seu arquivo foi descompactado, olhe em %s", arquivo);
    }
    else
        printf("ERRO!!!! (descompactar)");
}

int main()
{
    char arquivo[51];
    char escolha = ' ';

    setlocale(LC_ALL, "Portuguese");

    do {
        printf("\nSeja bem-vindo ao compilador e descompilador de Huffman !\n");
        printf("Você gostaria de:\nA- Compactar um arquivo\nB- Descompactar um arquivo\nC- Encerrar o programa\n");
        fflush(stdin);
        scanf("%c", &escolha);
        fflush(stdout);

        if (escolha == 'C')
            break;

        printf("Digite o endereço do arquivo que gostaria de abrir:\n");
        fflush(stdin);
        scanf("%s", &arquivo);
        fflush(stdout);

        switch (escolha) {
        case 'A': compactar(arquivo); break;
        case 'B': descompactar(arquivo); break;
        }
    } while (escolha != 'C');

    printf("Obrigado por utilizar nosso programa. Volte sempre!");
}
