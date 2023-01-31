#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"
#include <string.h>
#include <unistd.h>

static void stato(); //dichiaro variabile static che mi permette di sostituire ai valori dell'enum il corrispettivo colore
static void colore();
static void stampa_stanza();  //funzione che mi permette di stampare il tipo di stanza
static void stanza_random();  //funzione per randomizzare il tipo della stanza
static void avanza();       //funzione per far avanzare i giocatori
static void crea_stanza_avanti(); //crea in memoria una stanza in avanti rispetto alla stanza iniziale
static void crea_stanza_sinistra(); //crea in memoria una stanza a sinistra rispetto alla stanza iniziale
static void crea_stanza_destra(); //crea in memoria una stanza a destra rispetto alla stanza iniziale
static void esegui_quest();   //funzione per cambiare lo stato della stanza in vuota da parte degli astronauti
static void chiamata_emergenza();
static void uccidi_astronauta();  //funzione che cambia lo stato degli astronauti in assassinato e contiene anche probabilità per la defenestrazione
static void usa_botola();   //funzione che teletrasporta l'impostore in altre stanze
static void sabota();   ////funzione per cambiare lo stato della stanza in vuota da parte degli impostori
static void vittoria_astro();
static void vittoria_impos();

struct Giocatore *giocatori=NULL;
unsigned short quest_da_finire;               //dichiarazione variabili globali
struct Stanza *stanza_inizio;
struct Stanza *lista_stanze;

int numero_giocatori=0;      //variabile per far scegliere all'utente il numero dei giocatori
int conta_stanze=0;           //variabile che conta quante stanze sono state create

void imposta_gioco(){       //DEFINISCO LA FUNZIONE IMPOSTA GIOCO

time_t t; //variabile per la generazione di numeri casuali

printf("\t||====================================||\n");
printf("\t|| Sei nelle impostazioni del gioco!! ||\n");
printf("\t||====================================||\n\n");

do {                //creo un ciclo per evitare che l'utente inserisca un numero non adatto al gioco

  printf("Scegli il numero di giocatori da inserire (minimo 4 massimo 10):");
  scanf("%d", &numero_giocatori);           //inserimento numero giocatori
    while(getchar() != '\n');

    if(numero_giocatori<4 || numero_giocatori>10)     //condizione che, in caso di inserimento non idoneo mi faccia stampare un messaggio di errore
      printf("Non puoi inserire quel numero!\n");

}while(numero_giocatori<4 || numero_giocatori>10);    //una volta inserito un numero tra 4 e 10 si esce dal ciclo
  printf("\nPerfetto, hai impostato il numero dei giocatori a %d\n", numero_giocatori);

giocatori=(struct Giocatore*)malloc(numero_giocatori * sizeof(struct Giocatore));     //creo in memoria dinamica lo spazio per contenere i giocatori

 srand((unsigned)time(&t)); //inizializzo funzione numeri casuali usando il tempo



for(int i=0;  i<numero_giocatori; i++){         //creo un for per allocare in memoria i valori di ogni giocatore in modo casuale senza ripetizioni

    giocatori[i].colore=i;    //inizializzo tutti i personaggi in ordine
    giocatori[i].stato_giocatore=0;   //inizializzo tutti i personaggi astronauti

  }

  for(int i=0; i<numero_giocatori; i++){    //funzione per allocare in modo casuale i colori ai personaggi

    int t=giocatori[i].colore;
    int numeri_random = rand()%numero_giocatori;
    giocatori[i].colore = giocatori[numeri_random].colore;
    giocatori[numeri_random].colore = t;

  }

puts("");

//inizio inserimento tipo giocatore(astronauta o impostore)
int controllo=0;

 if(numero_giocatori>=4 && numero_giocatori<=6){      // nel caso in cui i giocatori siano tra 4 e 6
   for(int i=0; i<numero_giocatori; i++){
     int nrand=rand()%100;  //creo un numero casuale dentro un for
     if(nrand<15){   //se il numero uscito casualmente è minore di 25 aggiunge un impostore
       giocatori[i].stato_giocatore=1;
       ++controllo;
     }if(controllo==2){   //attraverso una variabile controllo se vengono creati più di 2 impostori
       break;
     }
    }
      if(controllo==0){             //attravrso la varibile controllo vedo se non è stato aggiunto nessun impostore in modo tale da aggiungerne 1
      giocatori[rand()%numero_giocatori].stato_giocatore=1;
      }
}else if(numero_giocatori>6 && numero_giocatori<=8){            //nel caso in cui i giocatori siano tra 7 e 8
      for(int i=0; i<numero_giocatori; i++){
        int nrand=rand()%100;               //creo numeri casuali dentro un for
        if(nrand<38){                //se il numero uscito casualmente è minore di 31 aggiunge un impostore (aumento di poco la probabilità all'aumentare dei giocatori)
          giocatori[i].stato_giocatore=1;
          ++controllo;
        }if(controllo==3){    //attraverso una variabile controllo se vengono creati più di 2 impostori
          break;
        }
      }if(controllo==0){       //attravrso la varibile controllo vedo se non è stato aggiunto nessun impostore in modo tale da aggiungerne 1
          giocatori[rand()%numero_giocatori].stato_giocatore=1;
          }
        }else if(numero_giocatori>=9 && numero_giocatori<=10){     //nel caso in cui i giocatori siano 9 o 10
          for(int i=0; i<numero_giocatori; i++){
            int nrand=rand()%100;
             if(nrand<42){
             giocatori[i].stato_giocatore=1;
             ++controllo;
           }if(controllo==3){    //attraverso una variabile controllo se vengono creati più di 3 impostori
             break;
          }
        }
      }if(controllo==0){
          giocatori[rand()%numero_giocatori].stato_giocatore=1;
            }

stanza_inizio=NULL;
stanza_inizio=(struct Stanza*)malloc(sizeof(struct Stanza));    //creo in memoria dinamica la prima stanza e inizializzo le direzioni a NULL
stanza_inizio -> avanti = NULL;
stanza_inizio -> sinistra = NULL;
stanza_inizio -> destra = NULL;
stanza_inizio -> stanza_precedente = NULL;
stanza_inizio -> prossima = NULL;

stanza_random(stanza_inizio);           //con questa funzione faccio in modo che la prima stanza abbia tipo randomico

lista_stanze=stanza_inizio;             //adesso lista stanze ha lo stesso indirizzo della stanza iniziale

do {

  printf("Inserisci numero di quest da completare:");   //chiedo di inserire numero quest
  scanf("%hd", &quest_da_finire);
  while(getchar() != '\n');

if(quest_da_finire<1 || quest_da_finire>4000){
  printf("Inserimento non valido! Inserire numero positivo\n");
  }
} while(quest_da_finire<1 || quest_da_finire>4000);

printf("\nAdesso puoi scegliere tra 2 alternative:\n");

static short scelta=0;

do{     //creo un ciclo che mi permette di riscrivere la scelta se inserisco 1 che mi fa stampare i giocatori
printf("1)Stampa giocatori;\n2)Inizia a giocare;\n");

scanf("%hd", &scelta);
while(getchar() != '\n');

switch (scelta) {
  case 1:
  printf("\nQuesta è la lista dei giocatori:\n\n");

for(int i=0; i<numero_giocatori; i++){          //con questo  ciclo stampo i giocatori da 1 a n

  printf("il giocatore %d è di colore ", i+1);
  colore(i);                                              //usando la funzione che converte l'enum in stringa
  printf(" ed è ");
  stato(i);                                               //e la funzione che mi converte l'enum dello stato
  puts("");

}
  printf("\n\n");
    break;
  case 2:         //in questo modo entro direttamente nella funzione gioca senza passare per il menù
    gioca();
    break;
  default:
    printf("\nInserire numero valido\n\n");
    break;

  }
}while(scelta==1 || scelta < 1 || scelta > 2);



}//FINE IMPOSTA GIOCO

