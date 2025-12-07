#include <stdio.h>
#include "chess.h"
#include "save.h"

int main(void) {
    Game g;
    int choix = 0;
    int running = 1;

    while (running) {
        printf("\n MENU PRINCIPAL \n");
        printf("1 - Nouvelle partie\n");
        printf("2 - Charger une partie\n");
        printf("3 - Quitter\n");
        printf("Votre choix: ");

        if (scanf("%d", &choix) != 1) {
            return 0;
        }

        switch (choix) {
            case 1:
                init_game(&g);
            play_game(&g);
            //  la fin de la partie on propose une sauvegarde
            printf("Sauvegarder la partie ? (y/n): ");
            char rep1;
            scanf(" %c", &rep1);
            if (rep1 == 'y' || rep1 == 'Y') {
                if (save_game(&g, "save.txt"))
                    printf("Partie sauvegardee dans save.txt\n");
                else
                    printf("Erreur de sauvegarde.\n");
            }
            break;

            case 2:
                if (load_game(&g, "save.txt")) {
                    printf("Partie chargee depuis save.txt\n");
                    play_game(&g);
                } else {
                    printf("Impossible de charger save.txt\n");
                }
            break;

            case 3:
                running = 0;
            break;

            default:
                printf("Choix invalide.\n");
            break;
        }
    }

    printf("good bye \n");
    return 0;
}

