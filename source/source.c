#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER1 24
#define NUMSTATI 1
#define INPUT_SIZE 1
#define MAXTRANSITION 1

//Autore: Gioele Mombelli

//---------------------------------------------//



long int maximum = 0;
int n_stati = 0;
int len_nastro = 0;
int accetta = 0;
struct macchina* testa;
struct macchina* start;

//---------STRUTTURE UTILIZZATE-----------//

struct transition{ //Rappresenta una transizione della MTND.

char letto;
char scritto;
char mossa;
int prossimo;

};

struct stato{ //Rappresenta uno stato della MTND con relative transizioni uscenti.

int accettazione;
int trans_uscenti;
struct transition* transizione; //array delle transizioni di uno stato

};

struct macchina{ //Rappresenta una MT deterministica

char* nastro;
int stato;
int size_nastro;
int testina;
long int level;
struct macchina* successiva;
struct macchina* precedente;

};


//--------------------------------------------------------//


struct stato* inizializza_struttura(int numero){ //Questa funzione crea un vettore di strutture che rappresentano ognuna uno stato della MT.

struct stato* array_stati = calloc(numero, sizeof(struct stato));
return array_stati;

};

int expand_buffer(int buffer){ //Funzione usata per espandere un buffer.

int ritorno = 0;
ritorno = buffer*2;
return ritorno;

}


void inizializza_transizioni(struct stato* stato, int old_size, int new_size){ //Inizializzo l'array di transizioni.

int i = 0;
for(i=old_size; i<new_size; i++ ){
stato[i].transizione = NULL;
}
return;
}

struct stato* estendi_stati(struct stato* old_array_stati, int old_size, int new_size){ //Aggiungo uno stato all'array di stati

int i = 0;
struct stato* new_array;

new_array = realloc(old_array_stati, sizeof(struct stato)*new_size);

for(i=old_size; i<new_size; i++ ){
new_array[i].accettazione = 0;
new_array[i].trans_uscenti = 0;
new_array[i].transizione = NULL;
}

return new_array;

}

struct transition* aggiungi_transizione(struct transition* old_array_trans, int new_size){ //Aggiungo una transizione all'array di transizioni

struct transition* new_array;

if(new_size == 1){
new_array = calloc(1, sizeof(struct transition));
return new_array;
}

new_array = realloc(old_array_trans, new_size*sizeof(struct transition));
return new_array;

}

long int get_maximum(){ //Ritorna il numero massimo di passi come intero.
    long int massimo = 0;
    scanf("%ld", &massimo);
    return massimo;
}



struct stato* build_machine(char* section, struct stato* stato){ //Gli passi un vettore di stati e questa funzione lo riempie, parsando la sezione tr.


char* transition = calloc(BUFFER1, sizeof(char)); //questa stringa ogni volta contiene cose tipo "0 a a R 1"
n_stati = NUMSTATI;

while(1){

(void)fgets(transition, BUFFER1, stdin);
int len = strlen(transition);
if(strcmp(transition, "acc\n")==0){break;}
if(transition[len-1]=='\n'){transition[len-1]='\0';}

char* token = strtok(transition, " ");
int id_stato = 0;
int sezione_trans = 0;
int id_transizione = 0;

while(token != NULL){

    if (sezione_trans==0){ //se ci entri, token è lo stato iniziale.
            id_stato = atoi(token);
            if(id_stato+1>n_stati){
                    stato = estendi_stati(stato, n_stati, id_stato+1);
                    n_stati = id_stato+1;
            }

            id_transizione = stato[id_stato].trans_uscenti;
            stato[id_stato].trans_uscenti++;

            stato[id_stato].transizione = aggiungi_transizione(stato[id_stato].transizione, stato[id_stato].trans_uscenti);
    }

    if (sezione_trans==1){ //se ci entri, token è carattere letto
            stato[id_stato].transizione[id_transizione].letto = *token;
            }

    if(sezione_trans==2){ //se ci entri, token è carattere scritto
            stato[id_stato].transizione[id_transizione].scritto = *token;

    }
    if (sezione_trans==3){ //se ci entri, token è il movimento della testina
            stato[id_stato].transizione[id_transizione].mossa = *token;


    }
    if (sezione_trans==4){ //se ci entri, token è lo stato di arrivo
        int next = atoi(token);
        stato[id_stato].transizione[id_transizione].prossimo = next;
        if(next>n_stati-1){
        stato = estendi_stati(stato, n_stati, next+1);
        n_stati = next+1;
        }
    }

    token=strtok(NULL, " ");
    sezione_trans++;
}

///////////////////


}

if(transition[3]=='\n'){transition[3]='\0';}
strcpy(section, transition);
free(transition);

return stato;

}