void gioca(){

system("clear");

printf("████████ ██████   █████  ███    ██ ███    ██  ██████  ██ \n");
printf("   ██    ██   ██ ██   ██ ████   ██ ████   ██ ██    ██ ██ \n");
printf("   ██    ██████  ███████ ██ ██  ██ ██ ██  ██ ██    ██ ██ \n");
printf("   ██    ██   ██ ██   ██ ██  ██ ██ ██  ██ ██ ██    ██ ██ \n");
printf("   ██    ██   ██ ██   ██ ██   ████ ██   ████  ██████  ██ \n\n\n");

printf("Il gioco prevede un gruppo di giocatori in un'ambientazione a tema spaziale, ognuno dei quali assume uno dei due ruoli:\n");
printf("astronauti: l'obiettivo è identificare gli impostori ed eliminarli durante il completamento degli incarichi sulla mappa;\n");
printf("impostori: l'obiettivo degli impostori è eliminare tutti gli astronauti senza essere identificati.\n");
printf("Buon divertimento!\n");

printf("Premi invio per cominciare il gioco:");
getchar();

system("clear");

printf("I giocatori si trovano nella stanza %p che contiene ", stanza_inizio);        //stampo in esadecimale l'indirizzo della stanza
  stampa_stanza(stanza_inizio);                                                                  //converto l'enum in stringa

printf("i giocatori sono:\n");

for(int i=0; i<numero_giocatori; i++){        //metto la posizione dei giocatori nella prima stanza

  printf("-");
  giocatori[i].posizione=stanza_inizio;       //ristampo i giocatori
  colore(i);
  puts("");

}

int a=0;    //variabile per il while

while(a<1){   //ciclo infinito per ripetere i turni

for(int i=0; i<numero_giocatori; i++){       //ciclo che mi crea i turni



  if(giocatori[i].stato_giocatore==0){      //condizione nel caso il giocatore sia un astronauta
    printf("\nE' il turno del giocatore:  ");
    colore(i);

    printf("\nSei un astronauta!\n");
    printf("Ti trovi nella stanza %p di tipo ", giocatori[i].posizione);
    stampa_stanza(giocatori[i].posizione);


    printf("i giocatori nella stanza sono:\n");

int presenza=0;     //flag che mi controlla se ci sono giocatori dentro la stanza

    for(int c=0; c<numero_giocatori; c++){    //controlla gli altri giocatori


    if(giocatori[i].posizione == giocatori[c].posizione && giocatori[i].colore != giocatori[c].colore){ //filtro i giocatori che hanno un colore diverso nella stessa stanza

      colore(c);
      presenza++;   //incremento per controllo se ci sono giocatori
      if(giocatori[c].stato_giocatore==2 || giocatori[c].stato_giocatore==3){     //così gli astronauti possono vedere lo stato dei giocatori morti e defenestrati
        printf(" (");
        stato(c);
        printf(")");
      }
      puts("");

    }

    }
    if(presenza==0){    //se non ci sono giocatori nella stanza
      printf("Non ci sono giocatori nella stanza\n");
    }


    printf("\n");
    printf("\nScegli tra le seguenti azioni:\n");
    printf("1)Avanza\n2)Esegui quest\n3)Chiamata d'emergenza\n");

    int scelta=0;         //serve per scegliere tra le azioni principali


do{
    scanf("%d", &scelta);
      while(getchar() != '\n');

    switch(scelta){
      case 1:
        avanza(i);
      break;
      case 2:

        esegui_quest(i, a);   //passo i due parametri

          if(quest_da_finire==0){   //se le quest finiscono finisce anche il gioco

            vittoria_astro();

            ++a;    //pr uscire dal while
            i=numero_giocatori; //finisco il ciclo for
            }

            break;
      case 3:
        chiamata_emergenza(i);
      break;
      default:
        printf("Inserimento errato\n");
        break;
      }
    }while(scelta < 1 || scelta > 3);

  }

  if(giocatori[i].stato_giocatore==1){      //condizione nel caso il giocatore sia un impostore

    printf("\nE' il turno del giocatore:  "); //stampo il colore del giocatore
    colore(i);

    printf("\nSei un impostore!\n");
    printf("Ti trovi nella stanza %p di tipo ", giocatori[i].posizione);    //scopre di essere impostore e mostro l'indirizzo della stana e il tipo
    stampa_stanza(giocatori[i].posizione);
    printf("\n");

printf("i giocatori nella stanza sono:\n");

int presenza=0;   //stesso controllo degli astronauti

for(int c=0; c<numero_giocatori; c++){


if(giocatori[i].posizione == giocatori[c].posizione && giocatori[i].colore != giocatori[c].colore){ //filtro i giocatori nella stanza di colore diverso per stamparli a video

  colore(c);
  printf(" (");
  stato(c);
  printf(")\n");
  ++presenza;
  }

}

if(presenza==0){
  printf("Non ci sono giocatori nella stanza\n");
}

    printf("\n\nScegli tra le seguenti azioni:\n");
    printf("1)Avanza\n2)Uccidi astronauta\n3)Chiamata d'emergenza\n4)Usa botola\n5)sabota\n");

    int scelta=0;   //per lo switch

 int conta_botole=0;   //LO USO PER SAPERE QUANTE STANZA BOTOLA CI SONO

 int contatore=0;

struct Stanza *stanze_botola=NULL;
stanze_botola=(struct Stanza*)malloc(conta_botole * sizeof(struct Stanza)); //alloco memoria per contenere le stanze di tipo botola

do{
    scanf("%d", &scelta);
      while(getchar() != '\n');

    switch(scelta){
      case 1:
        avanza(i);
        break;
      case 2:
        uccidi_astronauta(i);
        break;
      case 3:
        chiamata_emergenza(i);
        break;
      case 4:
        usa_botola(i, stanze_botola, contatore, conta_botole);
        break;
      case 5:
        sabota(i);
        break;
        default:
          printf("Inserimento errato\n");
          break;
      }

    }while(scelta < 1 || scelta > 5);
    }
    int flag_astro=0;
    int flag_impo=0;

for(int c=0; c<numero_giocatori; c++){    //controlla se ci sono astronauti a fine turno
  if(giocatori[c].stato_giocatore == 0){
    ++flag_astro;
  }
}

if(flag_astro==0){    //se non ci sono più astronauti vincono gli impostori

  vittoria_impos();

  a++;
  i=numero_giocatori;
}

for(int c=0; c<numero_giocatori; c++){    //controla se ci sono impostori a fine turno
  if(giocatori[c].stato_giocatore == 1){
    ++flag_impo;
  }
}

if(flag_impo==0){   //se non ci sono più impostori vincono gli astronauti

  vittoria_astro();

  a++;
  i=numero_giocatori;
}
  }
}


}
void termina_gioco(){

printf("Grazie per aver giocato, Arrivederci!\n");
free(giocatori);

struct Stanza *appoggio=0;
while(lista_stanze != NULL){
  appoggio=lista_stanze;
  lista_stanze=lista_stanze -> prossima;
  free(appoggio);
}

}



