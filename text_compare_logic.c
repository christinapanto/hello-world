#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INSTRUCTION 25
#define ALIENS 4

char **createAndFillArraywiththeInstuctions(int depth, int width);
void errorForAllocation(char *c, int i);
void freeTheArray(char **compare, int depth);

int main(void)
{
    char result, **compare, *input_instruction;
      int user_input, i;
      char DEL = '\177';
      compare = createAndFillArraywiththeInstuctions(ALIENS, INSTRUCTION);
      printf("chush\n" );
    input_instruction = (char*)calloc(1, sizeof(char*)*INSTRUCTION);
    errorForAllocation(input_instruction, 9);
    printf("input inchush\n" );
    printf("*----------------------------------------*\n");
  printf("0 of compare %s %d\n", compare[0], strlen(compare[0]));
  printf("1 of compare %s\n", compare[1]);
  printf("2 of compare %s\n", compare[2]);
  printf("3 of compare %s\n", compare[3]);
 printf("*----------------------------------------*\n");

for (i=0; i<ALIENS; i++) {
  user_input=0;
  input_instruction = (char*)calloc(1, sizeof(char*)*INSTRUCTION);

  while ((result=getchar()) != '\n') {
  input_instruction[user_input] = result;
  if ((result)>0 && result <128) {
      input_instruction[user_input] = result;
      if (user_input < INSTRUCTION) {
          user_input++;
      }
      else {
          printf("You are trying to write something to long\n");
      }
  }
 if (result == DEL) {
      if (user_input >= 0) {
          user_input--;
          printf("you deleted somtheing now %s\n", input_instruction);
      }
  }
   if (strcmp(input_instruction, compare[i]) == 0) {
    printf("IT WORKDS we are aat alien %d with instructions %s\n", i, input_instruction);
  }
printf("%d %s with length %d\n", i, input_instruction, strlen(input_instruction));

}
  printf("the while llop should end %d\n", i);
printf("compare %s with length %d\n", compare[i], strlen(compare[i]));
printf("%d %s with length %d\n", i, input_instruction, strlen(input_instruction));
}
return 0;
}



char **createAndFillArraywiththeInstuctions(int depth, int width)
{
    int i;
    char **compare;
    compare = (char**)calloc(depth, sizeof(char*));
    if (compare == NULL) {
        fprintf(stderr, "The allocation of memory for the 2D array failed\n");
        exit(1);
    }
    for (i = 0; i < ALIENS; i++) {
        compare[i] = (char*)calloc(1, sizeof(char*));
        errorForAllocation(compare[i], i);
    }
    compare[0] = "printf(\"HELLO\");";
    compare[1] = "scanf(\"%d\",&galaxies);";
    compare[2] = "for(i=0;i<5;i++)";
    compare[3] = "if(planets==50)";

    return compare;

}

void errorForAllocation(char *c, int i)
{
    if (c == NULL) {
        fprintf(stderr, "The allocation of memory for the character array failed %d\n", i);
        exit(1);
    }
}

void freeTheArray(char **compare, int depth)
{
    int i;
    for (i = depth; i > 0; i--) {
        free(compare[i]);
    }
}
