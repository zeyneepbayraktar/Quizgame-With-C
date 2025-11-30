#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUESTIONS 10 
#define LINE_BUFFER 256

typedef struct {
    char question[256];
    char options[4][64];
    int correct_option;
} Quizgame;

int load_questions(Quizgame quiz_questions[], int max_size);
void run_quiz(Quizgame quiz_questions[], int total_questions);

int main() {
    Quizgame quiz_questions[MAX_QUESTIONS];
    
    int total_questions = load_questions(quiz_questions, MAX_QUESTIONS);
    
    if (total_questions <= 0) {
        printf("\n\n--- ERROR ---\n");
        printf("Failed to load questions. Check if 'questions.txt' exists and is correctly formatted.\n");
        return 1; 
    }

    run_quiz(quiz_questions, total_questions);

    return 0;
}

int load_questions(Quizgame quiz_questions[], int max_size){
    FILE *file = NULL;
    char buffer[LINE_BUFFER];
    int q_count = 0;

    file = fopen("questions.txt", "r");

    if (file == NULL) {
        file = fopen("question.txt", "r");
    }

    if (file == NULL){
        return -1;
    }

    while(q_count<max_size){

        if (fgets(buffer, LINE_BUFFER, file) == NULL) break;
        
        buffer[strcspn(buffer, "\n")] = 0; 
        
        if (strlen(buffer) == 0 && q_count > 0) continue; 
        
        strcpy(quiz_questions[q_count].question, buffer);

        for (int i = 0; i < 4; i++) {
            if (fgets(buffer, LINE_BUFFER, file) == NULL) goto end_read;
            buffer[strcspn(buffer, "\n")] = 0; 
            strcpy(quiz_questions[q_count].options[i], buffer);
        }

        if (fscanf(file, "%d", &quiz_questions[q_count].correct_option) != 1) goto end_read; 

        if (fgets(buffer, LINE_BUFFER, file) == NULL) {
            q_count++;
            break; 
        }

        q_count++;
    }

    end_read:

    printf("Successfully loaded %d questions from file.\n\n", q_count);
    return q_count;
}

void run_quiz(Quizgame quiz_questions[], int total_questions) {
    int score = 0;
    int user_answer;

    printf("*********************************\n");
    printf("* STARTING C PROGRAM QUIZ NOW! *\n");
    printf("*********************************\n\n");

    for (int i = 0; i < total_questions; i++) {
        printf("--- Question %d of %d ---\n", i + 1, total_questions);
        printf("%s\n", quiz_questions[i].question);

        for (int j = 0; j < 4; j++) {
            printf("  %s\n", quiz_questions[i].options[j]);
        }
    
        printf("\nEnter your choice (1, 2, 3, or 4): ");
        
        while (scanf("%d", &user_answer) != 1 || user_answer < 1 || user_answer > 4) {
             while (getchar() != '\n'); 
             printf("Invalid input. Please enter a number between 1 and 4: ");
        }
        
        while (getchar() != '\n'); 

        if (user_answer == quiz_questions[i].correct_option) {
            printf("\n>> Correct! You earned a point.\n\n");
            score++;
        } else {
            printf("\n>> Wrong! The correct answer was option %d.\n\n", quiz_questions[i].correct_option);
        }
    }

    printf("\nQUIZ COMPLETE!\n");
    printf("Your final score is %d out of %d.\n", score, total_questions);

}