static void stato(int i){        //definisco il corpo della funzione a static in modo da essere visibile solo nel gamelib.c

    switch(giocatori[i].stato_giocatore){
      //sostituisco al valore dell'enum una stringa di testo
      case 0: printf("astronauta");
        break;
      case 1: printf("impostore");
        break;
      case 2: printf("Assassinato");
        break;
      case 3: printf("Defenestrato");
      }
}

static void colore(int i){


  switch(giocatori[i].colore){        //switch per sostituire a alle cifre dell'enum il corrispettivo colore

    case 0:  printf("rosso");
      break;
    case 1:  printf("blu");
      break;
    case 2:  printf("verde");
      break;
    case 3:  printf("rosa");
      break;
    case 4:  printf("arancione");
      break;
    case 5:  printf("giallo");
      break;
    case 6:  printf("nero");
      break;
    case 7:  printf("bianco");
      break;
    case 8:  printf("viola");
      break;
    case 9:  printf("marrone");
      break;
    }


}


static void stampa_stanza(struct Stanza *stanza){
  //funzione che sostituisce al valore dell'enum la stringa di testo corrispettiva
  if(stanza -> tipo_stanza==0){
    printf("Stanza vuota\n");
  }else if(stanza -> tipo_stanza==1){
    printf("Quest semplice\n");
  }else if(stanza -> tipo_stanza==2){
    printf("Quest doppia\n");
  }else if(stanza -> tipo_stanza==3){
    printf("Botola\n");
  }
}


