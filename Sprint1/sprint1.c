#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>   // para usar isalpha, digit, isspace, tolower
#include <stdbool.h> // para usar booleano

#define MAX_USUARIOS 30 // quantidade maxima de usuarios
#define MAX_NOME 30     // tamanho maximo do nome
#define MAX_SENHA 20    // tamanho maximo da senha
#define MAX_VENDAS 200  // tamanho maximo do relatorio de vendas
#define MAX_LINHA 100   // tamanho maximo do buffer de leitura de uma linha de arquivo
#define MAX_PRODUTO 100 // quantidade maxima de produtos cadastrados

int idSessao = 0;  // variavel para controle de "sessao", identifica o usuario logado
int position;      // variavel de controle
int excedido = 0;  // variavel de controle
int idUsuario = 0; // inicia com valor 0 antes da leitura dos usuarios ser feita e atribuida ao id

char senhaADM[] = "1234";
char senhaAdmin[MAX_SENHA];
char adm[] = "adm";
char caixa[] = "caixa";

typedef struct
{ // estrutura de variaveis atribuídas a um usuario
    char nome[MAX_NOME];
    char senha[MAX_SENHA];
    char cargo[MAX_NOME];
    int id;
} estrutura;

estrutura user[MAX_USUARIOS]; // array com os valores da estrutura

int limpar_tela() // funcao para limpar a tela
{
    system("cls"); // comando do Windows para limpar a tela
    return 1;
}

void limpabuffer() // funcao para limpar o buffer apos um endereco de memoria ser escrito
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // corpo vazio para limpar o endereco de memoria
    }
}

void toLowerCase(char *str) // transforma maiusculo em minusculo
{
    int i = 0;
    while (str[i] != '\0')
    {
        str[i] = tolower(str[i]);
        i++;
    }
}

void remove_espacos(char *str) // remove espacos de uma string
{
    char *dest = str; // ponteiro para o destino da string sem espacos
    while (*str)
    {
        if (!isspace((unsigned char)*str)) // verifica se o caractere nao e um espaco
        {
            *dest++ = *str; // copia o caractere nao espaco para o destino
        }
        str++; // avanca para o proximo caractere
    }
    *dest = '\0'; // adiciona o caractere nulo no final da nova string
}

void lerUsuarios() // funcao que executa a leitura do banco de dados
{
    FILE *leitura = fopen("idusersenha.txt", "r");
    if (leitura == NULL)
    {
        perror("Erro ao abrir arquivo");
        return;
    }

    idUsuario = 0;
    while (fgets(user[idUsuario].nome, MAX_NOME, leitura) != NULL) // le uma linha do arquivo por vez
    {
        user[idUsuario].nome[strcspn(user[idUsuario].nome, "\n")] = '\0';
        char *split = strtok(user[idUsuario].nome, "|"); // uso da funcao strtok() para "destrinchar" a linha lida

        user[idUsuario].id = atoi(split); // funcao atoi() transforma string em inteiro

        if (split != NULL)
        {
            split = strtok(NULL, "|"); // agora split recebe o nome lido
            strcpy(user[idUsuario].nome, split); // atribui split para o nome do usuario

            if (split != NULL)
            {
                split = strtok(NULL, "|"); // aqui split ira ignorar o token atual e ler o proximo
                strncpy(user[idUsuario].senha, split, MAX_SENHA - 1);
                user[idUsuario].senha[MAX_SENHA - 1] = '\0'; // define o ultimo caractere de .senha como nulo

                split = strtok(NULL, "|"); // ultimo token
                strncpy(user[idUsuario].cargo, split, MAX_SENHA - 1);
                user[idUsuario].cargo[MAX_SENHA - 1] = '\0';
                idUsuario++; // atribui a quantidade de usuarios cadastrados

                if (idUsuario >= MAX_USUARIOS)
                {
                    excedido = 1;
                    break;
                }
            }
        }
    }
    fclose(leitura);
}

int autentica() // funcao que verifica se o nome de usuario ja esta cadastrado
{
    lerUsuarios();
    char confirmaNome[MAX_NOME]; // variavel de usuario
    char confirmaSenha[MAX_SENHA]; // variavel para comparacao de senha

    printf("\tDigite o nome de usuario: ");
    fgets(confirmaNome, MAX_NOME, stdin);
    confirmaNome[strcspn(confirmaNome, "\n")] = '\0';
    remove_espacos(confirmaNome);
    toLowerCase(confirmaNome);

    printf("\tDigite a senha: ");
    fgets(confirmaSenha, MAX_SENHA, stdin);
    confirmaSenha[strcspn(confirmaSenha, "\n")] = '\0';
    remove_espacos(confirmaSenha);

    for (int i = 0; i < idUsuario; i++) // percorre todos os usuarios
    {
        if (strcmp(confirmaNome, user[i].nome) == 0) // compara nome com usuario
        {
            if (strcmp(confirmaSenha, user[i].senha) == 0) // se a senha estiver correta
            {
                limpar_tela();
                idSessao = i; // atribui o ID do usuario logado
                return 1;
            }
        }
    }
    limpar_tela();
    return 0;
}

