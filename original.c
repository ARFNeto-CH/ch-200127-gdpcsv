#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

struct auxiliar {
    char aux1[25];
    char aux3[15];
    int aux2;
    char paises[30];
    char paisaux[30];
    float gdp_perc;
    float satisfacao;
};

struct gdpperc {
    int qntd;
    char pais[30];
    float gdp;
    float satisfacao_vidareal;
    float satisfcao_vidacalc;
    float erro;
};
void leArquivoum(struct auxiliar* auxs);
void leArquivodois(struct auxiliar* auxs);
void criacaodeumcsv(struct auxiliar* auxs);
void leArquivocsv(struct gdpperc* pib);
void criarcsvfinal(struct gdpperc* pib);

int main(int argc, char* argv)
{
    int qntda;
    int qntd;
    qntda = 189;
    struct auxiliar* auxs = malloc(qntda * sizeof(struct auxiliar));
    printf("informe a quantidade de paises que deseja usar: \n");
    scanf("%d", &qntd);
    struct gdpperc* pib = malloc(qntd * sizeof(struct gdpperc));
    pib->qntd = qntd;
    criacaodeumcsv(auxs);
    criarcsvfinal(pib);
    free(auxs);
    system("PAUSE");
    return 0;
}

void leArquivoum(struct auxiliar* auxs) {
    int i;
    FILE* arquivoum;
    arquivoum = fopen("C:\\Users\\John\\Desktop\\trabalho\\gdp.csv", "r");
    for (i = 0; i < 189; i++)
    {
        fscanf(arquivoum, "%[^;]", &auxs[i].paises);
        for (int j = 0; j < 4; j++) {
            fscanf(arquivoum, "%[^;]", &auxs[i].aux1);
        }
        fscanf(arquivoum, "%f[^;]", &auxs[i].gdp_perc);

    }
    fclose(arquivoum);
}

void leArquivodois(struct auxiliar* auxs) {

    int k;

    FILE* arquivo2;

    arquivo2 = fopen("C:\\Users\\John\\Desktop\\trabalho\\qualidade.csv", "r");
    for (k = 0; k < 41; k++)
    {
        for (int q = 0; q <= 1; q++)
        {
            fscanf(arquivo2, "%[^;]", &auxs[k].aux3);
        }
        fscanf(arquivo2, "%[^;]", &auxs[k].paisaux);
        for (int l = 0; l < 21; l++) {
            fscanf(arquivo2, "%d[^;]", auxs[k].aux2);
        }
        for (int i = 0; i < 189; i++) {
            while (strcmp(auxs[k].paisaux, auxs[i].paises) == 0);
            {
                fscanf(arquivo2, "%f[^;]", &auxs[i].satisfacao);
            }
        }

    }
    fclose(arquivo2);
}


void criacaodeumcsv(struct auxiliar* auxs) {
    int indice;
    indice = 189;
    FILE* arquivof;
    arquivof = fopen("C:\\Users\\John\\Desktop\\trabalho\\primeiroresult.csv", "w");
    fprintf(arquivof, "%d\n", &indice);
    for (int i = 0; i < 189; i++) {
        fprintf(arquivof, "%s,%f,%f\n", &auxs[i].paises, auxs[i].gdp_perc, auxs[i].satisfacao);
    }
}

void leArquivocsv(struct gdpperc* pib)/* função para a abertura do arquivo csv. */ {
    int i;
    FILE* arquivofinal;
    arquivofinal = fopen("C:\\Users\\John\\Desktop\\trabalho\\primeiroresult.csv", "r");
    for (i = 0; i < pib[i].qntd; i++)
    {

        fscanf(arquivofinal, "%d[^,\n]", &pib[i].qntd);
        fscanf(arquivofinal, "%[^,]", &pib[i].pais);
        fscanf(arquivofinal, "%f[^,]", &pib[i].gdp);
        fscanf(arquivofinal, "%f[^,\n]", &pib[i].satisfacao_vidareal);

    }
}
void criarcsvfinal(struct gdpperc* pib) {
    float teta0 = 2.5;
    float teta1 = 1;
    float n = 0.1;
    float eam;
    int i;
    FILE* arquivofinale;
    for (i = 0; i < pib->qntd; i++)
    {
        arquivofinale = fopen("C:\\Users\\John\\Desktop\\trabalho\\resultadofinal.csv", "w");
        if (pib[i].satisfacao_vidareal != 0) {
            for (int s = 0; s < 100; s++) {
                pib[i].satisfcao_vidacalc = teta1 + teta0 * pib[i].gdp;
                pib[i].erro = pib[i].satisfcao_vidacalc - pib[i].satisfacao_vidareal;
                teta0 = teta0 + n * pib[i].erro * pib[i].gdp;
                teta1 = teta1 + n * pib[i].erro * pib[i].gdp;
            }
        }
        eam = pib[i].satisfcao_vidacalc - pib[i].satisfacao_vidareal / pib[i].qntd;
    }
    for (i = 0; i < pib->qntd; i++) {
        fprintf(arquivofinale, "%s, %f, %f, %f, %f \n", &pib[i].pais, pib[i].gdp, pib[i].satisfcao_vidacalc, pib[i].satisfacao_vidareal, pib[i].erro);
    }
    fprintf(arquivofinale, "Erro absoluto Médio: %f", &eam);
    fclose(arquivofinale);
}


// https://www.clubedohardware.com.br/forums/topic/
// 1422433-manipula%C3%A7%C3%A3o-de-arquivos-csv/?
// tab=comments#comment-7683079
