#include <stdio.h>

enum POS_SET
{
    SET,
    CUR,
    END
};

enum SIZE_LOGIC
{
    LOGIC,
    BYTE
};

// pega o arquivo, le, posiciona o cursor e retorna o tamanho
//@param ref arquivo aberto para leitura
//@param size Tamanho da estrutura
//@param dir: SET CUR e END Posicao final do ponteiro no arquivo
//@param tipo_retorno LOGIC ou BYTE: tamanho logico ou tamanho em bytes retornados
static inline int fsizer(FILE *ref, int size, POS_SET dir, SIZE_LOGIC tipo_retorno)
{
    fseek(ref, 0, SEEK_END);
    int contagem = ftell(ref);
    switch (dir)
    {
    case SET:
        fseek(ref, 0, SEEK_SET);
        break;
    case CUR:
        fseek(ref, 0, SEEK_CUR);
        break;
    case END:
        fseek(ref, 0, SEEK_END);
        break;
    }
    switch (tipo_retorno)
    {
    case LOGIC:
        contagem /= size;
        break;
    }
    return contagem;
}