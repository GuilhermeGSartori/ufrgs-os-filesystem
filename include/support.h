#define M 20000//tamanho da hash...

//List structure for the resolution of conflicts in the HashTable
//Cada entrada vai estar atrelada a um hash, se tiver conflito, é inserido na de baixo...
//percorre hash até chegar no valor calculado, se não achou, arquivo não existe
typedef struct element
{
    char entry name[7];

}element;

//ponteiro para M elementos...

//Se Hash pegar as 5 primeiras letras do nome... e só der a posição, tipo, a única informação salva na hash
//são os 5 bytes iniciais do nome, daí se acha um que dá match, pega a entrada de posição i (índice iterando até M)
//e vê se o nome é de fato o mesmo, se não for, volta e continua procurando... (se o \0 veio antes do ultimo não...)
//vou ter que achar o equilibrio de espaço pra hash e entradas...
//13 pra M e 13 entradas... sobra espaço mas né...

//tá, na real, vamos fazer por setor! entradas num setor e hash dessas entradas
//2 setores o bloco?
//duplica ambos, simples...

//em 256, com 32 pra cada entrada
//no mundo sem hash, dá 8 entradas
//7 entradas
//256 - 64
//192 da 6 entradas, beleza
//com 64 que hash a gente faz?
//10 caracteres x 6(quantas entradas vai ter)
//da 60... top
//
//512 -> 12 entradas 
//512 - 12*32 = 128
//128 pra gaurdar a hash
//12*10 da 120... 8 bytes pro buraco...
//quando ficar 32 bytes no buraco, consigo fazer +1 entrada, daí
//basicamente isso, vou de 6 em 6 entradas e a hash vai acompanhando
//desperdício vai crescendo de 4 em 4....
//tudo isso que foi definido é salvo no setor 2 lá no format, e resgatado no boot...

//na real esquece tudo isso...
//vai ter mto mais entradas...
//não vai ser só 2...
//32 bytes uma entrada
//256/32 -> 8 entradas por bloco
//1/16 do de indice pra entradas pra blocos de entradas(bloco e quanto falta, 4 bytes) (2 unsigned short)
//16/4 = 4
//dá 4 entradas * 8 = 32 (arquivos)
//32 entradas... hash vai precisar documentar 32 entradas e tem 240 bytes
//240/32 = 7 bytes pra arquivo na hash
//7.5 * 32 = 240
//e ainda vai sobrar um pouco pra armazinar a quantidade total de arquivos ocupados
//e assim vai se mantendo, sempre 1/16
//

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

//HashTable definition
typedef struct{

    element* entries[M];

} HashTable;

//inicializar hash
    {
        ht.Elementos[l] = malloc(sizeof(element));
        strcpy(ht.Elementos[l]->key, "NULL");
        ht.Elementos[l]->Score = 0;
        ht.Elementos[l]->Ocorrencias = 0;
        ht.Elementos[l]->Next = NULL;
    }




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
}