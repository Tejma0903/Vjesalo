#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations.h"

int maxGuesses = 6;

static void clearBuffer();

int main() {
    char choice;
    char filename[] = "words.txt";
    char word[MAX_WORD_LENGTH];
    char newWord[MAX_WORD_LENGTH];
    char oldWord[MAX_WORD_LENGTH];

    while (1) {
        printf("\n--- Vjesala ---\n");
        printf("a. Dodaj rijec\n");
        printf("b. Prikazi rijeci\n");
        printf("c. Azuriraj rijec\n");
        printf("d. Izbrisi rijec\n");
        printf("e. Igraj vjesala\n");
        printf("f. Pretrazi rijec\n");
        printf("g. Izlaz\n");
        printf("Unesite izbor: ");
        scanf(" %c", &choice); // Dodaj razmak prije %c kako bi uklonio eventualne whitespaceove

        switch (choice) {
        case 'a':
            clearBuffer(); // Očisti ulazni spremnik prije fgets
            printf("Unesite rijec za dodavanje: ");
            fgets(word, MAX_WORD_LENGTH, stdin);
            word[strcspn(word, "\n")] = '\0'; // Ukloni znak novog reda
            createWord(filename, word);
            break;
        case 'b':
            readWords(filename);
            break;
        case 'c':
            clearBuffer(); // Očisti ulazni spremnik prije fgets
            printf("Unesite rijec za azuriranje: ");
            fgets(oldWord, MAX_WORD_LENGTH, stdin);
            oldWord[strcspn(oldWord, "\n")] = '\0';
            printf("Unesite novu rijec: ");
            fgets(newWord, MAX_WORD_LENGTH, stdin);
            newWord[strcspn(newWord, "\n")] = '\0';
            updateWord(filename, oldWord, newWord);
            break;
        case 'd':
            clearBuffer(); // Očisti ulazni spremnik prije fgets
            printf("Unesite rijec za brisanje: ");
            fgets(word, MAX_WORD_LENGTH, stdin);
            word[strcspn(word, "\n")] = '\0';
            deleteWord(filename, word);
            break;
        case 'e':
            playHangman(filename);
            break;
        case 'f':
            clearBuffer(); // Očisti ulazni spremnik prije fgets
            printf("Unesite rijec za pretragu: ");
            fgets(word, MAX_WORD_LENGTH, stdin);
            word[strcspn(word, "\n")] = '\0';
            searchWord(filename, word);
            break;
        case 'g':
            exit(0);
        default:
            printf("Neispravan izbor, pokusajte ponovno.\n");
        }
    }

    return 0;
}


static void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
