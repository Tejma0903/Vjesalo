#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "operations.h"

#define MAX_WORD_LENGTH 50

extern int maxGuesses;

static void loadWords(const char* filename, Word** words, size_t* wordCount);
static int compareWords(const void* a, const void* b);

void createWord(const char* filename, const char* word) {
    if (word == NULL || strlen(word) == 0) {
        printf("Neispravna rijec!\n");
        return;
    }

    // Provjera da riječ ne sadrži brojeve ili posebne znakove
    for (size_t i = 0; i < strlen(word); ++i) {
        if (!isalpha((unsigned char)word[i])) { // Provjera da je znak slovo
            printf("Rijec ne smije sadrzavati brojeve ili posebne znakove!\n");
            return;
        }
    }

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Pogreska pri otvaranju datoteke");
        return;
    }
    fprintf(file, "%s\n", word);
    fclose(file);
}



void readWords(const char* filename) {
    Word* words = NULL;
    size_t wordCount = 0;

    // Učitaj riječi iz datoteke
    loadWords(filename, &words, &wordCount);

    if (wordCount == 0) {
        perror("Datoteka je prazna ili nema riječi.\n");
        return;
    }

    // Sortiraj riječi koristeći qsort
    qsort(words, wordCount, sizeof(Word), compareWords);

    printf("Rijeci u datoteci (sortirane):\n");
    for (size_t i = 0; i < wordCount; ++i) {
        printf("%s\n", words[i].word);
    }

    free(words);
}



void updateWord(const char* filename, const char* oldWord, const char* newWord) {
    if (oldWord == NULL || newWord == NULL || strlen(oldWord) == 0 || strlen(newWord) == 0) {
        printf("Neispravni parametri!\n");
        return;
    }

    // Provjera da stare i nove riječi ne sadrže brojeve ili posebne znakove
    for (size_t i = 0; i < strlen(oldWord); ++i) {
        if (!isalpha((unsigned char)oldWord[i])) {
            printf("Stara rijec ne smije sadrzavati brojeve ili posebne znakove!\n");
            return;
        }
    }
    for (size_t i = 0; i < strlen(newWord); ++i) {
        if (!isalpha((unsigned char)newWord[i])) {
            printf("Nova rijec ne smije sadrzavati brojeve ili posebne znakove!\n");
            return;
        }
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Pogreska pri otvaranju datoteke");
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Pogreska pri otvaranju privremene datoteke");
        fclose(file);
        return;
    }

    char buffer[MAX_WORD_LENGTH];
    int updated = 0;

    // Prolazimo kroz originalnu datoteku i tražimo rijec koju treba ažurirati
    while (fgets(buffer, MAX_WORD_LENGTH, file)) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Uklanja novi red
        if (strcmp(buffer, oldWord) == 0) {
            fprintf(tempFile, "%s\n", newWord);  // Upisujemo novu rijec u privremenu datoteku
            updated = 1;
        }
        else {
            fprintf(tempFile, "%s\n", buffer);  // Prepisujemo ostale rijeci
        }
    }

    fclose(file);
    fclose(tempFile);

    // Ako je rijec ažurirana, brišemo staru datoteku i zamenjujemo je novom
    if (updated) {
        remove(filename);               // Brišemo staru datoteku
        rename("temp.txt", filename);   // Preimenujemo privremenu datoteku u ime originalne
        printf("Rijec '%s' je azurirana u '%s'.\n", oldWord, newWord);
    }
    else {
        printf("Rijec '%s' nije pronadjena.\n", oldWord);
        remove("temp.txt");  // Ako rijec nije pronađena, brišemo privremenu datoteku
    }
}





void deleteWord(const char* filename, const char* word) {
    if (word == NULL || strlen(word) == 0) {
        printf("Neispravna rijec!\n");
        return;
    }

    // Provjera da riječ ne sadrži brojeve ili posebne znakove
    for (size_t i = 0; i < strlen(word); ++i) {
        if (!isalpha((unsigned char)word[i])) {
            printf("Rijec ne smije sadrzavati brojeve ili posebne znakove!\n");
            return;
        }
    }

    Word* words = NULL;
    size_t wordCount = 0;
    loadWords(filename, &words, &wordCount);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Pogreska pri otvaranju datoteke");
        free(words);
        return;
    }

    for (size_t i = 0; i < wordCount; ++i) {
        if (strcmp(words[i].word, word) != 0) {
            fprintf(file, "%s\n", words[i].word);
        }
    }

    fclose(file);
    free(words);
}