static void stanza_random(struct Stanza *stanza){

int nrand = rand()%100;               //creo numero randomico per il tipo della stanza iniziale
  if(nrand<25){
    stanza->tipo_stanza=3;
    }else if(nrand>24 && nrand<40){
      stanza->tipo_stanza=2;
      }else if(nrand>39 && nrand<70){
        stanza->tipo_stanza=1;
        }else if(nrand>69 && nrand<100){
          stanza->tipo_stanza=0;
          }
}

static void avanza(int i){

int direzione=0;      //serve per scegliere la direzione in cui spostarsi

  printf("\nScegli dove vuoi andare:\n");
  printf("-1 ti permette di andare avanti\n");
  printf("-2 ti permette di andare a sinistra\n");
  printf("-3 ti permette di andare a destra\n");
  printf("-4 ti fa rimanere nella stessa stanza\n");

do{
  scanf("%d", &direzione);
    while(getchar() != '\n');

  switch(direzione){
   //Avanti
    case 1:

      if(giocatori[i].posizione -> avanti == NULL){       //se la stanza successiva non esiste

      crea_stanza_avanti(i);

    do{

      if(lista_stanze -> prossima == NULL){                       //se prossima è vuoto
        lista_stanze -> prossima = giocatori[i].posizione;        //assegno all'indirizzo di prossima la nuova stanza usando la posizione del giocatore

        lista_stanze=NULL;                                        //serve per poter uscire dal while

        ++conta_stanze;
      }
      else{
        lista_stanze=lista_stanze -> prossima;                    //scorre le stanze create fino a diventare null e uscire dal ciclo

      }

    }while (lista_stanze != NULL);        //quando si verifica l'if esce dal while

    lista_stanze=stanza_inizio;           //lista stanze ha di nuovo l'indirizzo della prima stanza


      }else{            //non creo nessun nuovo spazio in memoria ma sposto solo il giocatore

        giocatori[i].posizione=giocatori[i].posizione->avanti;

        }

          printf("\n%p indirizzo del giocatore\n", giocatori[i].posizione);
            stampa_stanza(giocatori[i].posizione);

            break;
    //Sinistra
    case 2:

      if(giocatori[i].posizione -> sinistra == NULL){

      crea_stanza_sinistra(i);

          do{

            if(lista_stanze -> prossima == NULL){                       //se prossima è vuoto
              lista_stanze -> prossima = giocatori[i].posizione;        //assegno all'indirizzo di prossima la nuova stanza usando la posizione del giocatore

              lista_stanze=NULL;                                        //serve per poter uscire dal while

              ++conta_stanze;     //contatore per leggere il numero di stanza create
            }
            else{
              lista_stanze=lista_stanze -> prossima;        //scorrimento stanze

            }

          }while (lista_stanze != NULL);        //quando si verifica l'if esce dal while

          lista_stanze=stanza_inizio;


      }else{

        giocatori[i].posizione = giocatori[i].posizione -> sinistra;

        }

        printf("\n%p indirizzo della stanza \n", giocatori[i].posizione);
        stampa_stanza(giocatori[i].posizione);

        break;
    //Destra
    case 3:

      if(giocatori[i].posizione -> destra == NULL){

        crea_stanza_destra(i);

            do{

              if(lista_stanze -> prossima == NULL){                       //se prossima è vuoto
                lista_stanze -> prossima = giocatori[i].posizione;        //assegno all'indirizzo di prossima la nuova stanza usando la posizione del giocatore

                lista_stanze=NULL;                                        //serve per poter uscire dal while

                ++conta_stanze;
              }
              else{
                lista_stanze=lista_stanze -> prossima;

              }

            }while (lista_stanze != NULL);        //quando si verifica l'if esce dal while

            lista_stanze=stanza_inizio;

      }else{

        giocatori[i].posizione = giocatori[i].posizione -> destra;

      }

      printf("\n%p indirizzo del giocatore\n", giocatori[i].posizione);
      stampa_stanza(giocatori[i].posizione);

      break;
    //Fermo
    case 4:
      break;
      default:
      printf("Inserisci numero valido\n");
  }

}while(direzione < 1 || direzione > 4);

}