void get_acc(char* section, struct stato* stato){

int statoacc = 0;

while(1){

    scanf("%s*c", section);
    if(strcmp(section, "max")==0){break;}
    statoacc = atoi(section);
    stato[statoacc].accettazione = 1;
}

return;

}

struct stato* parser(struct stato* array_stati){

char *section = calloc(4, sizeof(char));


while(1){ //Chiama una funzione per ogni sezione del documento

(void)scanf("%s%*c", section);

    if (strcmp(section, "tr")==0){
    array_stati = build_machine(section, array_stati);}

    if (strcmp(section, "acc")==0){
    get_acc(section, array_stati);}

    if (strcmp(section, "max")==0){
    maximum = get_maximum();
    }

    if (strcmp(section, "run")==0){
    break;
    }

}

free(section);

return array_stati;

}

void dealloca_macchina(struct stato* stato){

int i=0;

for(i=0;i<n_stati;i++){

    free(stato[i].transizione);
}

free(stato);
return;

}


char* duplica_nastro(char* nastro, int size){

char* new_nastro = calloc(size, sizeof(char));
memcpy(new_nastro, nastro, size);
return new_nastro;

}

char* aggiungi_cella_dx(char*nastro, int size){

char* new_nastro = realloc(nastro, sizeof(char)*(size+1));
new_nastro[size] = '_';
return new_nastro;

}

char* aggiungi_cella_sx(char* nastro, int size){

char* new_nastro = calloc(size+1, sizeof(char));

memcpy(&new_nastro[1], nastro, sizeof(char)*size);
new_nastro[0] = '_';
free(nastro);
return new_nastro;

}

char* inizializza_nastro(int size){

int i = 0;
char* nastro = calloc(size, sizeof(char));

for(i=0; i<size; i++){

nastro[i] = '_';

}
return nastro;
}

char* get_input(char primo_char){

//Qui devi fare un'alocazione dinamica del nastro di memoria
int index = 0;
char letto = primo_char;
int size = INPUT_SIZE;
char* input = inizializza_nastro(size);

while (letto != '\n' && letto != EOF){

if (index < size){
input[index] = letto;
letto = getchar();
}

if(index == size){
size++;
input = realloc(input, sizeof(char)*(size));
input[index] = letto;
letto = getchar();
}

index++;

}
len_nastro = size;
return input;
}

void spegni_macchina(){


if(start->successiva==NULL && start->precedente==NULL){ //Ultima macchina rimasta!
    free(start->nastro);
    free(start);
    testa = NULL;
    return;
    }

if(start->successiva==NULL){
     start->precedente->successiva = NULL;
     free(start->nastro);
     free(start);
     return;
    }

if(start->precedente==NULL){ //Stai eliminando la prima. SPOSTA TESTA
    start->successiva->precedente = NULL;
    testa = start->successiva;
    free(start->nastro);
    free(start);
    return;
}

start->precedente->successiva = start->successiva;
start->successiva->precedente = start->precedente;
free(start->nastro);
free(start);
return;

}

void spegni_ogni_macchina(){ //Chiamata se becco 1.

struct macchina* next_free;

while(testa!=NULL){

    next_free = testa->successiva;
    free(testa->nastro);
    free(testa);
    testa = next_free;

}
start = NULL;
return;
}

void step_det(struct stato* stato, int i){
//Eseguo la macchina.

//Scrivo il carattere sul nastro.
start->nastro[start->testina] = stato[start->stato].transizione[i].scritto;
//Sposto la testina e nel caso rialloco il nastro
char mossa = stato[start->stato].transizione[i].mossa;
int new_testina = start->testina;

if(mossa=='L'){new_testina = new_testina-1;}
if(mossa=='R'){new_testina = new_testina+1;}

if(new_testina == -1){
    start->nastro = aggiungi_cella_sx(start->nastro, start->size_nastro);
    start->size_nastro = start->size_nastro+1;
    new_testina = 0;
}

if(new_testina == start->size_nastro){
    start->nastro = aggiungi_cella_dx(start->nastro, start->size_nastro);
    start->size_nastro = start->size_nastro+1;
}

start->testina = new_testina;

//Aggiorno lo stato
start->stato = stato[start->stato].transizione[i].prossimo;
//Aggiorno il livello
start->level = start->level+1;

return;

}