void cadastrarUsuario() // funcao responsavel pelo cadastro de usuarios
{
    if (excedido == 1) // se a quantidade maxima de usuarios ja estiver cadastrada
    {
        printf("Numero maximo de usuarios excedido\n");
        return;
    }
    limpar_tela();
    printf("Para cadastrar um novo usuario, por favor, digite a senha do administrador (ADM)");
    if (fgets(senhaAdmin, MAX_SENHA, stdin) != NULL) // insere senha de adm
    {
        senhaAdmin[strcspn(senhaAdmin, "\n")] = '\0';
        limpar_tela();
        if (strcmp(senhaAdmin, senhaADM) == 0) // compara senha de adm
        {
            estrutura novoUsuario; // nova estrutura para cadastro

            printf("\tDigite o nome de usuario: ");
            if (fgets(novoUsuario.nome, MAX_NOME, stdin) != NULL) // armazena nome
            {
                novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0';
                toLowerCase(novoUsuario.nome);
                remove_espacos(novoUsuario.nome);
                if (strlen(novoUsuario.nome) >= 4) // verifica tamanho do nome
                {
                    int usuarioExiste = 0;
                    for (int i = 0; i < idUsuario; i++) // verifica se usuario ja existe
                    {
                        if (strcmp(user[i].nome, novoUsuario.nome) == 0)
                        {
                            usuarioExiste = 1;
                            break;
                        }
                    }
                    if (usuarioExiste) // usuario existente
                    {
                        limpar_tela();
                        printf("Usuario existente\n");
                    }
                    else // cadastro do novo usuario
                    {
                        printf("\tDigite a senha: ");
                        if (fgets(novoUsuario.senha, MAX_SENHA, stdin) != NULL) // valor de senha
                        {
                            novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0';
                            remove_espacos(novoUsuario.senha);
                            if (strlen(novoUsuario.senha) >= 4) // verifica tamanho da senha
                            {
                                printf("Digite 1 para adm ou 2 para caixa: ");
                                if (scanf("%d", &position) == 1)
                                {
                                    limpabuffer(); // limpar buffer

                                    if (position == 1)
                                    {
                                        strcpy(novoUsuario.cargo, adm); // atribui cargo
                                    }
                                    else if (position == 2)
                                    {
                                        strcpy(novoUsuario.cargo, caixa); // atribui cargo
                                    }
                                    else
                                    {
                                        printf("Opcao invalida. Usuario nao cadastrado.\n");
                                        getch();
                                        limpar_tela();
                                        return;
                                    }

                                    user[idUsuario] = novoUsuario; // atribui novos valores
                                    idUsuario++;
                                    FILE *escrevearquivo = fopen("idusersenha.txt", "a"); // abre o banco de dados
                                    if (escrevearquivo == NULL)
                                    {
                                        perror("Erro ao abrir arquivo");
                                        return;
                                    }
                                    fprintf(escrevearquivo, "%d|%s|%s|%s\n", idUsuario, user[idUsuario - 1].nome, user[idUsuario - 1].senha, user[idUsuario - 1].cargo); // insere novo usuario
                                    fclose(escrevearquivo);

                                    printf("Usuario cadastrado com sucesso!\n");
                                    getch();
                                    limpar_tela();
                                    return;
                                }
                                else
                                {
                                    printf("Opcao invalida\n");
                                    getch();
                                    limpar_tela();
                                    return;
                                }
                            }
                            else
                            {
                                printf("Senha deve ter no minimo 4 caracteres\n");
                                getch();
                                limpar_tela();
                                return;
                            }
                        }
                        else
                        {
                            return;
                        }
                    }
                }
                else
                {
                    printf("Nome deve ter no minimo 4 caracteres\n");
                    getch();
                    limpar_tela();
                    return;
                }
            }
            else
            {
                printf("Erro ao ler o nome de usuario\n");
                getch();
                limpar_tela();
                return;
            }
        }
        else
        {
            printf("Senha incorreta!\n");
            getch();
            limpar_tela();
            return;
        }
    }
}

int obterOpcao() // funcao responsavel por mostrar o menu
{
    int opcao = 0;
    char buffer[100];

    while (1) // mostra o menu ate que o usuario digite uma opcao valida
    {
        printf("\nMenu:\n");
        printf("1. Cadastrar usuario\n");
        printf("2. Autenticar usuario\n");
        printf("3. LimparMenu\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &opcao) == 1 && opcao >= 1 && opcao <= 4) // filtra a escolha do menu
            {
                return opcao; // retorna a escolha para a funcao main()
            }
            else
            {
                limpar_tela();
                printf("Opcao invalida. Tente novamente.\n");
            }
        }
        else
        {
            limpar_tela();
            printf("Erro ao ler a entrada. Tente novamente.\n");
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    lerUsuarios();
    limpar_tela();
    do
    {
        opcao = obterOpcao(); // obtem o valor retornado pela funcao obterOpcao()

        switch (opcao)
        {
        case 1:
            cadastrarUsuario();
            break;
        case 2:
            if (autentica() == 1) // se usuario foi autenticado com sucesso
            {
                int autenticaOpcao = 0;
                limpar_tela();
                if (strcmp(user[idSessao].cargo, adm) == 0) // se for adm
                {
                    autenticaOpcao = 1;
                }
                else if (strcmp(user[idSessao].cargo, caixa) == 0) // se for caixa
                {
                    autenticaOpcao = 2;
                }
                printf("Bem Vindo, %s, %s\n", user[idSessao].nome, user[idSessao].cargo);

                switch (autenticaOpcao)
                {
                case 1: // se for adm
                    printf("ADMINISTRADOR");
                    // menuADM();
                    break;

                case 2: // se for caixa
                    printf("CAIXA");
                    // menuCAIXA();
                    break;
                }
            }
            else
            {
                limpar_tela();
                printf("Falha na autenticacao. Usuario ou senha incorretos.\n");
            }
            break;
        case 3:
            limpar_tela();
            main();
            break;
        case 4:
            printf("Saindo...\n");
            return 0;
            break;
        default:
            break;
        }
    } while (opcao != 3);

    return 0;
}
