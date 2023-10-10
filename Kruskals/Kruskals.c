#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int READ_ADJ(int N_EDGES, int GRAFO[N_EDGES][3], char TXT[]){

    char C[7];
    int TO, WEIGHT = 1, FROM, ON_WEIGHT = 1;
    FILE *file;

    for (int i = 0; i < N_EDGES; i++){
        for (int j = 0; j < 3; j++){
            GRAFO[i][j] = 0;
        }
    }

    file = fopen(TXT, "r");
    fgets(C, 7, file);
    fgets(C, 7, file);

    for (int i = 0; i < 7; i++) {
        if (C[i]=='\n'&&i==4) {
            ON_WEIGHT = 0;
            break;
        }
    }

    file = fopen(TXT, "r");
    fgets(C, 4, file);
    int aux = 0;
    if (ON_WEIGHT){
        while (fscanf(file, "%d %d %d", &FROM, &TO, &WEIGHT) != EOF) {
            GRAFO[aux][0] = FROM;
            GRAFO[aux][1] = TO;
            GRAFO[aux][2] = WEIGHT;
            aux++;
        }
    }
    else{
        while (fscanf(file, "%d %d", &TO, &FROM) != EOF) {
            GRAFO[aux][0] = FROM;
            GRAFO[aux][1] = TO;
            aux++;
        }
    }
    fclose(file);
    return GRAFO[N_EDGES][3];
}

void READ_HEADER(int *N_NODES, int *N_EDGES, char TXT[]){

    FILE *file;
    file = fopen(TXT, "r");

    fscanf(file, "%d %d", N_NODES, N_EDGES);

    if (file == NULL) {
        printf("Erro em abrir o arquivo de entrada.\n");
    }
    fclose(file);
}
  
int comparator(const void* p1, const void* p2){ 
    const int(*x)[3] = p1; 
    const int(*y)[3] = p2; 
  
    return (*x)[2] - (*y)[2]; 
} 
  
void makeSet(int parent[], int rank[], int n){ 
    for (int i = 0; i < n; i++) { 
        parent[i] = i; 
        rank[i] = 0; 
    } 
} 
  
int findParent(int parent[], int component){ 
    if (parent[component] == component) 
        return component; 
  
    return parent[component] 
           = findParent(parent, parent[component]); 
} 
  
void unionSet(int u, int v, int parent[], int rank[], int n){ 
    u = findParent(parent, u); 
    v = findParent(parent, v); 
  
    if (rank[u] < rank[v]) { 
        parent[u] = v; 
    } 
    else if (rank[u] > rank[v]) { 
        parent[v] = u; 
    } 
    else { 
        parent[v] = u; 
  
        rank[u]++; 
    } 
} 
  
void kruskalAlgo(int n, int edge[n][3], char TXT[], int pS){ 
    qsort(edge, n, sizeof(edge[0]), comparator); 
  
    int parent[n]; 
    int rank[n]; 
  
    makeSet(parent, rank, n); 
  
    int minCost = 0; 
    FILE *file;
    file = fopen(TXT, "w");

    if (file == NULL) {
        printf("Erro em abrir o arquivo de saída de resultado.\n");
        return;
    }

    for (int i = 0; i < n; i++) { 
        int v1 = findParent(parent, edge[i][0]); 
        int v2 = findParent(parent, edge[i][1]); 
        int wt = edge[i][2]; 
  
        if (v1 != v2) { 
            unionSet(v1, v2, parent, rank, n); 
            minCost += wt;

            fprintf(file, "(%d -- %d) = %d\n", edge[i][0], edge[i][1], wt);

            if (pS){
                printf("(%d -- %d) = %d\n", edge[i][0], edge[i][1], wt); 
            }
        } 
    }  
    
    fprintf(file, "\ncusto mínimo: %d\n", minCost);
    if (pS){
        printf("\ncusto mínimo: %d\n", minCost); 
    }

	fclose(file);  

} 

void main(int argc, char **argv){
    
    FILE *file;
    int frist = 1;
    int last = 0;
    int pS = 0;
    int N_NODES, N_EDGES;
    char *in = malloc(sizeof(char));
    char *Out= malloc(sizeof(char));
    in = realloc(in, strlen("In.txt"));
    strcpy(in, "In.txt");
    Out = realloc(Out, strlen("Out.txt"));
    strcpy(Out, "Out.txt");

    for (int i = 0; i < argc; i++){
        if ( strcmp(argv[i], "-h") == 0){
        printf("\n\t%s\t\t%s\n\t%s\t%s\n\t%s\t%s\n\t%s\t\t%s\n\t%s\t%s\n\t%s\t%s\n\n\tAVISOS:\n\t\tTenha certeza que entrada esteja após o parâmetro.\n\t\tEXEMPLO: -o TXT.txt\n\n\t\tTenha certeza que não há arestas repetidas na entrada de dados.\n\n"
            ,"-h",            ": mostra o help"
            ,"-o <arquivo>",  ": redireciona a saida para o ‘‘arquivo’’"
            ,"-f <arquivo>",  ": indica o ‘‘arquivo’’ que contém o grafo de entrada"
            ,"-s",            ": mostra a solução (em ordem crescente)"
            ,"-i <int>",      ": vértice inicial (dependendo do algoritmo)"
            ,"-l <int>",      ": vértice final (dependendo do algoritmo)");

        }
        else if ( strcmp(argv[i], "-o") == 0){
            Out = realloc(Out, strlen(argv[i+1]));
            strcpy(Out, argv[i+1]);
            file = fopen(Out, "r");
            if (file == NULL) {
                printf("Erro em abrir o arquivo de saída.\n");
                return;
            }
            i++;
        }
        else if ( strcmp(argv[i], "-f") == 0){
            in = realloc(in, strlen(argv[i+1]));
            strcpy(in, argv[i+1]);
            file = fopen(in, "r");
            if (file == NULL) {
                printf("Erro em abrir o arquivo de entrada.\n");
                return;
            }
            i++;
        }
        else if ( strcmp(argv[i], "-s") == 0){
            pS = 1;
        }
        else if ( strcmp(argv[i], "-i") == 0){
            frist = atoi(argv[i+1]);
            i++;
        }
        else if ( strcmp(argv[i], "-l") == 0){
            last = atoi(argv[i+1]);
            i++;
        }
    }

    READ_HEADER(&N_NODES, &N_EDGES, in);
    int GRAFO[N_EDGES][3];
    READ_ADJ(N_EDGES, GRAFO, in);
    kruskalAlgo(N_EDGES, GRAFO, Out, pS); 
}