static void crea_stanza_avanti(int i){

struct Stanza *nuova_stanza=NULL;

nuova_stanza=(struct Stanza*)malloc(sizeof(struct Stanza));

nuova_stanza -> avanti=NULL;
nuova_stanza -> sinistra=NULL;
nuova_stanza -> destra=NULL;
nuova_stanza -> stanza_precedente=NULL;
nuova_stanza -> prossima=NULL;

stanza_random(nuova_stanza);      //definisco il tipo della stanza


  nuova_stanza->stanza_precedente=giocatori[i].posizione;       //alla stanza precedente della nuova stnza inserisco la posizione dei giocatori che sarebbe la stanza iniziale
  giocatori[i].posizione->avanti=nuova_stanza;  //al puntatore in avanti metto la nuova stanza
  giocatori[i].posizione=nuova_stanza; //adesso la posizione del giocatore è nella nuova stanza

}

static void crea_stanza_sinistra(int i){

struct Stanza *nuova_stanza=NULL;

nuova_stanza=(struct Stanza*)malloc(sizeof(struct Stanza));

nuova_stanza -> avanti=NULL;
nuova_stanza -> sinistra=NULL;
nuova_stanza -> destra=NULL;
nuova_stanza -> stanza_precedente=NULL;
nuova_stanza -> prossima=NULL;

stanza_random(nuova_stanza);

nuova_stanza -> stanza_precedente = giocatori[i].posizione;
giocatori[i].posizione -> sinistra = nuova_stanza;
giocatori[i].posizione = nuova_stanza;

}

static void crea_stanza_destra(int i){

  struct Stanza *nuova_stanza=NULL;

  nuova_stanza=(struct Stanza*)malloc(sizeof(struct Stanza));

  nuova_stanza -> avanti = NULL;
  nuova_stanza -> sinistra = NULL;
  nuova_stanza -> destra = NULL;
  nuova_stanza -> stanza_precedente = NULL;
  nuova_stanza -> prossima = NULL;

  stanza_random(nuova_stanza);

  nuova_stanza -> stanza_precedente = giocatori[i].posizione;
  giocatori[i].posizione -> destra = nuova_stanza;
  giocatori[i].posizione = nuova_stanza;


}


static void esegui_quest(int i, int a){

  if(giocatori[i].posizione -> tipo_stanza == 1){       //se la stanza è quest semplice diventa vuota

    giocatori[i].posizione -> tipo_stanza=0;

    printf("Hai eseguito la quest semplice della stanza!\nAdesso è una stanza vuota\n");

    --quest_da_finire;     //decremento la variabile delle quest da finire di 1

    printf("Agli astronauti rimangono %d quest per vincere la partita\n", quest_da_finire);

  }else if(giocatori[i].posizione -> tipo_stanza == 2){      //se la stanza è quest doppia diventa vuota

    giocatori[i].posizione -> tipo_stanza=0;

    printf("Hai eseguito la quest doppia della stanza!\nAdesso è una stanza vuota\n");

    quest_da_finire-=2;     //decremento di 2

    if(quest_da_finire==65535){   //controllo nel caso in cui manchi una sola quest ma viene eseguita una quest doppia che quindi mi stampa il massimo numero rappresentabile in unsigned short

      quest_da_finire=0;
    }

      printf("Agli astronauti rimangono %d quest per vincere la partita\n", quest_da_finire);

    }

      else if(giocatori[i].posizione -> tipo_stanza == 0 || giocatori[i].posizione -> tipo_stanza == 3){      //se eseguo il comando esegui quest in stanze che non contengono quest

        printf("Non sono presenti quest in questa stanza!\n");

      }


}

