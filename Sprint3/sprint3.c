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

int idSessao = 0;             // variavel para controle de "sessao", identifica o usuario logado
int position = 0;             // variavel de controle
int idvenda = 0;              // SPRINT 3  - id relatorio de vendas
int excedido = 0;             // variavel de controle
int idUsuario = 0;            // inicia com valor 0 antes da leitura dos usuarios ser feita e atribuida ao id
int idproduto = 0;            // #SPRINT 2 contem o id de cada produto cadastrado
char debito[] = "debito";     // SPRINT 3
char credito[] = "credito";   // SPRINT 3
char dinheiro[] = "dinheiro"; // SPRINT 3
char senhaADM[] = "1234";
char senhaAdmin[MAX_SENHA];
char adm[] = "adm";
char caixa[] = "caixa";

typedef struct // #SPRINT 3
{
    int id;
    char vendedor[30];
    char produto[30];
    float preco;
    char horariovenda[20];
    char pagamento[20];
} structvendas;

structvendas vendas[999];
typedef struct // #SPRINT 1
{              // estrutura de variaveis atribuídas a um usuario
    char nome[MAX_NOME];
    char senha[MAX_SENHA];
    char cargo[MAX_NOME];
    int id;
} estrutura;

estrutura user[MAX_USUARIOS]; // array com os valores da estrutura

typedef struct // #SPRINT 2
{              // #SPRINT 2 estrutura de variaveis atribuidas a um produto
    char nome[MAX_NOME];
    int id;
    float preco;
} structproduto;

structproduto produto[MAX_PRODUTO]; // array com os valores da estrutura

int limpar_tela() // #SPRINT 1 funcao para limpar a tela
{
    system("cls"); // comando do Windows para limpar a tela
    return 1;
}

typedef struct
{ // SPRINT 3, estrutura para armazenar nomes de clientes
    char nome[MAX_NOME];
    char phone[12];

} structcliente;

structcliente clientes[MAX_LINHA];
void limpabuffer() // #SPRINT 1 funcao para limpar o buffer apos um endereco de memoria ser escrito
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // corpo vazio para limpar o endereco de memoria
    }
}

int contemLetras(const char *str) // SPRINT 3 - Verifica se contem letras na string
{
    while (*str)
    {
        if (isalpha((unsigned char)*str))
        {
            return 1; // Retorna 1 se encontrar uma letra
        }
        str++;
    }
    return 0; // Retorna 0 se não encontrar nenhuma letra
}

int contemDigitos(const char *str)
{ // SPRINT 3 Função para verificar se contém apenas dígitos

    while (*str)
    {
        if (!isdigit((unsigned char)*str))
        {
            return 0; // Retorna 0 se encontrar um caractere que não seja dígito
        }
        str++;
    }
    return 1; // Retorna 1 se todos os caracteres forem dígitos
}

void toLowerCase(char *str) // #SPRINT 1 transforma maiusculo em minusculo
{
    int i = 0;
    while (str[i] != '\0')
    {
        str[i] = tolower(str[i]);
        i++;
    }
}

void remove_espacos(char *str) // #SPRINT 1 remove espacos de uma string
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

bool verificastringdigito(const char *string) // #SPRINT 2 FUNÇÃO QUE VERIFICA SE A STRING POSSUI NUMEROS
{
    while (*string)
    {
        if (isdigit((unsigned char)*string))
        {
            return true;
        }
        string++;
    }
    return false;
}

