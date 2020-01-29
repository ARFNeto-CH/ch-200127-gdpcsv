#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct registro_saida
{
    char* Pais;
    char* GDP;
    char* LS;
};
typedef struct registro_saida  OutRec;


int main(int argc, char* argv)
{
    // para scanf()
    char* mascara = "%[^;]%c%[^;]%c%[^;]%c%[^;]%c%[^;]%c%s%c";

    // o GDP.csv
    char* GDP_file_name = "gdp.csv";
    FILE* GDP_file;

    char  grade[8][80];
    char  delim1 = '?';
    int   n = 0;

    // para a saida
    char* CSV_output_file_name = "saida.csv";
    FILE* CSV_out;
    char* header_csv = "Pais;GDP;Life Satisfaction\r";

    OutRec* p = (OutRec*)malloc(sizeof(OutRec));

    GDP_file = fopen(GDP_file_name, "r");

    // trata o header
    if (GDP_file == NULL)
    {
        printf("nao abriu %s\n", GDP_file_name);
        perror("Erro:");
        return -1;
    }
    else
    {
        printf("abriu %s\n", GDP_file_name);
    };  // if()
    n = fscanf(GDP_file, mascara,
        &grade[0][0], &delim1,
        &grade[1][0], &delim1,
        &grade[2][0], &delim1,
        &grade[3][0], &delim1,
        &grade[4][0], &delim1,
        &grade[5][0], &delim1
    );
    printf("fscanf() retornou %d. Esperados 12\n", n);
    if (n != 12) exit(-1);
    printf("Eis os campos do CSV\n");
    for (int i = 0; i < (n / 2); i += 1)
        printf("Campo %d: [%s]\n", i, &grade[i][0]);

    //
    // agora le a entrada e cria uma struct para cada linha
    // e por enquanto vai gravando no csv de saida so pra
    // ver se o mecanismo esta ok
    //
    CSV_out = fopen(CSV_output_file_name, "w");
    if (CSV_out == NULL)
    {
        printf("nao abriu %s\n", CSV_output_file_name);
        perror("Erro:");
        return -1;
    };  // if()
    printf("abriu %s\n", CSV_output_file_name);
    fwrite(header_csv, strlen(header_csv), 1, CSV_out);
    char* LS_dummy = "A Calcular";
    int rec = 0;
    while (!feof(GDP_file))
    {
        n = fscanf(GDP_file, mascara,
            &grade[0][0], &delim1, // pais
            &grade[1][0], &delim1,
            &grade[2][0], &delim1,
            &grade[3][0], &delim1, 
            &grade[4][0], &delim1,
            &grade[5][0], &delim1 // GDP
        );
        if (n != 12) break;
        printf("Eis os campos do CSV\n");
        for (int i = 0; i < (n / 2); i += 1)
            printf("Campo %d: [%s]\n", i, &grade[i][0]);
        rec += 1;
        printf("Registro %d: fscanf() retornou %d. Esperados 12\n", rec, n);
        p->Pais = (char*) malloc( strlen(&grade[0][0]) + 1);
        strcpy(p->Pais, &grade[0][0]);
        p->GDP =  (char*) malloc( strlen(&grade[5][0]) + 1);
        strcpy(p->GDP, &grade[5][0]);
        p->LS =   (char*) malloc(strlen(LS_dummy) + 1);
        strcpy(p->LS, LS_dummy);
        fprintf(CSV_out, "%s;%s;%s\n", p->Pais, p->GDP, p->LS);
        
        free(p->Pais);
        free(p->GDP);
        free(p->LS);

    };  // while()
    printf("\nGRAVADOS %d registros\n", rec);
    free(p);
    fclose(GDP_file);
    fclose(CSV_out);
    return 0;
};  // main()

// https://www.clubedohardware.com.br/forums/topic/
// 1422433-manipula%C3%A7%C3%A3o-de-arquivos-csv/?
// tab=comments#comment-7683079
