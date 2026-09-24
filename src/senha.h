// Constantes do sistema 

#define TAM_MIN_SENHA 1
#define TAM_MAX_SENHA 32
#define MAX_SENHAS 100
#define ARQUIVO_SENHAS "senhas.txt"
#define ARQUIVO_TEMP "senhas_temp.txt"

// Dados de uma senha gerada 

struct Senha {
    int id;
    char texto[TAM_MAX_SENHA + 1];
    int tamanho;
};

// Escolhas do usuario para gerar a senha (1 = sim, 0 = não) 
struct ConfiguracaoSenha {
    int tamanho;
    int usarMaiusculas;
    int usarMinusculas;
    int usarNumeros;
    int usarEspeciais;
};

// Senhas guardadas em memória durante a execução 
struct ListaSenhas {
    struct Senha senhas[MAX_SENHAS];
    int quantidade;
    int proximoId;
};
