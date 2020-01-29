#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct csv_fields
{
    char                delim;
    unsigned short      fieldc;
    char**              fieldv;
    char*               file_name;
};
typedef struct csv_fields  CSV_fields;

CSV_fields* ch_cria_CSV_fields(char*, char);

char* ch_CSV_look_up(
    char* csv,
    char    delim,
    short   campo_chave,
    char* valor_chave,
    short   campo_alvo
);
CSV_fields* ch_free_CSV_fields(CSV_fields*);
void        ch_show_CSV_fields(CSV_fields*);


int main(int argc, char* argv)
{
    char* LSI_file_name = "qualidade.csv";
    char* valor;
    valor = ch_CSV_look_up(
        LSI_file_name,
        ';',
        2,
        "Israel",
        22
    );
    return 0;
};  // main()


CSV_fields* ch_cria_CSV_fields(char* LSI_file_name, char d)
{    // para scanf()
    char* mascara_padrao = "%[^;^\n]%c"; // um campo por vez
    char mascara[20];
    strcpy(mascara, mascara_padrao);
    mascara[3] = d; // troca o delimitador

    FILE* LSI_file;

    char  campo[256];
    char  delim1 = '?';
    int   n = 0;

    LSI_file = fopen(LSI_file_name, "r");
    if (LSI_file == NULL)
    {
        printf("nao abriu %s\n", LSI_file_name);
        perror("Erro:");
        return -1;
    }
    else
    {
        printf("abriu %s\n", LSI_file_name);
    };  // if()

    // trata o header
    short rec_count = 0;
    while (!feof(LSI_file))
    {
        n = fscanf(
            LSI_file,
            mascara,
            &campo[0], &delim1
        );
        rec_count += 1;
        if (delim1 != d) break;
    };  // while()
    LSI_file = freopen(LSI_file_name, "r", LSI_file);
    // o CSV em questao tem rec_count campos
    // prepara a struct{} que descreve os campos
    CSV_fields* pLSI = (CSV_fields*)malloc(sizeof(CSV_fields));
    pLSI->delim = d;
    pLSI->fieldc = rec_count; // obvio
    pLSI->file_name =
        (char*)malloc(sizeof(char) * (strlen(LSI_file_name) + 1));
    strcpy(pLSI->file_name, LSI_file_name);
    // agora cria os campos e preenche
    pLSI->fieldv = (char**)malloc(sizeof(char*) * rec_count);
    for (int rec_count = 0; rec_count < pLSI->fieldc; rec_count += 1)
        *((pLSI->fieldv) + rec_count) = NULL;
    for (int rec_count = 0; rec_count < pLSI->fieldc; rec_count += 1)
    {   // le o proximo campo
        n = fscanf(LSI_file, mascara, &campo[0], &delim1);
        *((pLSI->fieldv) + rec_count) = (char*)malloc(strlen(campo) + 1);
        strcpy(*((pLSI->fieldv) + rec_count), campo);
    };  // for()
    return pLSI;
};  // ch_cria_CSV_fields()


char* ch_CSV_look_up(
    char* arquivo, 
    char  delim, 
    short campo_chave, 
    char* valor_chave, 
    short campo_alvo)
{
    CSV_fields* csv = NULL;
    csv = ch_cria_CSV_fields(arquivo, delim);
    printf(
        "ch_CSV_look_up(%s)\n",
        arquivo);
    printf(
        "Procurando\n  valor do campo %d=[%s]\n",
        campo_alvo,
        *((csv->fieldv)+campo_alvo-1)
    );
    printf(
        "  onde o campo %d=[%s]. Delim:'%c'\n",
        campo_chave, valor_chave, delim);
    //ch_show_CSV_fields(csv);
    csv = ch_free_CSV_fields(csv);
    return NULL;
};   // ch_CSV_look_up()


CSV_fields* ch_free_CSV_fields(CSV_fields* csv)
{
    printf(
        "ch_free_CSV_fields(): Apagando struct de %s\n",
        csv->file_name
    );
    for (int i = 0; i < csv->fieldc; i += 1)
        free( *((csv->fieldv) + i));
    free(csv->fieldv);
    free(csv->file_name);
    free(csv);
    return NULL; // para invalidar o pointer
};  // ch_free_CSV_fields()


void        ch_show_CSV_fields(CSV_fields* csv)
{
    printf(
        "ch_show_CSV_fields(%s): %d campos, delimitador = '%c'\n",
        csv->file_name,
        csv->fieldc,
        csv->delim
    );
    for (int rec_count = 0; rec_count < csv->fieldc; rec_count += 1)
    {
        printf("Campo %d: [%s]\n",
            1 + rec_count,
            *((csv->fieldv) + rec_count));
    };  // for()
    return;
};  // ch_show_CSV_fields()

// https://www.clubedohardware.com.br/forums/topic/
// 1422433-manipula%C3%A7%C3%A3o-de-arquivos-csv/?
// tab=comments#comment-7683079