static void chiamata_emergenza(int i){

  int count_ass=0;  //flag che controlla se c'è l'assassinato in stanza
  int astro[numero_giocatori];//array che contiene il numero degli astronauti
  int impost[numero_giocatori];//array che contiene il numero degli impostori
  int a=0;
  int p=0;

if(giocatori[i].posizione -> emergenza_chiamata == 0){    //se la stanza non ha ancora avuto la chaiamata d'emergenza
    for(int c=0; c<numero_giocatori; c++){
    //se si verifica che ci sono assassinati in stanza incremento count_ass
      if(giocatori[i].posizione == giocatori[c].posizione && giocatori[c].stato_giocatore == 2){
        count_ass=1;
      }
    }

  if(count_ass == 1){// se c'è assassinato in stanza

    for(int c=0; c<numero_giocatori; c++){

      if(giocatori[i].posizione == giocatori[c].posizione && giocatori[c].stato_giocatore == 0){//salvo gli astronauti
          astro[a]=c;   //inserisco il numero del giocatore dentro l'array in posizione 0 inizialmente e ogni volta che entra nella condizione la a viene incrementata
          a++;
      }

        if(giocatori[i].posizione == giocatori[c].posizione && giocatori[c].stato_giocatore == 1){//salvo gli impostori
          impost[p]=c;
          p++;
        }

    }

    int percent_imp=0;  //percentuale che hanno gli impostori
    int percent_astro=0;  //percentuale che hanno gli astronauti
    int n_casuale=0;  //variabile per il numero casuale

    if(p!=0){ //se c'è un impostore aggiungo 30
      percent_imp=30;
    }
    if(a!=0){//se c'è un astronauta aggiungo 30
      percent_astro=30;
    }

    for(int j=0; j<p-1; j++){ //decrementa di 30 ogni impostore presente
      percent_imp-=30;
    }
    for(int j=0; j<a; j++){ //incremento di 20 per ogni astronauta presente
      percent_imp+=20;
    }
    for(int j=0; j<a-1; j++){//decremento di 30 ogni astronauta presente
      percent_astro-=30;
    }
    for(int j=0; j<p; j++){//incremento di 20 ogni impostore presente
      percent_astro+=20;
    }

if(percent_astro<0){
  percent_astro=0;
}
if(percent_imp<0){
  percent_imp=0;
}                             //controlli per non sforare sotto 0 e sopra 100
if(percent_imp>100){
  percent_imp=100;
}
if(percent_astro>100){
  percent_astro=100;
}

int giocatore_random=0;

n_casuale=rand()%100;//numero casuale
if(n_casuale<percent_imp){//se il numero che esce e minore della percentuale che hanno gli impostori
  giocatore_random=rand()%p; //casualmente scelgo un impostore tra i presenti
  giocatori[impost[giocatore_random]].stato_giocatore = 3; //e lo defenestro

  printf("Attraverso la chiamata d'emergenza è stato defenestrato il giocatore ");
  colore(impost[giocatore_random]);
  printf(" ed era un impostore\n");
  //se non esce nessun impostore controlla gli astronauti
}else{
  n_casuale=rand()%100;
  if(n_casuale<percent_astro){
    giocatore_random=rand()%a;
    giocatori[astro[giocatore_random]].stato_giocatore = 3;

    printf("Attraverso la chiamata d'emergenza è stato defenestrato il giocatore ");
    colore(astro[giocatore_random]);
    printf(" ed era un astronauta\n");
  //se non esce nemmeno un astronauta
  }else{
    printf("Non è stato defenestrato nessuno!\n");
  }
}
giocatori[i].posizione -> emergenza_chiamata = 1; //cambio la chiamata_emergenza in si così non si può richiedere

}
  if(count_ass == 0){ //se non entra nella condizione dove c'è l'assassinato stampa direttamente il messaggio
    printf("Non puoi eseguire una chiamata d'emergenza se non ci sono assassinati in stanza\n");
  }

}else{    //se la chiamata d'emergenza è stata chiamata
  printf("Non puoi eseguire la chiamata d'emergenza perchè è già stata utilizzata\n");
  }
}

static void uccidi_astronauta(int i){

  int c=0;
  int flag=0;   //controllo per far uccidere o no
  int astronauti[numero_giocatori];   //array che contiene i numeri dei giocatori che si possono uccidere
  int s=0;

printf("I giocatori che puoi uccidere sono:\n");

  for(int c=0; c<numero_giocatori; c++){

    if(giocatori[i].posizione == giocatori[c].posizione && giocatori[c].stato_giocatore == 0){    //filtro gli astronauti che sono in stanza con l'impostore che vuole uccidere

      astronauti[s]=c;  //inserisco il numero del giocatore dentro l'array che ogni ciclo viene incrementato

      printf("%d) ", s+1);
      colore(c);
      puts("");
      ++s;

      flag=1;

    }
}

if(flag==0){    //se non entra nel ciclo precedente vuol dire che non ci sono astronauti quindi stampa e si perde il turno

  printf("Non ci sono astronauti che puoi uccidere!\n");

}else{    //se flag è 1

do{

printf("Scegli quale astronauta uccidere: ");
scanf("%d", &c);
while(getchar()!='\n');

if(c<1 || c>s){
printf("Inserisci numero valido\n");
}

}while(c<1 || c>s);


printf("\nHai scelto di uccidere il giocatore %d\n", c);

colore(astronauti[c-1]);
printf(" è stato assassinato\n");

giocatori[astronauti[c-1]].stato_giocatore = 2;     //diventa assassinato

int n_casuale=0;
int probabilita=0;

for(int k=0; k<numero_giocatori; k++){
  if(giocatori[i].posizione == giocatori[k].posizione && giocatori[k].stato_giocatore == 0){   //se ci sono astronauti oltre all'assassinato e all'impostore
    probabilita+=50;  //incremento una variabile della probabilità
  }

    if(giocatori[i].posizione -> stanza_precedente == giocatori[k].posizione && giocatori[k].stato_giocatore == 0){ //se nella stanza precedente ci sono astronauti
    probabilita+=20;  //incremento probabilità
  }

}
  if(probabilita>100)
    probabilita=100;

  n_casuale=rand()%100;

  if(n_casuale < probabilita){
    giocatori[i].stato_giocatore = 3;

    printf("Sei stato scoperto! Verrai espulso dall'astronave\n");
  }else{
    printf("Nessuno ti ha notato!\n");
  }

}
}

