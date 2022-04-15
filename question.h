#define QUESTION_LENGHT 256
#define ANSWER_LENGHT 20

typedef struct Question{
    char task[QUESTION_LENGHT];
    char optionA[ANSWER_LENGHT];
    char optionB[ANSWER_LENGHT];
    char optionC[ANSWER_LENGHT];
    char optionD[ANSWER_LENGHT];
    char correct;
}Question;