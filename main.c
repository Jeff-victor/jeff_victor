#include <stdio.h>
#include "chess.h"
#include "save.h"

int main(void) {
    Game g;
    int choice = 0;
    int running = 1;

    while (running) {
        printf("\nMAIN MENU\n");
        printf("1 - New game\n");
        printf("2 - Load game\n");
        printf("3 - Quit\n");
        printf("Your choice: ");

        if (scanf("%d", &choice) != 1) return 0;

        if (choice == 1) {
            init_game(&g);
            play_game(&g);

            printf("Save game? (y/n): ");
            char r; scanf(" %c", &r);
            if (r == 'y' || r == 'Y') {
                if (save_game(&g, "save.txt"))
                    printf("Game saved.\n");
                else
                    printf("Error saving game.\n");
            }

        } else if (choice == 2) {
            if (load_game(&g, "save.txt")) {
                printf("Game loaded.\n");
                play_game(&g);
            } else {
                printf("Failed to load save.txt\n");
            }

        } else if (choice == 3) {
            running = 0;

        } else {
            printf("Invalid choice.\n");
        }
    }

    printf("Goodbye!\n");
    return 0;
}
