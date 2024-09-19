#ifndef OPERATIONS_H
#define OPERATIONS_H

#define MAX_WORD_LENGTH 50

extern int maxGuesses;

typedef struct {
    char word[MAX_WORD_LENGTH];
} Word;

void createWord(const char* filename, const char* word);
void readWords(const char* filename);
void updateWord(const char* filename, const char* oldWord, const char* newWord);
void deleteWord(const char* filename, const char* word);
void playHangman(const char* filename);
void searchWord(const char* filename, const char* searchWord);


#endif // OPERATIONS_H