void lerProduto() // #SPRINT 2 - responsavel pela leitura do banco de dados produtos.txt e atribuição dos produtos ao array de produtos
{
    FILE *lerproduto = fopen("produtos.txt", "r");
    if (lerproduto == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[MAX_LINHA];
    idproduto = 0;
    // le a primeira linha do arquivo
    while (fgets(linha, sizeof(linha), lerproduto) != NULL)
    {
        // Remove o caractere de nova linha, se presente
        linha[strcspn(linha, "\n")] = '\0';
        char *parse = strtok(linha, "|");

        produto[idproduto].id = atoi(parse); // atribui o id ao produto lido
        // printf("%i|", produto[idproduto].id);
        if (parse != NULL)
        {

            parse = strtok(NULL, "|");
            strcpy(produto[idproduto].nome, parse); // atribui o nome ao produto lido
            // printf("%s|", produto[idproduto].nome);

            if (parse != NULL)
            {
                parse = strtok(NULL, "|");
                produto[idproduto].preco = (float)atof(parse); // atribui o preço ao produto lido
                // printf("%.2f\n", produto[idproduto].preco);
                // printf("%i|", produto[idproduto].id);
                // printf("%s|", produto[idproduto].nome);
                // printf("%.2f\n", produto[idproduto].preco);
                idproduto++;
                if (idproduto >= MAX_PRODUTO) // se quant de produtos maximos cadastrados
                {
                    limpar_tela();
                    printf("numero de produtos maximos cadastrados excedidos!");
                    break;
                }
            }
        }
    }

    fclose(lerproduto);
}
void menuProdutos() // #SPRINT 2, Função que imprime na tela todos os produtos para consulta
{
    lerProduto();
    printf("\t       MENU\n\n");

    for (int i = 0; i < idproduto; i++) // percorre o array produtos a quantidade de produtos cadastrados
    {
        printf("\tCOD: %i -- %s -- R$%.2f KG\n\n", produto[i].id, produto[i].nome, produto[i].preco); // printa o menu na tela
    }
}

void substituirProdutoNoArquivo(int id, structproduto NovoP) // #SPRINT 2
{
    // essa função exclui o produto da linha passada atraves do id e reescreve em um novo arquivo sem o produto que foi excluido.
    FILE *arquivo = fopen("produtos.txt", "r"); // arquivo original
    FILE *arquivoTemp = fopen("temp.txt", "w"); // arquivo reescrito

    if (arquivo == NULL || arquivoTemp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];                             // ate 255 caracteres por linha
    while (fgets(linha, sizeof(linha), arquivo)) // lê linha do banco de dados
    {
        int idExistente;
        char nomeExistente[MAX_NOME];
        float precoExistente;

        // Ler os dados da linha atual e atribui às variaveis acima para efeito de comparação no proximo bloco
        sscanf(linha, "%d|%[^|]|%f", &idExistente, nomeExistente, &precoExistente);

        // Se id for igual ao produto que desejamos alterar ele ira regravar a linha com os novos dados
        if (idExistente == id)
        {
            fprintf(arquivoTemp, "%d|%s|%.2f\n", NovoP.id, NovoP.nome, NovoP.preco);
        }
        else
        {
            // Caso contrario, gravar a linha original
            fprintf(arquivoTemp, "%s", linha);
        }
    }

    fclose(arquivo);
    fclose(arquivoTemp);

    // Substituir o arquivo original pelo arquivo temporario
    remove("produtos.txt");
    rename("temp.txt", "produtos.txt");

    printf("\n\tProduto atualizado com sucesso!\n");
}

void cadastrarProduto() // #SPRINT 2, realiza o cadastro de produtos no banco de dados
{
    char entrada[MAX_NOME]; // nome do produto a ser cadastrado
    menuProdutos();         // imprime o menu de produtos para consulta
    structproduto NovoP;    // um novo array de produtos para compração de produto existente e cadastro de novo produto
    printf("\n\tInsira o nome do produto que deseja cadastrar: ");
    if (fgets(entrada, sizeof(entrada), stdin) != NULL) // nome do produto
    {
        entrada[strcspn(entrada, "\n")] = '\0';

        char entradanome[30];

        remove_espacos(entrada);
        toLowerCase(entrada);
        strcpy(entradanome, entrada);
        int entradaid = atoi(entrada);
        if (strlen(entrada) >= 3 || entradaid > 0) // se nome do produto for maior que 3 caracteres
        {
            strcpy(NovoP.nome, entradanome); // atribui o novo nome do produto a variavel nome do array NovoP

            for (int i = 0; i < idproduto; i++) // percorre os produtos cadastrados e verifica se o produto ja existe
            {
                if (strcmp(NovoP.nome, produto[i].nome) == 0 || entradaid == produto[i].id) // se existe:
                {
                    printf("\tO item %s ja esta cadastrado! Deseja alterar o valor? (s) sim (n) nao: ", produto[i].nome); // deseja alterar o produto?
                    char resposta[10];

                    if (fgets(resposta, sizeof(resposta), stdin))
                    {
                        resposta[strcspn(resposta, "\n")] = '\0';

                        if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) // se deseja alterar o produto
                        {
                            char novopreco[15]; // Buffer maior para o preco
                            float novoprecofloat;
                            printf("\tInsira o novo preco por kg: ");

                            if (fgets(novopreco, sizeof(novopreco), stdin) != NULL) // lê o novo valor do produto a ser alterado
                            {
                                novopreco[strcspn(novopreco, "\n")] = '\0';
                                novoprecofloat = strtof(novopreco, NULL); // strftof() transforma string em float e atribui na variavel novoprecofloat

                                produto[i].preco = novoprecofloat; // atribui o novo preco ao produto
                                if (produto[i].preco > 0)          // se preço for valido
                                {
                                    printf("\n\tResumo do produto a ser cadastrado:\n");
                                    printf("\tNome: %s\n", produto[i].nome);
                                    printf("\tpreco por kg: R$%.2f\n", produto[i].preco);
                                    printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                                    char opcao1[10];
                                    if (fgets(opcao1, sizeof(opcao1), stdin) != NULL) // le 1 confirma ou 2 cancela
                                    {
                                        int escolha = atoi(opcao1); // atoi() transforma string em inteiro

                                        switch (escolha)
                                        {
                                        case 1:                                                    // Confirma
                                            substituirProdutoNoArquivo(produto[i].id, produto[i]); // Chama a função que faz a alteração do valor no banco de dados, passando como parametro o endereço do produto a ser alterado
                                            getch();
                                            break;
                                        case 2: // Cancelar
                                            limpar_tela();
                                            printf("\tAlteracao do produto cancelada.\n");
                                            break;

                                        default:
                                            printf("Opção inválida. Cadastro não realizado.\n");
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        perror("Invalid");
                                        return;
                                    }
                                }
                            }
                            else
                            {
                                printf("Usuário escolheu não. Cancelando a alteracao...\n");
                            }
                        }
                        else
                        {
                            printf("Erro ao ler a entrada.\n");
                        }
                        return; // Parar a função após alterar o produto
                    }
                }
            }
            // Se o produto não existe, solicitar preco e confirmar o cadastro
            limpar_tela();
            printf("\n\tInsira o preco do KG do(a) %s: ", entrada);
            char novopreco[15];
            if (fgets(novopreco, sizeof(novopreco), stdin) != NULL) // le o novo preço em string
            {
                NovoP.preco = strtof(novopreco, NULL); // transforma a string em float e atribui na variavel preco do NovoP
                if (NovoP.preco > 0)
                {
                    NovoP.id = idproduto + 1; // Atribuir ID do novo produto

                    // Mostrar informações para confirmação
                    printf("\n\tResumo do produto a ser cadastrado:\n");
                    printf("\tNome: %s\n", NovoP.nome);
                    printf("\tpreco por kg: R$%.2f\n", NovoP.preco);
                    printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                    char opcao[10];
                    if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                    {
                        int escolha = atoi(opcao);

                        switch (escolha)
                        {
                        case 1: // Confirmar
                            produto[idproduto] = NovoP;
                            idproduto++;

                            // Escrever o novo produto no banco de dados produtos.txt
                            FILE *escreveProduto = fopen("produtos.txt", "a");
                            if (escreveProduto != NULL)
                            {
                                fprintf(escreveProduto, "%i|%s|%.2f\n", NovoP.id, NovoP.nome, NovoP.preco);
                                fclose(escreveProduto);
                                printf("\n\tProduto cadastrado com sucesso\n");
                            }
                            else
                            {
                                printf("Erro ao abrir o arquivo de produtos!\n");
                            }
                            break;

                        case 2: // Cancelar
                            printf("Cadastro do produto cancelado.\n");
                            break;

                        default:
                            printf("Opção inválida. Cadastro não realizado.\n");
                            break;
                        }
                    }
                    else
                    {
                        printf("Erro ao ler a escolha. Cadastro não realizado.\n");
                    }
                }
                else
                {
                    printf("preco inválido...\n");
                }
            }
        }
        else
        {
            limpar_tela();
            perror("Invalido");
            getch();
        }
    }

    else
    {
        limpar_tela();
        printf("Erro ao ler a entrada. Tente novamente.\n");
    }
}

void excluirProduto() // #SPRINT 2, função que permite excluir algum produto tanto pelo id quanto pelo nome, ela tambem reescreve o banco de dados após a exclusão para atualizar o id de todos os produtos após a exclusão de um.
{

    printf("\tDigite o ID/NOME do produto que deseja excluir: ");
    int idProduto;
    char idStr[MAX_NOME]; // buffer para entrada de dados
    if (fgets(idStr, sizeof(idStr), stdin) != NULL)
    {
        idStr[strcspn(idStr, "\n")] = '\0';
        remove_espacos(idStr);

        idProduto = atoi(idStr); // Converter a string para número

        int encontrado = -1; // variavel de controle caso o produto exista
        for (int i = 0; i < idproduto; i++)
        {
            if (produto[i].id == idProduto || strcmp(produto[i].nome, idStr) == 0) // entende tando o ID como o Nome do produto
            {
                if (strcmp(produto[i].nome, idStr) == 0)
                {
                    encontrado = i; // atribui a posição do produto ao ser encontrado
                    break;          // sai do laco ao ser encontrado
                }
                encontrado = i;
                break;
            }
        }

        if (encontrado != -1) // se produto for encontrado...
        {
            printf("\tProduto %s encontrado. Deseja realmente excluir? (s/n): ", produto[encontrado].nome);
            char resposta[5]; // buffer
            if (fgets(resposta, sizeof(resposta), stdin))
            {
                resposta[strcspn(resposta, "\n")] = '\0';

                if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0)
                {
                    // Remover o produto do array
                    for (int i = encontrado; i < idproduto - 1; i++) // le até a quantidade de produtos - 1 tendo em vista que foi excluido 1 produto da lista
                    {
                        produto[i] = produto[i + 1]; // atribui o proximo produto ao produto anterior sucessivamente, a partir do produto que foi encontrado, sobreescrevendo ele para que nao haja duplicação ou incongruência nos IDs
                    }
                    idproduto--; // Decrementa o total de produtos

                    // Reescrever o arquivo de produtos com IDs atualizados
                    FILE *arquivo = fopen("produtos.txt", "w");
                    if (arquivo != NULL)
                    {
                        for (int i = 0; i < idproduto; i++)
                        {
                            produto[i].id = i + 1; // Atualizar ID de cada produto
                            fprintf(arquivo, "%i|%s|%.2f\n", produto[i].id, produto[i].nome, produto[i].preco);
                        }
                        fclose(arquivo);
                        printf("\n\tProduto excluído e IDs atualizados com sucesso!\n");
                    }
                    else
                    {
                        printf("Erro ao abrir o arquivo de produtos.\n");
                    }
                }
                else
                {
                    printf("\tOperação de exclusão cancelada.\n");
                }
            }
            else
            {
                printf("\tErro ao ler a resposta. Tente novamente.\n");
            }
        }
        else
        {
            printf("\tProduto com ID %d não encontrado!\n", idProduto);
        }
    }
    else
    {
        printf("Erro ao ler a entrada.\n");
    }
}
void lerUsuarios() // #SPRINT 1 funcao que executa a leitura do banco de dados
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
            split = strtok(NULL, "|");           // agora split recebe o nome lido
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