static void usa_botola(int i, struct Stanza *stanze_botola, int contatore, int conta_botole){

  if(conta_stanze == 0){          //se non ci sono altre stanze oltre a quella iniziale non ti permette di spostarti
    printf("\nNon ci sono altre stanze in cui puoi spostarti\n");
  }
  else{
    if(giocatori[i].posizione -> tipo_stanza == 3){       //se premendo 3 la stanza è di tipo botola:

      do{

          if(lista_stanze -> tipo_stanza == 3){           //controlla ogni stanza se è di tipo botola e aumenta un contatore

            stanze_botola[conta_botole].prossima = lista_stanze;      //metto nell'array gli indirizzi delle stanze
            ++conta_botole;

            }
            lista_stanze=lista_stanze -> prossima;          //scorre la lista delle stanze
      }while(lista_stanze != NULL);                      //ripete tutto finchè lista stanze è null

      lista_stanze = stanza_inizio;       //rimetto a lista stanze l'indirizzo della stanza iniziale

      printf("\nCi sono %d stanze botola in questa partita\n", conta_botole);

      if(conta_botole == 1){    //se la stanza botola è una sola
        // prendo giocatore e lo sposto randomicamente in una stanza a caso
        printf("\nDato che non ci sono altre stanze botole verrai spostato in una stanza casuale\n");
        printf("Le stanze create in questa partita sono %d\n", conta_stanze);

        int numero_rand = rand()%conta_stanze;        //variabile per spostarmi in stanze botola casuali

printf("numero randomico %d\n", numero_rand);
      do{

        if(contatore == numero_rand){       //appena il contatore è uguale al numero randomico vuol dire che ho fatto scorrere le stanze per il numero randomico di volte

          giocatori[i].posizione = lista_stanze;

printf("indirizzo giocatore %p\n", giocatori[i].posizione);

          lista_stanze = NULL;
        }else{

        lista_stanze = lista_stanze -> prossima;      //se il contatore è inferiore al numero uscito scorre le stanze
        ++contatore;
      }

      }while(lista_stanze != NULL);

      lista_stanze=stanza_inizio;


    }else if(conta_botole > 1){              //se le stanze botola sono più di una
                                              //sposto l'impostore nelle altre stanza botola tranne quella in cui sono
      printf("Dato che le stanze di tipo botola sono più di 1 verrai spostato in un'altra stanza di tipo botola\n");

      int n_random;

      do{

      n_random = rand() % conta_botole;     //creo numero randomico da 0 al numero delle botole -1

    }while(stanze_botola[n_random].prossima == giocatori[i].posizione);   //finchè non esce una stanza botola diversa da quella in cui sono

      giocatori[i].posizione = stanze_botola[n_random].prossima;  //sposto il giocatore nella stanza botola uscita

      printf("Il giocatore adesso si trova nella stanza %p", giocatori[i].posizione);

  }

    }else{
      printf("Non ci sono botole in questa stanza\n");      //caso in cui
    }
}

}

static void sabota(int i){      //funzione degli impostori

  if(giocatori[i].posizione -> tipo_stanza == 1){   //se la stanza è di tipo quest semplice

    giocatori[i].posizione -> tipo_stanza = 0;    //la faccio diventare vuota

    printf("Hai sabotato la stanza e adesso la stanza non ha più quest eseguibili\n");

  }else if(giocatori[i].posizione -> tipo_stanza == 2){   //stessa cosa se la stanza ha quest doppia

      giocatori[i].posizione -> tipo_stanza = 0;

      printf("Hai sabotato la stanza e adesso la stanza non ha più quest eseguibili\n");

  }else{
    printf("Non ci sono quest da sabotare\n");
  }

}

static void vittoria_astro(){

printf("COMPLIMENTI ASTRONAUTI!\nAVETE VINTO LA PARTITA\n");
  printf("⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⢻⡻⠿⣿⣿⣿⣿⣿⣿⣿⣿ \n");
  printf("⣿⣿⣿⣿⣿⣿⣟⣫⡾⠛⠛⠛⠛⠛⠛⠿⣾⣽⡻⣿⣿⣿⣿⣿ \n");
  printf("⣿⣿⣿⣿⣿⡟⣼⠏⠀⠀⠀⠀⠀⠀⣀⣀⡀⣙⣿⣎⢿⣿⣿⣿ \n");
  printf("⣿⣿⣿⣿⣿⢹⡟⠀⠀⠀⣰⡾⠟⠛⠛⠛⠛⠛⠛⠿⣮⡻⣿⣿ \n");
  printf("⣿⡿⢟⣻⣟⣽⠇⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⡹⣿ \n");
  printf("⡟⣼⡟⠉⠉⣿⠀⠀⠀⠀⢿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⢟⣿ \n");
  printf("⣇⣿⠁⠀⠀⣿⠀⠀⠀⠀⠘⢿⣦⣄⣀⣀⣀⣀⣤⡴⣾⣏⣾⣿ \n");
  printf("⡇⣿⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠈⠉⠛⠋⠉⠉⠀⠀⢻⣿⣿⣿ \n");
  printf("⢃⣿⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⣿⣿ \n");
  printf("⡻⣿⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣧⣿⣿ \n");
  printf("⡇⣿⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⢹⣿⣿ \n");
  printf("⣿⡸⢷⣤⣤⣿⡀⠀⠀⠀⠀⢠⣤⣄⣀⣀⣀⠀⠀⢠⣿⣿⣿⣿ \n");
  printf("⣿⣿⣷⣿⣷⣿⡇⠀⠀⠀⠀⢸⡏⡍⣿⡏⠀⠀⠀⢸⡏⣿⣿⣿ \n");
  printf("⣿⣿⣿⣿⣿⢼⡇⠀⠀⠀⠀⣸⡇⣷⣻⣆⣀⣀⣀⣼⣻⣿⣿⣿ \n");
  printf("⣿⣿⣿⣿⣿⣜⠿⢦⣤⣤⡾⢟⣰⣿⣷⣭⣯⣭⣯⣥⣿⣿⣿⣿\n");
}

