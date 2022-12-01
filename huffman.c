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
    struct No* esq, * dir, * prox; // colocamos que o n� tenha conex�o com tr�s n�s,
    // o esq e o dir s�o referentes a �rvore e o prox � referente a lista de prioridade
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
        lista->inicio = aux->prox; // raiz muda de raiz, j� que o n� foi removido
        aux->prox = NULL; // tira o n�
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
            junto->conteudo = ' '; // n�o possui conte�do, � s� um ligador
            junto->freq = no1->freq + no2->freq; // soma das frequ�ncias dos dois n�s
            junto->esq = no1;
            junto->dir = no2;
            junto->prox = NULL;

            inserirOrdenado(lista, junto); // vamos colocar o n� da �rvore de volta na lista;
        }
        else {
            printf("\nErro ao montar �rvore");
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

    if (lista->inicio == NULL) { // se a lista estiver vazia, adicionamos o n� no in�cio
        lista->inicio = no;
    }

    else if (no->freq <= lista->inicio->freq) { // se a frequ�ncia do n� for menor ou igual
        no->prox = lista->inicio; // a frequ�ncia do byte no come�o da lista
        lista->inicio = no;
    }

    else { // se a frequ�ncia do n� for maior que o n� do come�o da lista
        aux = lista->inicio;
        while (aux->prox == NULL && aux->prox->freq < no->freq) { // vamos percorrer  lista at� que o a frequ�ncia do n�
            aux = aux->prox; // seja maior do que a do aux, ou seja, at� que a gente ache onde o n� se encaixa na fila de prioridade
        }
        // quando a posi��o for achada,
        no->prox = aux->prox; // colocaremos o pr�ximo da posi��o atual como pr�ximo do n�
        aux->prox = no; // e o n� se encaixar� como pr�ximo do n�
        // antes:  | aux | prox |   a  | b | c |...
        // depois: | aux |  n�  | prox | a | b |...
    }

    lista->tamanho++; // o tamanho da lista aumenta j� que a inser��o foi realizada nos tr�s if else
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
        // por exemplo: raiz.esq � s� tem uma folha, ent�o, a altura da �rvore � 2 do lado esquerdo
        dir = alturaArvore(raiz->dir) + 1;
        // por exemplo: raiz.dir tem duas folhas, ent�o a altura do lado direito � 3

        if (esq > dir)
            return esq;
        else
            return dir; // retornamos o lado maior, ent�o se forem 2 e 3 a altura da �rvore,
                        // a altura total � 3
    }
}

char** alocarDicionario() {
    char** dicionario; // matriz din�mica

    dicionario = malloc(sizeof(char*) * tamanhoLista);

    for (int i = 0; i < tamanhoLista; i++) {
        dicionario[i] = calloc(21, sizeof(char)); // aloca uma regi�o de mem�ria e a limpa
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

        criarDicionario(dicionario, raiz->esq, esq); // criamos outros dicionarios para os filhos do n�
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
    printf("Compacta��o iniciada !!!\n");
    FILE* open = fopen(arquivo, "rb"); // abrimos o arquivo para leitura

    if (!open) {
        printf("O arquivo n�o existe!!! (Erro em compactar)");
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
        tabFreq[conteudo[i]]++; // contando a frequ�ncia
    }
    printf("Tabela de frequ�ncia iniciada e preenchida!\n");

    Lista lista;
    iniciarLista(&lista);
    preencherLista(tabFreq, &lista); // preenchendo a lista de acordo com as frequ�ncias
    printf("Lista iniciada e preenchida!\n");

    No* arvore;
    arvore = montarArvore(&lista);
    printf("�rvore montada!\n");

    char** dicionario = alocarDicionario();
    printf("Dicionario alocado\n");
    criarDicionario(dicionario, arvore, "");
    printf("Dicion�rio montado\n");

    char* codificado = codificar(&dicionario, conteudo);
    printf("Conte�do inserido no dicion�rio\n");

    int length = 0; // inicia��o fora do for, pq precisamos da variavel para o outro for
    for (; arquivo[length] != '\0'; length++) {} // contar o tamanho da "string" do arquivo


    for (int i = length; i > 0; i--) {
        if (arquivo[i] == '.') {
            // ex.: C:\app\huffman.png => vamos parar no . e tirar, para s� ficar o nome do arquivo, como C:\app\huffman
            for (int j = i; j < length; j++) {
                arquivo[j] = '\0'; // colocamos o '\0' para indicar o fim da string
            }
            break; // paramos o for
        }
    }

    strcat(arquivo, ".huff"); // concatena��o do nome do arquivo para o compactado .huff
    FILE* novo;
    novo = fopen(arquivo, "wb"); // cria��o de um arquivo com o nome anteriormente passado

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
        printf("ERRO!!!! A posi��o passada como par�metro n�o pode ser maior que o tamanho da string");
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
        printf("O arquivo n�o existe!!! (Erro em descompactar)");
        return;
    }

    int c;
    int tabFreq[tamanhoLista];
    iniciarTab(tabFreq);
    char conteudo[tamanhoChar] = "";
    char temp[2];

    for (int i = 0; (c = fgetc(open)) != '\n'; i++) { // at� achar o \n previamente inserido pelo codificar
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

    int length = 0; // inicia��o fora do for, pq precisamos da variavel para o outro for
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
    novo = fopen(arquivo, "wb"); // cria��o de um arquivo com o nome anteriormente passado

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
                    r = r->dir; // o ponteiro vai se mover para a direita da �rvore
                else // se o bit for 0
                    r = r->esq; // o ponteiro vai se mover para a esquerda da �rvore

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
        printf("Voc� gostaria de:\nA- Compactar um arquivo\nB- Descompactar um arquivo\nC- Encerrar o programa\n");
        fflush(stdin);
        scanf("%c", &escolha);
        fflush(stdout);

        if (escolha == 'C')
            break;

        printf("Digite o endere�o do arquivo que gostaria de abrir:\n");
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
