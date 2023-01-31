 
//File che utilizzo per contenere le variabili globali che verranno utlizzate negli altri file linkandoli

enum Stato_giocatore{astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore{rosso, blu, verde, rosa, arancione, giallo, nero, bianco, viola, marrone};
enum Tipo_stanza{vuota, quest_semplice, quest_complicata, botola};
enum Emergenza_chiamata{no, si};

struct Stanza{
  struct Stanza *avanti;
  struct Stanza *sinistra;
  struct Stanza *destra;
  struct Stanza *stanza_precedente;
  struct Stanza *prossima;            //Aggiungo campo a struct stanza per poter scorrere le stanze con lista stanze
  enum Tipo_stanza tipo_stanza;
  enum Emergenza_chiamata emergenza_chiamata;
};

struct Giocatore{
  struct Stanza *posizione;
  enum Stato_giocatore stato_giocatore;
  enum Nome_giocatore colore;
};

void imposta_gioco();
void gioca();
void termina_gioco();
void menu();
void crediti();