int autentica() // #SPRINT 1 funcao que verifica se o nome de usuario ja esta cadastrado
{
    lerUsuarios();
    char confirmaNome[MAX_NOME];   // variavel de usuario
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

void cadastrarUsuario() // #SPRINT 1 funcao responsavel pelo cadastro de usuarios
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

void cadastrarVenda() // #SPRINT 3, função responsavel pelas vendas e pelo banco de dados das vendas
{
    float precokg, valorkg, x;  // variaveis de preços
    char entrada[100];          // buffer de entrada
    char pesokg[10];            // buffer de entrada
    char opcao[10];             // buffer de entrada
    char nomeproduto[MAX_NOME]; // atribuição do nome
    char desc[100];             // descrição da venda avulsa
    int id = -1;                // Inicialmente, nenhum ID
    int produtoIndex = -1;      // Índice do produto, se encontrado
    structvendas novavenda;     // um novo array de vendas para atribuir ao array principal
    structcliente clientes[MAX_LINHA];

    limpar_tela();

    printf("1. Vender produto\n");
    printf("2. Venda avulsa\n");
    printf("3. Voltar\n");

    if (fgets(opcao, sizeof(opcao), stdin) != NULL)
    {
        int escolha = atoi(opcao);
        switch (escolha)
        {
        case 1: // SPRINT 3 Vende produto

            menuProdutos(); // imprime relação de produtos na tela
            printf("\n\n\tInforme o id/nome do produto que deseja vender: ");
            if (fgets(entrada, sizeof(entrada), stdin) != NULL)
            {

                entrada[strcspn(entrada, "\n")] = '\0';
                remove_espacos(entrada);
                toLowerCase(entrada);

                if (strlen(entrada) < 1) // strlen() verifica se o tamanho da string entrada é maior que 1.
                {
                    printf("Produto invalido");
                    return;
                }
                limpar_tela();

                id = atoi(entrada); // atoi() transforma valor inserido em inteiro

                if (id > 0 && id <= MAX_VENDAS && produto[id - 1].preco > 0) // Se produto id/nome existe, atribui o id do produto a variavel de controle produtoIndex.
                {
                    produtoIndex = id - 1; // Encontrado pelo ID
                }
                else
                {

                    for (int i = 0; i < MAX_VENDAS; i++) // Se não encontrou pelo ID, procura pelo nome
                    {
                        if (strcmp(produto[i].nome, entrada) == 0 && produto[i].preco > 0) // strcmp(), compara duas strings sendo 0 TRUE e 1 FALSE
                        {
                            produtoIndex = i; // atribui o id encontrado pelo nome
                            break;
                        }
                    }
                }

                if (produtoIndex >= 0) // Verifica se o id do produto for valido
                {
                    strcpy(nomeproduto, produto[produtoIndex].nome); // strcpy(destino, origem)
                    precokg = produto[produtoIndex].preco;

                    printf("\n\t%s R$%.2f/KG.\n\t\n\tInsira a quantidade de KGs: ", nomeproduto, precokg);
                    if (fgets(pesokg, sizeof(pesokg), stdin) != NULL)
                    {
                        pesokg[strcspn(pesokg, "\n")] = '\0';
                        valorkg = strtof(pesokg, NULL); // strtof(var, NULL), transforma string em inteiro

                        x = valorkg * precokg; // resultado do peso, valor da venda

                        if (x > 0)
                        {
                            //////////
                            printf("\n\tValor a pagar: R$%.2f\n", x);
                            printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                            if (fgets(opcao, sizeof(opcao), stdin) != NULL) // lê a escolha do usuário
                            {
                                int escolha = atoi(opcao);

                                switch (escolha)
                                {
                                case 1:

                                    // forma de pagamento produto
                                    printf("\n\t1. Debito");
                                    printf("\n\t2. Credito");
                                    printf("\n\t3. Dinheiro");
                                    printf("\n\t4. Cancelar\n");
                                    printf("\t");
                                    if (fgets(opcao, sizeof(opcao), stdin) && x > 0)
                                    {
                                        char buffer[100];
                                        // lê o relatorio de vendas para ter o ID da nova venda avulsa
                                        FILE *relatorioVendas = fopen("relatorioVendas.txt", "r"); // abre o relatorio de vendas em modo 'READ'
                                        idvenda = 0;                                               // reseta a quantidade de vendas para refazer a leitura
                                        while (fgets(buffer, sizeof(buffer), relatorioVendas) != NULL)
                                        {
                                            idvenda++; // atribui a quantidade de vendas
                                        }
                                        fclose(relatorioVendas);

                                        time_t now;
                                        time(&now);

                                        struct tm *local = localtime(&now);
                                        escolha = atoi(opcao);

                                        char hora[80];
                                        strftime(hora, sizeof(hora), "%Y-%m-%d %H:%M:%S", local);

                                        switch (escolha)
                                        {
                                        case 1: // DEBITO, a mesma logica de vender um produto, apenas armazena a descrição ao nome.

                                            novavenda.id = idvenda;
                                            novavenda.preco = x;
                                            strcpy(novavenda.produto, nomeproduto);
                                            strcpy(novavenda.vendedor, user[idSessao].nome);
                                            strcpy(novavenda.horariovenda, hora);
                                            strcpy(novavenda.pagamento, debito);
                                            vendas[idvenda] = novavenda;

                                            FILE *relatorioVendas = fopen("relatorioVendas.txt", "a");
                                            limpar_tela();
                                            printf("\n\tResumo da venda a ser cadastrada:\n");
                                            printf("\tNome: %s\n", vendas[idvenda].produto);
                                            printf("\tPreco por kg: R$%.2f\n", vendas[idvenda].preco);
                                            printf("\tForma de pagamento: %s\n", vendas[idvenda].pagamento);
                                            printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");
                                            if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                                            {
                                                if (verificastringdigito(opcao))
                                                {
                                                    escolha = atoi(opcao);
                                                    switch (escolha)
                                                    {
                                                    case 1:
                                                        limpar_tela();

                                                        printf("\t\nNome do cliente: ");
                                                        char nomeCliente[50];
                                                        fgets(nomeCliente, sizeof(nomeCliente), stdin);
                                                        nomeCliente[strcspn(nomeCliente, "\n")] = '\0';

                                                        if (strlen(nomeCliente) > 1 && strlen(nomeCliente) < 50)
                                                        {
                                                            strcpy(clientes[idvenda].nome, nomeCliente);

                                                            printf("\n\tDigite o numero de telefone do cliente: ");
                                                            char phone[12];

                                                            if (fgets(phone, sizeof(phone), stdin) != NULL)
                                                            {
                                                                phone[strcspn(phone, "\n")] = '\0';

                                                                if (!contemDigitos(phone))
                                                                { // Verificação atualizada
                                                                    printf("\t\nInsira apenas digitos!");
                                                                    getch();
                                                                    return;
                                                                }

                                                                strcpy(clientes[idvenda].phone, phone);

                                                                printf("\t\nCliente cadastrado com sucesso");
                                                                printf("\n\tVenda concluida: ID:%i | R$%.2f | %s | %s | %s | %s | %s | %s\n",
                                                                       vendas[idvenda].id,
                                                                       vendas[idvenda].preco,
                                                                       vendas[idvenda].produto,
                                                                       vendas[idvenda].vendedor,
                                                                       vendas[idvenda].pagamento,
                                                                       vendas[idvenda].horariovenda,
                                                                       clientes[idvenda].nome,
                                                                       clientes[idvenda].phone);

                                                                fprintf(relatorioVendas, "ID:%i|R$%.2f|%s|%s|%s|%s|%s|%s\n",
                                                                        vendas[idvenda].id,
                                                                        vendas[idvenda].preco,
                                                                        vendas[idvenda].produto,
                                                                        vendas[idvenda].vendedor,
                                                                        vendas[idvenda].pagamento,
                                                                        vendas[idvenda].horariovenda,
                                                                        clientes[idvenda].nome,
                                                                        clientes[idvenda].phone);
                                                                fclose(relatorioVendas);
                                                                getch();
                                                            }
                                                            else
                                                            {
                                                                printf("Digite um número válido");
                                                            }
                                                        }
                                                        else
                                                        {
                                                            printf("/t/nInvalido");
                                                        }

                                                        return;
                                                        break;
                                                    case 2:
                                                        printf("Cancelando a compra");
                                                        return;
                                                        break;
                                                    default:
                                                        printf("Opcao invalida");
                                                        return;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                printf("Opcao invalida!");
                                                return;
                                            }

                                            break;

                                        case 2: // CREDITO, a mesma logica de vender um produto, apenas armazena a descrição ao nome.

                                            novavenda.id = idvenda;
                                            novavenda.preco = x;
                                            strcpy(novavenda.produto, nomeproduto);
                                            strcpy(novavenda.vendedor, user[idSessao].nome);
                                            strcpy(novavenda.horariovenda, hora);
                                            strcpy(novavenda.pagamento, credito);
                                            vendas[idvenda] = novavenda;

                                            relatorioVendas = fopen("relatorioVendas.txt", "a");
                                            limpar_tela();
                                            printf("\n\tResumo da venda a ser cadastrada:\n");
                                            printf("\tNome: %s\n", vendas[idvenda].produto);
                                            printf("\tPreco: R$%.2f\n", vendas[idvenda].preco);
                                            printf("\tForma de pagamento: %s\n", vendas[idvenda].pagamento);
                                            printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");
                                            if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                                            {
                                                if (verificastringdigito(opcao))
                                                {
                                                    escolha = atoi(opcao);
                                                    switch (escolha)
                                                    {
                                                    case 1:
                                                        limpar_tela();

                                                        printf("\t\nNome do cliente: ");
                                                        char nomeCliente[50];
                                                        fgets(nomeCliente, sizeof(nomeCliente), stdin);
                                                        nomeCliente[strcspn(nomeCliente, "\n")] = '\0';

                                                        if (strlen(nomeCliente) > 1 && strlen(nomeCliente) < 50)
                                                        {
                                                            strcpy(clientes[idvenda].nome, nomeCliente);

                                                            printf("\n\tDigite o numero de telefone do cliente: ");
                                                            char phone[12];

                                                            if (fgets(phone, sizeof(phone), stdin) != NULL)
                                                            {
                                                                phone[strcspn(phone, "\n")] = '\0';

                                                                if (!contemDigitos(phone))
                                                                { // Verificação atualizada
                                                                    printf("\t\nInsira apenas digitos!");
                                                                    getch();
                                                                    return;
                                                                }

                                                                strcpy(clientes[idvenda].phone, phone);

                                                                printf("\t\nCliente cadastrado com sucesso");
                                                                printf("\n\tVenda concluida: ID:%i | R$%.2f | %s | %s | %s | %s | %s | %s\n",
                                                                       vendas[idvenda].id,
                                                                       vendas[idvenda].preco,
                                                                       vendas[idvenda].produto,
                                                                       vendas[idvenda].vendedor,
                                                                       vendas[idvenda].pagamento,
                                                                       vendas[idvenda].horariovenda,
                                                                       clientes[idvenda].nome,
                                                                       clientes[idvenda].phone);

                                                                fprintf(relatorioVendas, "ID:%i|R$%.2f|%s|%s|%s|%s|%s|%s\n",
                                                                        vendas[idvenda].id,
                                                                        vendas[idvenda].preco,
                                                                        vendas[idvenda].produto,
                                                                        vendas[idvenda].vendedor,
                                                                        vendas[idvenda].pagamento,
                                                                        vendas[idvenda].horariovenda,
                                                                        clientes[idvenda].nome,
                                                                        clientes[idvenda].phone);
                                                                fclose(relatorioVendas);
                                                                getch();
                                                            }
                                                            else
                                                            {
                                                                printf("Digite um número válido");
                                                            }
                                                        }
                                                        else
                                                        {
                                                            printf("/t/nInvalido");
                                                        }

                                                        return;
                                                        break;
                                                    case 2:
                                                        printf("Cancelando a compra");
                                                        return;
                                                        break;
                                                    default:
                                                        printf("Opcao invalida");
                                                        return;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                printf("Opcao invalida!");
                                                return;
                                            }
                                            break;

                                        case 3: // DINHEIRO, a mesma logica de vender um produto, apenas armazena a descrição ao nome e mostra o troco na tela
                                            novavenda.id = idvenda;
                                            novavenda.preco = x;
                                            strcpy(novavenda.produto, nomeproduto);
                                            strcpy(novavenda.vendedor, user[idSessao].nome);
                                            strcpy(novavenda.horariovenda, hora);
                                            strcpy(novavenda.pagamento, dinheiro);
                                            vendas[idvenda] = novavenda;
                                            float usuariopaga, troco;
                                            limpar_tela();
                                            printf("Troco para: R$");
                                            scanf("%f", &usuariopaga);
                                            limpabuffer();

                                            if (usuariopaga < novavenda.preco)
                                            {
                                                printf("\nO valor para calcular o troco deve ser maior que o valor da venda!");
                                                getch();
                                                return;
                                            }
                                            else
                                            {
                                                troco = usuariopaga - novavenda.preco;

                                                FILE *relatorioVendas = fopen("relatorioVendas.txt", "a");
                                                limpar_tela();
                                                printf("\n\tResumo da venda a ser cadastrada:\n");

                                                printf("\tNome: %s\n", vendas[idvenda].produto);
                                                printf("\tPreco: R$%.2f\n", vendas[idvenda].preco);
                                                printf("\tForma de pagamento: %s\n", vendas[idvenda].pagamento);
                                                printf("\n\tTroco do cliente: R$%.2f\n", troco);
                                                printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");
                                                if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                                                {
                                                    if (verificastringdigito(opcao))
                                                    {
                                                        escolha = atoi(opcao);
                                                        switch (escolha)
                                                        {
                                                        case 1:
                                                            limpar_tela();
                                                            printf("\t\nNome do cliente: ");
                                                            char nomeCliente[50];
                                                            fgets(nomeCliente, sizeof(nomeCliente), stdin);
                                                            nomeCliente[strcspn(nomeCliente, "\n")] = '\0';

                                                            if (strlen(nomeCliente) > 1 && strlen(nomeCliente) < 50)
                                                            {
                                                                strcpy(clientes[idvenda].nome, nomeCliente);

                                                                printf("\n\tDigite o numero de telefone do cliente: ");
                                                                char phone[12];

                                                                if (fgets(phone, sizeof(phone), stdin) != NULL)
                                                                {
                                                                    phone[strcspn(phone, "\n")] = '\0';

                                                                    if (!contemDigitos(phone))
                                                                    { // Verificação atualizada
                                                                        printf("\t\nInsira apenas digitos!");
                                                                        getch();
                                                                        return;
                                                                    }

                                                                    strcpy(clientes[idvenda].phone, phone);

                                                                    printf("\t\nCliente cadastrado com sucesso");
                                                                    printf("\n\tVenda concluida: ID:%i | R$%.2f | %s | %s | %s | %s | %s | %s\n",
                                                                           vendas[idvenda].id,
                                                                           vendas[idvenda].preco,
                                                                           vendas[idvenda].produto,
                                                                           vendas[idvenda].vendedor,
                                                                           vendas[idvenda].pagamento,
                                                                           vendas[idvenda].horariovenda,
                                                                           clientes[idvenda].nome,
                                                                           clientes[idvenda].phone);

                                                                    fprintf(relatorioVendas, "ID:%i|R$%.2f|%s|%s|%s|%s|%s|%s\n",
                                                                            vendas[idvenda].id,
                                                                            vendas[idvenda].preco,
                                                                            vendas[idvenda].produto,
                                                                            vendas[idvenda].vendedor,
                                                                            vendas[idvenda].pagamento,
                                                                            vendas[idvenda].horariovenda,
                                                                            clientes[idvenda].nome,
                                                                            clientes[idvenda].phone);
                                                                    fclose(relatorioVendas);
                                                                    getch();
                                                                }
                                                                else
                                                                {
                                                                    printf("Digite um número válido");
                                                                }
                                                            }
                                                            else
                                                            {
                                                                printf("/t/nInvalido");
                                                            }

                                                            getch();
                                                            return;

                                                            break;
                                                        case 2:
                                                            printf("Cancelando a compra");
                                                            return;
                                                            break;
                                                        default:
                                                            printf("Opcao invalida");
                                                            return;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        printf("Opcao invalida!");
                                                        return;
                                                    }
                                                }
                                                else
                                                {
                                                    printf("Opcao invalida!");
                                                    return;
                                                }
                                            }
                                            break;
                                        default:
                                            printf("opcao invalida");
                                        }
                                    }
                                    break;
                                case 2:
                                    return;
                                    break;
                                }
                            }

                            //////////

                            else
                            {
                                printf("ERRO DE LEITURA\n");
                            }
                        }
                        else
                        {
                            perror("Invalido");
                        }
                    }
                    else
                    {
                        printf("ERRO DE LEITURA\n");
                    }
                }
                else
                {
                    printf("Produto inexistente ou inválido!\n");
                }
            }
            break;
        case 2:
            // SPRINT 3 - produtos avulsos
            limpar_tela();

            printf("\tInforme um breve resumo: ");
            fgets(desc, sizeof(desc), stdin); // insere a descrição da venda
            desc[strcspn(desc, "\n")] = '\0';
            toLowerCase(desc);
            if (strlen(desc) > 0)
            {
                printf("\n\tInforme o valor da venda: R$");
                fgets(entrada, sizeof(entrada), stdin); // buffer do valor da venda
                entrada[strcspn(entrada, "\n")] = '\0';

                if (contemLetras(entrada) == 1) // se o usuario inseriu um caractere no campo 'valor', o programa sera retornado
                {
                    printf("Apenas numeros!");
                    getch();
                    return;
                }
                else
                {                                    // SE NAO
                    float p = strtof(entrada, NULL); // converte a string entrada em float e atribui na variavel p
                    if (p <= 0)                      // se o valor for invalido
                    {
                        printf("Insira um valor valido!");
                        return;
                    }

                    strcpy(novavenda.produto, desc); // copia a descrição da venda para o nome do produto em novavenda

                    // opçõse de venda avulsa
                    printf("\n\t1. Debito");
                    printf("\n\t2. Credito");
                    printf("\n\t3. Dinheiro");
                    printf("\n\t4. Cancelar\n");
                    printf("\t");
                    if (fgets(opcao, sizeof(opcao), stdin) && p > 0)
                    {
                        char buffer[100];
                        // lê o relatorio de vendas para ter o ID da nova venda avulsa
                        FILE *relatorioVendas = fopen("relatorioVendas.txt", "r"); // abre o relatorio de vendas em modo 'READ'
                        idvenda = 0;                                               // reseta a quantidade de vendas para refazer a leitura
                        while (fgets(buffer, sizeof(buffer), relatorioVendas) != NULL)
                        {
                            idvenda++; // atribui a quantidade de vendas
                        }
                        fclose(relatorioVendas);

                        time_t now;
                        time(&now);

                        struct tm *local = localtime(&now);
                        escolha = atoi(opcao);

                        char hora[80];
                        strftime(hora, sizeof(hora), "%Y-%m-%d %H:%M:%S", local);

                        switch (escolha)
                        {
                        case 1: // DEBITO, a mesma logica de vender um produto, apenas armazena a descrição ao nome.

                            novavenda.id = idvenda;
                            novavenda.preco = p;
                            strcpy(novavenda.produto, desc);
                            strcpy(novavenda.vendedor, user[idSessao].nome);
                            strcpy(novavenda.horariovenda, hora);
                            strcpy(novavenda.pagamento, debito);
                            vendas[idvenda] = novavenda;

                            FILE *relatorioVendas = fopen("relatorioVendas.txt", "a");
                            limpar_tela();
                            printf("\n\tResumo da venda a ser cadastrada:\n");
                            printf("\tNome: %s\n", vendas[idvenda].produto);
                            printf("\tPreco por kg: R$%.2f\n", vendas[idvenda].preco);
                            printf("\tForma de pagamento: %s\n", vendas[idvenda].pagamento);
                            printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");
                            if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                            {
                                if (verificastringdigito(opcao))
                                {
                                    escolha = atoi(opcao);
                                    switch (escolha)
                                    {
                                    case 1:
                                        printf("Venda concluida: ID:%i | R$%.2f | %s | %s | %s | %s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].produto, vendas[idvenda].vendedor, vendas[idvenda].pagamento, vendas[idvenda].horariovenda);
                                        fprintf(relatorioVendas, "ID:%i|R$%.2f|%s|%s|%s|%s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].produto, vendas[idvenda].vendedor, vendas[idvenda].pagamento, vendas[idvenda].horariovenda);
                                        fclose(relatorioVendas);
                                        return;
                                        break;
                                    case 2:
                                        printf("Cancelando a compra");
                                        return;
                                        break;
                                    default:
                                        printf("Opcao invalida");
                                        return;
                                    }
                                }
                                else
                                {
                                    printf("Opcao invalida!");
                                    return;
                                }
                            }
                            else
                            {
                                printf("Opcao invalida!");
                                return;
                            }

                            break;

                        case 2: // CREDITO, a mesma logica de vender um produto, apenas armazena a descrição ao nome.

                            novavenda.id = idvenda;
                            novavenda.preco = p;
                            strcpy(novavenda.produto, desc);
                            strcpy(novavenda.vendedor, user[idSessao].nome);
                            strcpy(novavenda.horariovenda, hora);
                            strcpy(novavenda.pagamento, credito);
                            vendas[idvenda] = novavenda;

                            relatorioVendas = fopen("relatorioVendas.txt", "a");
                            limpar_tela();
                            printf("\n\tResumo da venda a ser cadastrada:\n");
                            printf("\tNome: %s\n", vendas[idvenda].produto);
                            printf("\tPreco: R$%.2f\n", vendas[idvenda].preco);
                            printf("\tForma de pagamento: %s\n", vendas[idvenda].pagamento);
                            printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");
                            if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                            {
                                if (verificastringdigito(opcao))
                                {
                                    escolha = atoi(opcao);
                                    switch (escolha)
                                    {
                                    case 1:
                                        printf("Venda concluida: ID:%i | R$%.2f | %s | %s | %s | %s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].produto, vendas[idvenda].vendedor, vendas[idvenda].pagamento, vendas[idvenda].horariovenda);
                                        fprintf(relatorioVendas, "ID:%i|R$%.2f|%s|%s|%s|%s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].produto, vendas[idvenda].vendedor, vendas[idvenda].pagamento, vendas[idvenda].horariovenda);
                                        fclose(relatorioVendas);
                                        return;
                                        break;
                                    case 2:
                                        printf("Cancelando a compra");
                                        return;
                                        break;
                                    default:
                                        printf("Opcao invalida");
                                        return;
                                    }
                                }
                                else
                                {
                                    printf("Opcao invalida!");
                                    return;
                                }
                            }
                            else
                            {
                                printf("Opcao invalida!");
                                return;
                            }
                            break;

                        case 3: // DINHEIRO, a mesma logica de vender um produto, apenas armazena a descrição ao nome e mostra o troco na tela
                            novavenda.id = idvenda;
                            novavenda.preco = p;
                            strcpy(novavenda.produto, desc);
                            strcpy(novavenda.vendedor, user[idSessao].nome);
                            strcpy(novavenda.horariovenda, hora);
                            strcpy(novavenda.pagamento, dinheiro);
                            vendas[idvenda] = novavenda;
                            float usuariopaga, troco;
                            limpar_tela();
                            printf("Troco para: R$");
                            scanf("%f", &usuariopaga);
                            limpabuffer();

                            if (usuariopaga < novavenda.preco)
                            {
                                printf("\nO valor para calcular o troco deve ser maior que o valor da venda!");
                                getch();
                                return;
                            }
                            else
                            {
                                troco = usuariopaga - novavenda.preco;

                                FILE *relatorioVendas = fopen("relatorioVendas.txt", "a");
                                limpar_tela();
                                printf("\n\tResumo da venda a ser cadastrada:\n");

                                printf("\tNome: %s\n", vendas[idvenda].produto);
                                printf("\tPreco: R$%.2f\n", vendas[idvenda].preco);
                                printf("\tForma de pagamento: %s\n", vendas[idvenda].pagamento);
                                printf("\n\tTroco do cliente: R$%.2f\n", troco);
                                printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");
                                if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                                {
                                    if (verificastringdigito(opcao))
                                    {
                                        escolha = atoi(opcao);
                                        switch (escolha)
                                        {
                                        case 1:
                                            printf("Venda concluida: ID:%i | R$%.2f | %s | %s | %s | %s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].produto, vendas[idvenda].vendedor, vendas[idvenda].pagamento, vendas[idvenda].horariovenda);
                                            fprintf(relatorioVendas, "ID:%i|R$%.2f|%s|%s|%s|%s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].produto, vendas[idvenda].vendedor, vendas[idvenda].pagamento, vendas[idvenda].horariovenda);
                                            fclose(relatorioVendas);
                                            return;
                                            break;
                                        case 2:
                                            printf("Cancelando a compra");
                                            return;
                                            break;
                                        default:
                                            printf("Opcao invalida");
                                            return;
                                        }
                                    }
                                    else
                                    {
                                        printf("Opcao invalida!");
                                        return;
                                    }
                                }
                                else
                                {
                                    printf("Opcao invalida!");
                                    return;
                                }
                            }
                            break;
                        default:
                            printf("opcao invalida");
                        }
                    }
                }
            }
            else
            {
                printf("Insira uma descricao!");
                return;
            }

            break;
        default:
            return;
        }
    }

    else
    {
        printf("ERRO DE LEITURA\n");
    }
}

void relatorioVendasCaixa() {
    limpar_tela();
    FILE *arquivo = fopen("relatorioVendas.txt", "r");
    if (arquivo == NULL) {
        perror("ARQUIVO VAZIO");
        return;
    }

    int i = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        if (strlen(buffer) > 1) {
            buffer[strcspn(buffer, "\n")] = '\0'; 

            // Substitui a vírgula por ponto no preço
            char *p = strstr(buffer, "R$");
            if (p != NULL) {
                char *comma = strchr(p, ',');
                if (comma != NULL) {
                    *comma = '.'; // Substitui a vírgula por ponto
                }
            }

            int camposLidos = sscanf(buffer, "ID:%i|R$%f|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
                                     &vendas[i].id,
                                     &vendas[i].preco,
                                     vendas[i].produto,
                                     vendas[i].vendedor,
                                     vendas[i].pagamento,
                                     vendas[i].horariovenda,
                                     clientes[i].nome,
                                     clientes[i].phone);

            if (camposLidos == 8) {
                if (strcmp(vendas[i].vendedor, user[idSessao].nome) == 0) {
                    i++;
                }
            } else {
                fprintf(stderr, "Erro ao ler a linha: %s (campos lidos: %d)\n", buffer, camposLidos);
            }
        }
    }

    fclose(arquivo);

    printf("\tCODIGO  -  VALOR  -  PRODUTO  -  VENDEDOR  -  PAGAMENTO  -  HORARIO | CLIENTE | TELEFONE\n");

    for (int j = 0; j < i; j++) {
        printf("\n\t    ID %i - R$%.2f - %s - %s - %s - %s | %s | %s\n",
               vendas[j].id,
               vendas[j].preco,
               vendas[j].produto,
               vendas[j].vendedor,
               vendas[j].pagamento,
               vendas[j].horariovenda,
               clientes[j].nome,
               clientes[j].phone);
    }
}

void relatorioVendas() // SPRINT 3, relatorio de todas as vendas, apenas o adm tem acesso
{
    limpar_tela();
    FILE *arquivo = fopen("relatorioVendas.txt", "r");
    if (arquivo == NULL)
    {
        perror("ARQUIVO VAZIO");
        return;
    }

    int i = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL)
    {
        // Verifica se a linha não está vazia antes de tentar ler
        if (strlen(buffer) > 1)
        {
            // Remove caracteres indesejados
            buffer[strcspn(buffer, "\n")] = '\0';

            // Substitui a vírgula por ponto no preço
            char *p = strstr(buffer, "R$"); //bucando a substring 'R$' dentro da string buffer
            if (p != NULL)
            {
                char *comma = strchr(p, ',');//busca a primeira ocorrencia de um caractere
                if (comma != NULL)
                {
                    *comma = '.'; // Substitui a vírgula por ponto
                }
            }

            // Lê os campos da linha
            int camposLidos = sscanf(buffer, "ID:%i|R$%f|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
                                     &vendas[i].id,
                                     &vendas[i].preco,
                                     vendas[i].produto,
                                     vendas[i].vendedor,
                                     vendas[i].pagamento,
                                     vendas[i].horariovenda,
                                     clientes[i].nome,
                                     clientes[i].phone);

            if (camposLidos == 8)
            {
                i++;
            }
            else
            {//debbugar eventuais erros de leitura
                fprintf(stderr, "Erro ao ler a linha: %s (campos lidos: %d)\n", buffer, camposLidos);
            }
        }
    }

    fclose(arquivo);

    printf("\tCODIGO  -  VALOR  -  PRODUTO  -  VENDEDOR  -  PAGAMENTO  -  HORARIO | CLIENTE | TELEFONE\n");

    for (int j = 0; j < i; j++)
    {
        // Exibindo o relatório de todas as vendas
        printf("\n\t    ID %i - R$%.2f - %s - %s - %s - %s | %s | %s\n",
               vendas[j].id,
               vendas[j].preco,
               vendas[j].produto,
               vendas[j].vendedor,
               vendas[j].pagamento,
               vendas[j].horariovenda,
               clientes[j].nome,
               clientes[j].phone);
    }
}

int obterOpcao() // #SPRINT 1 funcao responsavel por mostrar o menu
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
int menuADM() // #SPRINT 2, mostra o menu do administrador
{
    lerProduto();
    int opcaoADM = 0;
    char bufferADM[9];
    while (1)
    {
        limpar_tela();
        printf("Bem Vindo, %s, %s\n", user[idSessao].nome, user[idSessao].cargo);
        printf("\nMenu:\n");
        printf("1. Cadastrar/Alterar Produtos\n");
        printf("2. Efetuar Venda\n");
        printf("3. Relatorio de Vendas\n");
        printf("4. Sair\n");
        printf("5. Remover Produto\n");
        printf("6. Menu de Produtos\n");
        printf("Escolha uma opcao: ");

        if (fgets(bufferADM, sizeof(bufferADM), stdin))
        {
            if (sscanf(bufferADM, "%d", &opcaoADM) == 1 && opcaoADM >= 1 && opcaoADM <= 6)
            {
                switch (opcaoADM)
                {
                case 1:
                    limpar_tela();

                    cadastrarProduto();

                    break;
                case 2: // AQUI SERA FEITA A CHAMADA FUNÇÃO QUE IRA EFETUAR UMA VENDA
                    // limpabuffer();
                    printf("\tEfetuar Venda\n");
                    cadastrarVenda();
                    break;
                case 3: // AQUI SERA FEITA A CHAMADA DA FUNÇÃO QUE IRA IMPRIMIR O RELATORIO DE VENDAS GERAL
                    relatorioVendas();
                    getch();
                    break;
                case 4:
                    printf("Saindo...");
                    // getch();
                    limpar_tela();
                    return 0;
                    break;
                case 5:
                    limpar_tela();
                    menuProdutos();
                    excluirProduto();
                    break;
                case 6:
                    limpar_tela();
                    menuProdutos();
                    getch();
                    break;
                }
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

int menuCAIXA() // #SPRINT 2, mostra o menu do caixa
{
    int opcaoCAIXA = 0;
    char bufferCAIXA[9];
    while (1)
    {
        limpar_tela();
        printf("Bem Vindo, %s, %s\n", user[idSessao].nome, user[idSessao].cargo);
        printf("\nMenu:\n");
        printf("1. Efetuar Venda\n");
        printf("2. Consultar Precos\n");
        printf("3. Relatorio de vendas\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(bufferCAIXA, sizeof(bufferCAIXA), stdin))
        {
            if (sscanf(bufferCAIXA, "%d", &opcaoCAIXA) == 1 && opcaoCAIXA >= 1 && opcaoCAIXA <= 4)
            {
                switch (opcaoCAIXA)
                {
                case 1: // AQUI SERA FEITO A CHAMADA DA FUNÇÃO QUE IRA CADASTRAR UMA VENDA
                    printf("Efetuar Venda");
                    cadastrarVenda();
                    
                    break;
                case 2:
                    menuProdutos();
                    getch();
                    
                    break;
                case 3: // AQUI SERA FEITO A CHAMADA DA FUNÇÃO QUE IRA IMPRIMIR O RELATORIO DE VENDAS DO CAIXA

                    relatorioVendasCaixa();
                    getch();
                    break;
                case 4:
                    printf("Saindo...");
                    getch();
                    limpar_tela();
                    return 0;
                    break;
                }
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
    setlocale(LC_ALL, "pt_BR");
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
                    menuADM();
                    break;

                case 2: // se for caixa
                    printf("CAIXA");
                    menuCAIXA();
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