void playHangman(const char* filename) {
    Word* words = NULL;
    size_t wordCount = 0;
    loadWords(filename, &words, &wordCount);

    if (wordCount == 0) {
        printf("Nema rijeci za igru.\n");
        return;
    }

    // Prikazivanje rijeci dostupnih za izbor
    printf("Dostupne rijeci za igru:\n");
    for (size_t i = 0; i < wordCount; ++i) {
        printf("%zu. %s\n", i + 1, words[i].word);  // Ispis sa numeracijom
    }

    // Korisnik bira rijec
    int choice;
    printf("Unesite broj rijeci koju zelite igrati: ");
    scanf("%d", &choice);

    // Provjera unosa
    if (choice < 1 || choice > wordCount) {
        printf("Neispravan izbor.\n");
        free(words);
        return;
    }

    // Izabrana rijec
    Word selectedWord = words[choice - 1];
    free(words);

    // Igra počinje sa izabranom rijeci
    size_t len = strlen(selectedWord.word);
    char* guessed = calloc(len + 1, sizeof(char));
    int remainingGuesses = maxGuesses;
    int correctGuesses = 0;

    for (size_t i = 0; i < len; ++i) {
        guessed[i] = '_';
    }

    while (remainingGuesses > 0 && correctGuesses < len) {
        printf("\nRijec: %s\n", guessed);
        printf("Preostalo pokusaja: %d\n", remainingGuesses);
        printf("Unesite slovo: ");
        char guess;
        scanf(" %c", &guess);

        int found = 0;
        for (size_t i = 0; i < len; ++i) {
            if (selectedWord.word[i] == guess && guessed[i] == '_') {
                guessed[i] = guess;
                correctGuesses++;
                found = 1;
            }
        }

        if (!found) {
            remainingGuesses--;
        }
    }

    if (correctGuesses == len) {
        printf("Cestitamo! Pogodili ste rijec: %s\n", selectedWord.word);
    }
    else {
        printf("Izgubili ste. Rijec je bila: %s\n", selectedWord.word);
    }

    free(guessed);
}

void searchWord(const char* filename, const char* searchWord) {
    Word* words = NULL;
    size_t wordCount = 0;

    // Učitaj riječi iz datoteke
    loadWords(filename, &words, &wordCount);

    if (wordCount == 0) {
        printf("Datoteka je prazna ili nema riječi.\n");
        return;
    }

    // Sortiraj riječi
    qsort(words, wordCount, sizeof(Word), compareWords);

    // Pretraži riječ koristeći bsearch
    Word key = { .word = "" };
    strncpy(key.word, searchWord, MAX_WORD_LENGTH);
    Word* found = bsearch(&key, words, wordCount, sizeof(Word), compareWords);

    if (found != NULL) {
        printf("Rijec '%s' je pronadena.\n", found->word);
    }
    else {
        printf("Rijec '%s' nije pronadena.\n", searchWord);
    }

    free(words);
}

static void loadWords(const char* filename, Word** words, size_t* wordCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Pogreska pri otvaranju datoteke");
        return;
    }

    char buffer[MAX_WORD_LENGTH];
    *wordCount = 0;

    // Prvo brojimo riječi
    while (fgets(buffer, MAX_WORD_LENGTH, file)) {
        (*wordCount)++;
    }

    rewind(file);

    // Alociramo memoriju za riječi
    *words = malloc(*wordCount * sizeof(Word));
    if (*words == NULL) {
        perror("Pogreska pri alociranju memorije");
        fclose(file);
        return;
    }

    size_t index = 0;
    // Čitamo riječi iz datoteke
    while (fgets(buffer, MAX_WORD_LENGTH, file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Uklanja novi red
        strncpy((*words)[index].word, buffer, MAX_WORD_LENGTH);
        index++;
    }

    fclose(file);
}

static int compareWords(const void* a, const void* b) {
    return strcmp(((Word*)a)->word, ((Word*)b)->word);
}
