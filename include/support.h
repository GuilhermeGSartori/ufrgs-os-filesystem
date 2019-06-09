#include <stdio.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/apidisk.h"


//a cada 16 linhas no disco fecha um setor!
//xxxx são 2 bytes em binário
        //alem disso tem que verificar se tem blocos suficientes para arquivo crescer...
        //quando for adicionar dados e etc... 

//diretório vai ser indexado na real, com hash no bloco de índice, mesma proporção
//8 (6, 2), só que os 6 vão ser ocupados com entradas de 32 ainda... vai ter uma entrada
//de 4 bytes pra dizer quantos livres ainda tem também
//ou 12 entradas de 16... mas o q elas tem? ponteiro pra bloco e quantidade de livres nele
//posso fazer 24 entradas de 8 então... 2 unsigned int (ponteiro e quantos tem livre)
//quando alocar aloca bloco de índice e logo já aloca "." e ".."
//.. na raiz troca de partição (reboot)
//bitmap em memória, mas sempre que fizer algo, so fazer um write com ele e gg
//tipo de arquivo define se é 0 = dir, 1 = txt 2 = bin
//se bin lê normal mas mete uns print em binário...

void four_bytes_to_sector_array(BYTE *sector, unsigned int info, int *iterator, int position);

int mbr_info(BYTE *mbr, unsigned int *version, unsigned int *sector_bytes, unsigned int *partition_table, unsigned int *partition_count);

void global_initialization(unsigned int *information);

void read_little_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator);

void read_big_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator);

int boot2();
/*
    int ComputeHash(char s[]) {
 	int hash=0;
 	int i;
    for(i = 0;i<strlen(s);i++){
        hash = (31*hash + s[i]) % 20000;
    }
    return hash;
}

//Inserts the new information in the HashTable, it verifies if the words already exists, to increment the occurrence and update the score, if the
//word is new but with the same hash from another word, to place it on the final position of a linked list from that hash value or if the word
//produces a new HashValue to put this new word in the empty address of the HashTable.
void insertHash(HashTable* ht, int score, char key[])
{
    int NewWordFlag = 0;
    double ScoreFim;

    int EndCalc = 0;

    EndCalc = ComputeHash(key);

    element* Temp;

    int EndLivre = EndCalc;
    Temp = ht->Elementos[EndLivre-1];

    if(strcmp(Temp->key, "NULL"))
    {
        do
        {

            if(!strcmp(Temp->key, key))//The word already exists in the HashTable
            {
                Temp->Ocorrencias += (double)1 ;
                (Temp->Score) += (double)score;
                NewWordFlag = 1;
            }

            else
            {
                if(Temp->Next != NULL)
                    Temp = Temp->Next;
            }

        }while(!NewWordFlag && Temp->Next != NULL);

        if(!NewWordFlag)//Old HashValue but new word
        {
            Temp->Next = malloc(sizeof(element));
            Temp->Next->Next = NULL;
            ScoreFim = (double)score/1;
            Temp->Next->Score = ScoreFim;
            strcpy(Temp->Next->key,key);
            Temp->Next->Ocorrencias = (double)1;
        }
    }

    else//New HashValue
    {
        ht->Elementos[EndLivre-1]->Score = (double)score;
        strcpy(ht->Elementos[EndLivre-1]->key,key);
        ht->Elementos[EndLivre-1]->Ocorrencias = (double)1;
    }
}




//Search a word in the HashTable using a similar algorithm to the insertion one.
double searchHash(HashTable* ht, char comentario[])
{
     double found=0;

    int EndCalc = ComputeHash(comentario);

    element* Temp;

    int EndLivre = EndCalc;
    Temp = ht->Elementos[EndLivre-1];
    int WordFoundFlag = 1;

    if(!(strcmp(Temp->key, "NULL")))
    {
    }

    else
    {
        do
        {
            if((!strcmp(Temp->key, comentario)))
            {
                found = Temp->Score/Temp->Ocorrencias;
                WordFoundFlag = 0;
            }
            else if(Temp->Next != NULL)
                Temp = Temp->Next;
        }while (Temp->Next != NULL && WordFoundFlag);
    }
    return found;
}*/