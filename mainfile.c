#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

void menu();

int main(){

menu();

  return 0;
}

void menu(){
  int c=0;    //variabile per non far iniziare direttamente da gioca

  int scelta=0;   //vriabile per scegliere tra i case dello switch

  do {
    printf(". 　　　。　　　　•　 　ﾟ　　。 　　.　ﾟ　　。 　　. \n");
    printf(".　　 。　　　　　  。ඞ . 　　 • 　　　　•　 　ﾟ　　\n");
    printf(". 　　　。　　　　• || TRANNOI ||　 　ﾟ　　。 　　.\n");
    printf("　　　•　 　ﾟ　　。 　　. 　　 。　.   ඞ 。 . 　　 •\n");            //mettere un menù bellino
    printf(". 　　　。　　　　1- Imposta gioco　ﾟ　　。 　　.  \n");
    printf("　　　•　 　ﾟ　　 2- Gioca　　　•　 　ﾟ　　。 　　. \n");
    printf("　　　•　 　ﾟ　　 3- Termina gioco　　　　•　 　ﾟ　\n");
    printf(". 　　　。　　　　4- Crediti　ﾟ　　。 　　.   ඞ   \n");
    printf("　　 。　.   ඞ   . 　　 •　　•　 　ﾟ　　。 　　. 　\n");

    scanf("%d", &scelta);
    while(getchar() != '\n');                                              //inserire scelta

    switch (scelta) {
      case 1:
      imposta_gioco();
      ++c;
      break;
        case 2:
        if(c==0)
        printf("Non puoi iniziare se non imposti il gioco\n");
        else
        gioca();
        break;
          case 3:
          termina_gioco();
          break;
          case 4:
            crediti();
            scelta=0;
            break;
            default:
            printf("Non esiste questo inserimento.\n");
          }
    }while(scelta != 3);      //ripetere il do while finchè non si preme termina gioco
}
