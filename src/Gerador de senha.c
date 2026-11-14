#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>

// Guarda o ID e o texto de cada senha.
typedef struct {
    int id;
    char senha[27];
} Senha;

// Limpa a tela do terminal.
void limparTela(void) {
    system("cls");
}

// Mostra as opcoes do menu principal.
void menu(void) {
    printf("\n========== GERADOR DE SENHAS ALEATORIAS ==========\n");
    printf("1 - Criar senha\n");
    printf("2 - Verificar senhas\n");
    printf("3 - Alterar senha\n");
    printf("4 - Excluir senha\n");
    printf("5 - Sair do sistema\n");
}

// Le um numero e verifica se esta dentro do intervalo permitido.
int validarOpcao(int minimo, int maximo) {
    int opcao, leitura, c;
    do {
        opcao = 0;
        printf("Opcao: ");
        leitura = scanf("%d", &opcao);
        // Descarta caracteres que sobraram na linha digitada.
        while ((c = getchar()) != '\n' && c != EOF) {}
        if (leitura == EOF || c == EOF) return -1;
        if (leitura != 1 || opcao < minimo || opcao > maximo)
            printf("\nOpcao invalida! Digite novamente.\n");
    } while (leitura != 1 || opcao < minimo || opcao > maximo);
    return opcao;
}

// Permite escolher os tipos de caracteres, sem repetir opcoes.
int selecionarCaracteres(int selecionados[4]) {
    int opcao, resposta, quantidadeTipos = 0;
    do {
        printf("\n========== TIPOS DE CARACTERES ==========\n");
        printf("Selecione um tipo de caractere:\n");
        if (!selecionados[0]) printf("1 - Letras minusculas (a-z)\n");
        if (!selecionados[1]) printf("2 - Letras maiusculas (A-Z)\n");
        if (!selecionados[2]) printf("3 - Caracteres especiais\n");
        if (!selecionados[3]) printf("4 - Numeros (0-9)\n");

        do {
            opcao = validarOpcao(1, 4);
            if (opcao == -1) return 0;
            if (selecionados[opcao - 1])
                printf("\nTipo ja selecionado! Escolha outro.\n");
        } while (selecionados[opcao - 1]);

        // Marca a opcao escolhida como selecionada.
        selecionados[opcao - 1] = 1;
        quantidadeTipos++;
        if (quantidadeTipos == 4) break;

        printf("\nDeseja adicionar outro tipo de caractere?\n");
        printf("1 - Sim\n2 - Nao\n");
        resposta = validarOpcao(1, 2);
        if (resposta == -1) return 0;
    } while (resposta == 1);
    return 1;
}

// Gera a senha usando apenas os tipos de caracteres escolhidos.
void gerarSenha(char destino[27], int tamanho, const int selecionados[4]) {
    char caracteres[100] = "";
    int i, total;
    // Junta os caracteres das categorias escolhidas.
    if (selecionados[0]) strcat(caracteres, "abcdefghijklmnopqrstuvwxyz");
    if (selecionados[1]) strcat(caracteres, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (selecionados[2]) strcat(caracteres, "!@#$%&*()-_=+?");
    if (selecionados[3]) strcat(caracteres, "0123456789");
    total = (int)strlen(caracteres);
    // Sorteia um caractere permitido para cada posicao da senha.
    for (i = 0; i < tamanho; i++)
        destino[i] = caracteres[rand() % total];
    // Marca o fim da senha como texto em C.
    destino[tamanho] = '\0';
}

// Pede as preferencias, gera a senha e guarda no array dinamico.
void criarSenha(Senha **senhas, int *quantidade, int *capacidade, int *proximoId) {
    Senha nova = {0};
    int tamanho;
    int selecionados[4] = {0};

    printf("\n========== CRIAR SENHA ==========\n");
    printf("\nInforme o tamanho da senha (8 a 26):\n");
    tamanho = validarOpcao(8, 26);
    if (tamanho == -1) return;
    if (!selecionarCaracteres(selecionados)) return;

    if (*proximoId == INT_MAX) {
        printf("\nLimite de IDs atingido.\n");
        return;
    }
    // Aumenta o array em uma posicao quando nao houver espaco.
    if (*quantidade >= *capacidade) {
        int novaCapacidade;
        Senha *temporario;
        if (*capacidade == INT_MAX ||
            (size_t)(*capacidade + 1) > SIZE_MAX / sizeof(Senha)) {
            printf("\nLimite de memoria atingido.\n");
            return;
        }
        novaCapacidade = *capacidade + 1;
        // Tenta aumentar a memoria sem perder os registros existentes.
        temporario = realloc(*senhas, (size_t)novaCapacidade * sizeof(Senha));
        if (temporario == NULL) {
            printf("\nErro ao aumentar a memoria.\n");
            return;
        }
        *senhas = temporario;
        *capacidade = novaCapacidade;
    }

    gerarSenha(nova.senha, tamanho, selecionados);
    // Atribui um ID sequencial a nova senha.
    nova.id = *proximoId;
    // Guarda a nova senha na proxima posicao livre do array.
    (*senhas)[*quantidade] = nova;
    (*quantidade)++;
    (*proximoId)++;
    printf("\nSenha criada com sucesso!\n");
    printf("Identificador: SENHA-%d\n", nova.id);
    printf("Senha gerada: %s\n", nova.senha);
}

// Inicia o programa e controla as opcoes do menu.
int main(void) {
    int quantidade = 0, capacidade = 5, proximoId = 1;
    int opcao;
    // Reserva espaco inicial para cinco senhas na memoria.
    Senha *senhas = malloc((size_t)capacidade * sizeof(Senha));
    if (senhas == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }
    // Usa o horario como semente para variar a sequencia do rand() entre execucoes.
    srand((unsigned int)time(NULL));
    // Repete o menu ate o usuario escolher sair.
    do {
        menu();
        opcao = validarOpcao(1, 5);
        if (opcao == -1) break;
        switch (opcao) {
            case 1:
                limparTela();
                criarSenha(&senhas, &quantidade, &capacidade, &proximoId);
                break;
            case 2:
                limparTela();
                printf("\n========== VERIFICAR SENHAS ==========\n");
                printf("Funcionalidade ainda nao implementada.\n");
                break;
            case 3:
                limparTela();
                printf("\n========== ALTERAR SENHA ==========\n");
                printf("Funcionalidade ainda nao implementada.\n");
                break;
            case 4:
                limparTela();
                printf("\n========== EXCLUIR SENHA ==========\n");
                printf("Funcionalidade ainda nao implementada.\n");
                break;
            case 5:
                printf("\nEncerrando o programa.\n");
                break;
        }
    } while (opcao != 5);
    // Libera a memoria reservada para as senhas.
    free(senhas);
    return 0;
}
