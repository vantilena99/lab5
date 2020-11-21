//manca allitterazione di "nuova vision" + non fatto debugging su database file + check lunghezza è giusto??

#include <stdio.h>
#include <string.h>
#define MAX_LENGTH_VERSO 10
#define MAX_NUM_VERSI 2
#define MAX_DIFF 1.2 //max diff = 20%
#define RIMA 3 //decidiamo che sono in rima se gli ultimi 3 caratteri coincidono

typedef char riga[MAX_LENGTH_VERSO];
typedef riga versi[MAX_NUM_VERSI];

void printEsito(char* type, int esito) {
    if (esito == 1)
        printf("\n %s ok", type);
    else
        printf("\n %s non ok", type);
    return;
}

int checkLunghezza(char* s1, char* s2) {
    int length1, length2;
    length1 = strlen(s1);
    length2 = strlen(s2);
    if (length1 / length2 <= MAX_DIFF)
        return 1; //assumo 1 = true
    return 0;
}

void setValue(int length1, int length2, int * value1, int* value2) {

    if ( length1 >= RIMA)
        *value1 = length1 - RIMA;
    else
        *value1 = 0;
    if (length2 >= RIMA)
        *value2 = length2 - RIMA;
    else
        *value2 = 0;
    return;
}

int checkRima (char* s1, char* s2) {
    int length1, length2;
    int flag;
    int value1, value2;
    length1 = strlen(s1);
    length2 = strlen(s2);
    flag = 1;
    setValue(length1, length2, &value1, &value2);
    length1 --;
    length2 --;
    while ( (length1 >= value1) && (length2 >= value2) && (flag == 1))
    {
        if (s1[length1] != s2[length2])
            flag = 0;
        length1--;
        length2--;
    }
    return flag;
}

int cesura(char *s1, char* newString){ //non ottimizzata perché ricopia la stringa anche se non necessario
    int i,j;
    int flag;
    char temp[MAX_LENGTH_VERSO];

    flag = 0;
    j=0;
    for (i = 0; (*(s1 +i) != '\0') && (flag == 0); i++) {
        if(*(s1 + i) != '.') {
            temp[j]= *(s1 +i);
            j++;
        }
        else
            flag = 1;
    }

    if (flag == 1) {
        temp[j]= '\0';
        j = 0;
        while(temp[j] != '\0') {
            *(newString + j) = temp[j];
        }
        *(newString + j) = '\0';
        return 1;
    }
    return 0;
}

void findVowels(char * v1, char* v2, char s[]) {
    int i;
    int count;
    i = strlen(s) - 1;
    count = 0;
    while ((i >= 0) && (count <2)){
        if ((s[i] == 'a')||(s[i] == 'e')||(s[i] == 'i') || (s[i] == 'o')||(s[i] == 'u')) {
            if (count == 0) {
                *v1 = s[i];
            }
            else if (count == 1) {
                *v2 = s[i];
            }
            count++;
        }
        i++;
    }
    return;
}

int checkAssonanza(char s1[], char s2[]) {
    int i,j;
    char vow1, vow2, vow3, vow4;
    findVowels(&vow1, &vow2, s1);
    findVowels(&vow3, &vow4, s2);
    return ((vow1 == vow3) && (vow2 == vow4));
}

/*int checkAllitterazione (char s[], int dim) { //non ottimizzato
    int count;
    int i, j;
    int flag;

    i = 0;
    flag = 0;
    count = 1;

    while ((i < dim) && (flag == 0)) {
        for (j = i ; (j < dim) && s[j] != ' '; j++)
            ;
        if (s[j] == ' ') {
            j++;
            if (s[j] == s[i])
                count++;
        }
        if (count == ALLIT)
            flag = 1;
        else {
            count = 1;
            for (; (i < dim) && (s[i] != ' '); i++);
            if (s[i] == ' ')
                i++;
        }
    }
    return flag;
}*/

int checkAllitterazione(char s[], int dim) { //non mi viene in mente senza liste dinamiche, unica cosa mappare caratteri
                                             //su array di 21 posizioni

}

int main(int argc, char* argv[]) {

    versi poesia;
    int i;
    int comando;
    int verso1, verso2;
    int esito;
    char allitt;
    FILE* database;
    char newString[MAX_LENGTH_VERSO];
    char tempString[MAX_LENGTH_VERSO];

    printf("\n Inserisci poema: ");
    for (i = 0; i < MAX_NUM_VERSI; i++)
        scanf("%s", poesia[i]);

    printf("\n Cosa vuoi fare?");
    scanf("%d",&comando);
    while (comando != 8) {
        if (comando == 1) {
            scanf("%d",&verso1);
            scanf("%d",&verso2);
            esito = checkRima(poesia[verso1], poesia[verso2]);
            printEsito("Rima", esito);
        }
        else if (comando == 2) { //cercare nel db verso in rima con quello dato
            scanf("%d",&verso1);
            database = fopen("futuribili.txt", "r");
            esito = 0;
            while ((database) && (esito == 0)) {
                fgets(newString, MAX_LENGTH_VERSO, database);
                esito = checkRima(newString, poesia[verso1]);
            }
            if (esito == 1)
                printf("Ho trovato la stringa %s", newString);

            else
                printf("\n Nessuna stringa trovata");
        }
        else if (comando == 3) { //cercare verso nel database cui sia possibile applicare una cesura
            database = fopen("futuribili.txt", "r");
            esito = 0;
            while ((database) && (esito == 0)) {
                fgets(newString, MAX_LENGTH_VERSO, database);
                esito = cesura(newString, tempString);
            }
            if (esito == 1)
                printf("Ho trovato la stringa %s", newString);
            else
                printf("\n Nessuna stringa trovata");
        }
        else if (comando == 4) {
            scanf("%d",&verso1);
            scanf("%d",&verso2);
            esito = checkAssonanza(poesia[verso1], poesia[verso2]);
            printEsito("Assonanza", esito);
        }
        else if (comando == 5) { //verso nel db che abbia assonanza con quello dato
            database = fopen("futuribili.txt", "r");
            scanf("%d",&verso1);
            esito = 0;
            while ((database) && (esito == 0)) {
                fgets(newString, MAX_LENGTH_VERSO, database);
                esito = checkAssonanza(newString, poesia[verso1]);
            }
            if (esito == 1)
                printf("Ho trovato la stringa %s", newString);
            
            else
                printf("\n Nessuna stringa trovata");

        }
        else if (comando == 6) {
            scanf("%d",&verso1);
            esito = checkAllitterazione(poesia[verso1], MAX_LENGTH_VERSO);
            printEsito("Allitterazione", esito);
        }
        else if (comando ==7) //verso nel db con allitterazione con lettera nuova
            scanf("%c",&allitt);

        scanf("%d",&comando);
    }

}