static void vittoria_impos(){

  printf("COMPLIMENTI IMPOSTORI!\nAVETE VINTO LA PARTITA\n");
  printf("░░█▀░░░░░░░░░░░▀▀███████░░░░ \n");
  printf("░░█▌░░░░░░░░░░░░░░░▀██████░░░\n");
  printf("░█▌░░░░░░░░░░░░░░░░███████▌░░ \n");
  printf("░█░░░░░░░░░░░░░░░░░████████░░\n");
  printf("▐▌░░░░░░░░░░░░░░░░░▀██████▌░░ \n");
  printf("░▌▄███▌░░░░▀████▄░░░░▀████▌░░\n");
  printf("▐▀▀▄█▄░▌░░░▄██▄▄▄▀░░░░████▄▄░ \n");
  printf("▐░▀░░═▐░░░░░░══░░▀░░░░▐▀░▄▀▌▌\n");
  printf("▐░░░░░▌░░░░░░░░░░░░░░░▀░▀░░▌▌\n");
  printf("▐░░░▄▀░░░▀░▌░░░░░░░░░░░░▌█░▌▌\n");
  printf("░▌░░▀▀▄▄▀▀▄▌▌░░░░░░░░░░▐░▀▐▐░\n");
  printf("░▌░░▌░▄▄▄▄░░░▌░░░░░░░░▐░░▀▐░░ \n");
  printf("░█░▐▄██████▄░▐░░░░░░░░█▀▄▄▀░░ \n");
  printf("░▐░▌▌░░░░░░▀▀▄▐░░░░░░█▌░░░░░░ \n");
  printf("░░█░░▄▀▀▀▀▄░▄═╝▄░░░▄▀░▌░░░░░░ \n");
  printf("░░░▌▐░░░░░░▌░▀▀░░▄▀░░▐░░░░░░░ \n");
  printf("░░░▀▄░░░░░░░░░▄▀▀░░░░█░░░░░░░ \n");
  printf("░░░▄█▄▄▄▄▄▄▄▀▀░░░░░░░▌▌░░░░░░ \n");
  printf("░░▄▀▌▀▌░░░░░░░░░░░░░▄▀▀▄░░░░░ \n");
  printf("▄▀░░▌░▀▄░░░░░░░░░░▄▀░░▌░▀▄░░░ \n");
  printf("░░░░▌█▄▄▀▄░░░░░░▄▀░░░░▌░░░▌▄▄ \n");
  printf("░░░▄▐██████▄▄░▄▀░░▄▄▄▄▌░░░░▄░ \n");
  printf("░░▄▌████████▄▄▄███████▌░░░░░▄ \n");
  printf("░▄▀░██████████████████▌▀▄░░░░ \n");
  printf("▀░░░█████▀▀░░░▀███████░░░▀▄░░ \n");
  printf("░░░░▐█▀░░░▐░░░░░▀████▌░░░░▀▄░ \n");
  printf("░░░░░░▌░░░▐░░░░▐░░▀▀█░░░░░░░▀ \n");
  printf("░░░░░░▐░░░░▌░░░▐░░░░░▌░░░░░░░ \n");
  printf("░╔╗║░╔═╗░═╦═░░░░░╔╗░░╔═╗░╦═╗░ \n");
  printf("░║║║░║░║░░║░░░░░░╠╩╗░╠═╣░║░║░ \n");
  printf("░║╚╝░╚═╝░░║░░░░░░╚═╝░║░║░╩═╝░\n");


}

void crediti(){
  printf("████████ ██████   █████  ███    ██ ███    ██  ██████  ██ \n");
  sleep(1);
  printf("   ██    ██   ██ ██   ██ ████   ██ ████   ██ ██    ██ ██ \n");
  sleep(1);
  printf("   ██    ██████  ███████ ██ ██  ██ ██ ██  ██ ██    ██ ██ \n");
  sleep(1);
  printf("   ██    ██   ██ ██   ██ ██  ██ ██ ██  ██ ██ ██    ██ ██ \n");
  sleep(1);
  printf("   ██    ██   ██ ██   ██ ██   ████ ██   ████  ██████  ██ \n\n\n");
  sleep(1);

  printf("Directed by: Christian Angileri\n\n");
    sleep(1);
  printf("Game directors:Francesco Santini & Christian Angileri\n\n");
    sleep(1);
  printf("Art director: Christian Angileri\n\n");
    sleep(1);
  printf("Programming director: Christian Angileri\n\n");
    sleep(1);
  printf("Technical artist: Christian Angileri\n\n");
    sleep(1);
  printf("Lead programmer: Christian Angileri\n\n");
    sleep(1);
  printf("Lead artist: Christian Angileri\n\n");
    sleep(1);
  printf("Supervision: Christian Angileri\n\n");
    sleep(1);
  printf("Game designer: Christian Angileri\n\n");
    sleep(1);
  printf("We would like to thank Christian Angileri for his contribution, support and dedication\n\n");
    sleep(1);
  printf("january 21 2021\n\n");


}