void step_nondet(struct stato* stato, int i){ //Lui dovrà spostare la testa perché aggiunge.

int corrente = start->stato;
long int level = start->level;
int testina = start->testina;
char* nastro = duplica_nastro(start->nastro, start->size_nastro);
int size_nastro = start->size_nastro;

char scritto = stato[corrente].transizione[i].scritto;
char mossa = stato[corrente].transizione[i].mossa;
int prossimo = stato[corrente].transizione[i].prossimo;

//Scrivo il carattere.
nastro[testina] = scritto;

//Sposto la testina, e nel caso rialloco.

if(mossa=='L'){testina=testina-1;}
if(mossa=='R'){testina=testina+1;}

if(testina==-1){

    nastro = aggiungi_cella_sx(nastro, size_nastro);
    size_nastro++;
    testina = 0;
}

if(testina==size_nastro){
    nastro = aggiungi_cella_dx(nastro, size_nastro);
    size_nastro++;
}

//Creo la macchina.

struct macchina* new_machine = calloc(1, sizeof(struct macchina));
new_machine->level = level+1;
new_machine->nastro = nastro;
new_machine->size_nastro = size_nastro;
new_machine->stato = prossimo;
new_machine->testina = testina;

//La aggiungo in testa.
new_machine->successiva = testa;
new_machine->precedente = NULL;
testa->precedente = new_machine;
testa = new_machine;

return;
}

int check_pozzo(struct stato* stato, int i){ //NEL CASO FERMA MACCHINA

int testina = start->testina;
char* nastro = start->nastro;
int corrente = start->stato;
int size_nastro = start->size_nastro;
char letto = nastro[testina];

int next = stato[corrente].transizione[i].prossimo;
char da_leggere = stato[corrente].transizione[i].letto;
char scritto = stato[corrente].transizione[i].scritto;
char mossa = stato[corrente].transizione[i].mossa;

//1: controllo che la macchina sta looppando all'infinito a sinistra.

if(next==corrente && testina==0 && letto==da_leggere && letto=='_' && mossa=='L'){
    spegni_macchina();
    return 1;
}

//2: Controllo che la macchina sta looppando all'infinito a destra.

if(next==corrente && testina==size_nastro && letto==da_leggere && letto == '_' && mossa=='R'){
    spegni_macchina();
    return 1;
}

//Controllo che la macchina sia ferma su uno stato

if(next==corrente && letto==scritto && mossa=='S'){
    spegni_macchina();
    return 1;
}


return 0;
}

void step(struct stato* stato){//data una configurazione di una MT, la esegue.

struct macchina* next_exec = start->successiva;

int corrente = start->stato;
int testina = start->testina;
char letto = start->nastro[testina];
int numtrans = stato[corrente].trans_uscenti;

//Controllo se è oltre il massimo, nel caso la spengo.
long int livello = start->level;

if(livello==maximum){
    accetta = 2;
    spegni_macchina();
    start=next_exec;
    return;
}

//Controllo se accettazione.

if(stato[corrente].accettazione==1){
    accetta = 1;
    spegni_ogni_macchina();
    return;
}


//Controllo determinismo e ricavo vettore di indici transizioni.

int i = 0;
int transdet;
int trans_possibili = 0;


for(i=0;i<numtrans;i++){
    if(letto==stato[corrente].transizione[i].letto){
            transdet=i;
            trans_possibili++;
    }
}

if(trans_possibili==1){ //Caso deterministico. Se non è pozzo, faccio fare il passo alla macchina.
    if(check_pozzo(stato, transdet)==1){accetta = 2; start = next_exec; return;}
    step_det(stato, (transdet));
}

i=0;
int j=0;

if(trans_possibili>1){ //Caso non deterministico.
    for(i=0; i<numtrans;i++){
        if(letto==stato[corrente].transizione[i].letto){
            j++;
            if(j==trans_possibili){step_det(stato, i);}
            else{step_nondet(stato, i);}
        }
    }
}

if(trans_possibili==0){ //La macchina si è bloccata. La spengo.
spegni_macchina();
}

start = next_exec;
return;

}




void loop_on_list(struct stato* stato){ //Funzione che esegue tutte le MT contenute in una lista, da "start" in poi.

while(start!=NULL){
    step(stato);
}

return;

}

void processa(struct stato* stato, char* input){ //Stampa se una stringa è accettata o no

struct macchina* first = calloc(1, sizeof(struct macchina));
first->level = 0;
first->nastro = input;
first->size_nastro = len_nastro;
first->testina = 0;
first->stato = 0;
first->successiva = NULL;
first->precedente = NULL;
testa = first;
start = first;

///INSERIRE QUI LOOP SULLA LISTA FINCHE' NON E' VUOTA

while(testa!=NULL){ //Finché la lista non è vuota...

    loop_on_list(stato);
    start = testa;

}


if(accetta==0){printf("0\n");}
if(accetta==1){printf("1\n");}
if(accetta==2){printf("U\n");}

return;
}

void input_loop(struct stato* stato){ //Ogni volta ricava una stringa di input e la passa alla funzione che la esegue

char letto = getchar();

while(letto != EOF){

char* input = get_input(letto);
accetta = 0;
processa(stato, input);
letto = getchar();
}

return;
}


int main(){

struct stato* stati_macchina = inizializza_struttura(NUMSTATI);
stati_macchina = parser(stati_macchina);
input_loop(stati_macchina);
dealloca_macchina(stati_macchina);
return 0;
}
