#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Biblioteca necessaria para calcular o tempo de execucao

#define CAPACIDADE_MAX 10 // Limite da mochila estática

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
    int prioridade; // Novo campo
} Item;

// --- Protótipos das Funções ---
void adicionarItem(Item mochila[], int *total);
void removerItem(Item mochila[], int *total);
void listarItens(Item mochila[], int total);
void ordenarItens(Item mochila[], int total);
void buscaBinariaPorNome(Item mochila[], int total);
void limparBuffer();

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

int main() {
    Item mochila[CAPACIDADE_MAX]; // Nossa lista estática
    int total_itens = 0;          // Contador de itens atuais
    int opcao;

    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar itens
    // 5. Busca Binaria
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    do {
        // Limpa a tela (opcional, funciona bem no VS Code/Windows)
        // system("cls"); 

        printf("\n========================================\n");
        printf("MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
        printf("========================================\n");
        printf("Itens na Mochila: %d/%d\n\n", total_itens, CAPACIDADE_MAX);

        printf("1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens na Mochila\n");
        printf("4. Ordenar itens\n");
        printf("5. Busca Binaria\n");
        printf("0. Sair\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limparBuffer(); // Sempre limpar o buffer após ler número antes de ler texto

        switch (opcao) {
            case 1:
                adicionarItem(mochila, &total_itens);
                break;
            case 2:
                removerItem(mochila, &total_itens);
                break;
            case 3:
                listarItens(mochila, total_itens);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 4:
                ordenarItens(mochila, total_itens);
                break;
            case 5:
                buscaBinariaPorNome(mochila, total_itens);
                break;
            case 0:
                printf("\nSaindo do inventario. Boa sorte na ilha!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("\nPressione Enter para continuar...");
                getchar();
        }

    } while (opcao != 0);

    return 0;
}

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void adicionarItem(Item mochila[], int *total) {
    if (*total >= CAPACIDADE_MAX) {
        printf("\nSua mochila esta CHEIA! Remova algo antes de dar loot.\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    printf("\n--- Adicionar Novo Item ---\n");
    
    // Pegando o endereço do próximo espaço vazio na mochila
    Item *novoItem = &mochila[*total];

    printf("Nome do item: ");
    fgets(novoItem->nome, 50, stdin);
    novoItem->nome[strcspn(novoItem->nome, "\n")] = 0; // Tira o 'Enter' do final da string

    printf("Tipo do item (arma, municao, cura, etc.): ");
    fgets(novoItem->tipo, 30, stdin);
    novoItem->tipo[strcspn(novoItem->tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novoItem->quantidade);
    limparBuffer();

    printf("Prioridade (apenas numeros): ");
    scanf("%d", &novoItem->prioridade);
    limparBuffer();

    printf("\nItem '%s' adicionado com sucesso!\n\n", novoItem->nome);
    
    // Aumenta o contador de itens na mochila
    (*total)++;

    // Nas suas imagens, ele lista os itens logo após adicionar
    listarItens(mochila, *total);

    printf("\nPressione Enter para continuar...");
    getchar();
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem(Item mochila[], int *total) {
    if (*total == 0) {
        printf("\nNao ha nada para remover. A mochila ja esta vazia.\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    char nomeRemover[50];
    printf("\n--- Remover Item ---\n");
    printf("Digite o NOME EXATO do item que deseja jogar fora: ");
    fgets(nomeRemover, 50, stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = 0;

    int encontrado = -1; // -1 significa que ainda não achou

    // Busca linear para encontrar o item
    for (int i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // Lógica de Remoção em Lista Estática: Puxar os itens da frente para trás
        for (int i = encontrado; i < (*total) - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        
        (*total)--; // Diminui a quantidade total de itens
        printf("\nO item '%s' foi jogado fora!\n", nomeRemover);
    } else {
        printf("\nItem nao encontrado na mochila!\n");
    }

    printf("\nPressione Enter para continuar...");
    getchar();
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens(Item mochila[], int total) {
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n\n", total, CAPACIDADE_MAX);
    
    if (total == 0) {
        printf("Sua mochila esta vazia. Va procurar recursos!\n");
        return;
    }

    // Cabeçalho da tabela com espaçamento alinhado 
    printf("%-16s | %-16s | %-10s | %-10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < total; i++) {
        // %-16s alinha a string à esquerda ocupando 16 espaços
        printf("%-16s | %-16s | %-10d | %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("--------------------------------------------------------------\n");
}

// ordenarItens():
// Menu e implementação dos algoritmos de ordenação Bubble, Insertion e Selection Sort
void ordenarItens(Item mochila[], int total) {
    if (total <= 1) {
        printf("\nNao ha itens suficientes para ordenar.\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    int escolha;
    printf("\n--- ORDENAR ITENS ---\n");
    printf("1. Por Nome [Bubble Sort]\n");
    printf("2. Por Tipo [Insertion Sort]\n");
    printf("3. Por Prioridade [Selection Sort]\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);
    limparBuffer();

    int passadas = 0;
    clock_t inicio, fim;
    double tempo_gasto;

    if (escolha == 1) {
        // --- BUBBLE SORT (Por Nome - Ordem Alfabetica) ---
        inicio = clock();
        for (int i = 0; i < total - 1; i++) {
            for (int j = 0; j < total - i - 1; j++) {
                passadas++; // Conta cada comparacao feita
                if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                    Item temp = mochila[j];
                    mochila[j] = mochila[j + 1];
                    mochila[j + 1] = temp;
                }
            }
        }
        fim = clock();
        printf("\nMochila ordenada por NOME com sucesso!\n");

    } else if (escolha == 2) {
        // --- INSERTION SORT (Por Tipo - Ordem Alfabetica) ---
        inicio = clock();
        for (int i = 1; i < total; i++) {
            Item chave = mochila[i];
            int j = i - 1;
            
            while (j >= 0) {
                passadas++; // Conta cada comparacao no loop interno
                if (strcmp(mochila[j].tipo, chave.tipo) > 0) {
                    mochila[j + 1] = mochila[j];
                    j--;
                } else {
                    break;
                }
            }
            mochila[j + 1] = chave;
        }
        fim = clock();
        printf("\nMochila ordenada por TIPO com sucesso!\n");

    } else if (escolha == 3) {
        // --- SELECTION SORT (Por Prioridade - Maior para Menor) ---
        inicio = clock();
        for (int i = 0; i < total - 1; i++) {
            int min_idx = i;
            for (int j = i + 1; j < total; j++) {
                passadas++; // Conta cada comparacao feita
                // Maior numero = maior prioridade, entao ordenamos decrescente
                if (mochila[j].prioridade > mochila[min_idx].prioridade) { 
                    min_idx = j;
                }
            }
            Item temp = mochila[min_idx];
            mochila[min_idx] = mochila[i];
            mochila[i] = temp;
        }
        fim = clock();
        printf("\nMochila ordenada por PRIORIDADE com sucesso!\n");

    } else {
        printf("\nOpcao invalida!\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    // Calcula o tempo total em segundos
    tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("-> Desempenho do Algoritmo: %d passadas (comparacoes) realizadas.\n", passadas);
    printf("-> Tempo de execucao: %f segundos.\n", tempo_gasto);
    
    printf("\nPressione Enter para continuar...");
    getchar();
}

// Função para limpar o lixo do teclado (Enter) que quebra o fgets
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscaBinariaPorNome(Item mochila[], int total) {
    if (total == 0) {
        printf("\nMochila vazia! Nao ha o que buscar.\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    int confirmacao;
    printf("\n[AVISO IMPORTANTE]\n");
    printf("A Busca Binaria so funciona corretamente se os itens estiverem ordenados por NOME.\n");
    printf("Voce ja ordenou a mochila no menu de ordenacao?\n");
    printf("1 - Sim, ja ordenei por nome.\n");
    printf("0 - Nao, preciso voltar e ordenar.\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &confirmacao);
    limparBuffer();

    if (confirmacao != 1) {
        printf("\nBusca Binaria cancelada. Por favor, ordene os itens antes de tentar novamente!\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    char nomeBuscado[50];
    printf("\nDigite o nome do item que deseja buscar: ");
    fgets(nomeBuscado, 50, stdin);
    nomeBuscado[strcspn(nomeBuscado, "\n")] = 0;

    int inicio = 0;
    int fim = total - 1;
    int meio;
    int encontrado = -1;

    // Logica da Busca Binaria (divide a lista pela metade a cada passo)
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        int cmp = strcmp(mochila[meio].nome, nomeBuscado);

        if (cmp == 0) {
            encontrado = meio; // Achou!
            break;
        } else if (cmp < 0) {
            inicio = meio + 1; // Se vem depois alfabeticamente, descarta a metade esquerda
        } else {
            fim = meio - 1;    // Se vem antes alfabeticamente, descarta a metade direita
        }
    }

    if (encontrado != -1) {
        printf("\n--- Detalhes do Item (Encontrado via Busca Binaria) ---\n");
        printf("Nome: %s\n", mochila[encontrado].nome);
        printf("Estoque: %d\n", mochila[encontrado].quantidade);
        printf("Tipo: %s\n", mochila[encontrado].tipo);
        printf("Prioridade: %d\n", mochila[encontrado].prioridade);
    } else {
        printf("\nResultado: Item \"%s\" nao existe na mochila.\n", nomeBuscado);
    }

    printf("\nPressione Enter para continuar...");
    getchar();
}