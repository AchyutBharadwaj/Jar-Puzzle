#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int incommands(char **args, char *instr);
int commands(char **args, int argc, char *jar5, char *jar7, int *jars, int *inc, int *jm);
int equate(char *name1, char *name2, char *jar5, char *jar7, int *inc, int *jm);
int fill(char *name, int *jars, char *jar5, char *jar7, int *jm);
int transfer(char *name1, char *name2, char *jar5, char *jar7, int *jars, int *jm);
void status(int *jars, char *jar5, char *jar7);
int empty(char *name, int *jars, char *jar5, char *jar7);
int bye();

int main(int count, char **argument) {
  char instr[100];
  int argc;
  char **args;
  int i;
  int j;
  int jars[2];
  char com[20];
  char *jar5;
  char *jar7;
  int *inc;
  int jm[2];
  int ret;
  jar5 = "";
  jar7 = "";
  if(count > 1) {
    if(strcmp(argument[1], "wh") == 0) j=0;
    else {
      j = 1;
      printf("Invalid Argument. Enter again.\n");
    }
  }
  else if(count == 1) {
    system("xdg-open /home/achyut/latex/help.pdf");
    j=0;
  }
  if(j==0){
    printf("You have two jars, one of x litres and another of y litres, where x and y are relatively \
  prime. You can do one of the following:\n a) Fill a jar completely, \n b) Empty a jar completey, \n c) Transfer \
  the contents of one jar to another.\nYour aim is to get at least one of the jars to have 1 litre in them using \
  the moves given \
  above. \nHere are the commands and steps you can use in this program:\n 1. Initialize your variables\n \
   a) <name1> = x, \n  b) <name2> = y. \n 2. fill <name> : fills the jar given in argument. \n 3. empty <name> : \
  empties the jar given in argument.\n 4. transfer <name1> <name2> : transfers the content from the jar given in the\
   first argument\
   to the jar given in \n    the second argument. \n 5. status : shows the level of each jar.\n 6. exit : exits the \
  program.\nLet' start!\n\n");
  }
  jar5 = malloc(sizeof(char)*10);
  jar7 = malloc(sizeof(char)*10);
  inc = malloc(sizeof(int));
  *inc = 0;
  while(j==0) {
    i=0;
    args = malloc(sizeof(char *)*5);
    while(i<5) {
      *(args+i) = malloc(sizeof(char)*20);
      i++;
    }
    printf("»———➤ ");
    fgets(instr, 100, stdin);
    argc = incommands(args, instr);
    ret = commands(args, argc, jar5, jar7, jars, inc, jm);
    if(ret==0) j=1;
  }
}

int incommands(char **args, char *instr) {
  int i;
  int j;
  int argc;
  i=0;
  argc = 0;
  while(instr[i] != '\n' && instr[i] != '\0') {
    j=0;
    while(instr[i] != ' ' && instr[i] != '\n' && instr[i] != '\0' && instr[i] != '=') {
      *(*(args+argc)+j) = instr[i];
      j++;
      i++;
    }
    while(instr[i] == ' ') {
      i++;
    }
    argc++;
    if(instr[i] == '=') {
      *(*(args+argc)) = instr[i];
      *(*(args+argc)+1) = '\0';
      if(instr[i+1] != ' ') {
        argc++;
        *(*(args+argc)+j) = '\0'; 
      }
      i++;
    }
  }
  return argc;
}

int commands(char **args, int argc, char *jar5, char *jar7, int *jars, int *inc, int *jm) {
  int ret;
  if(strcmp(*(args+1), "=")==0) {
    *inc = equate(*(args+2), *(args), jar5, jar7, inc, jm);
    return 1;
  }
  else if(strcmp(*(args),"fill")==0) {
    ret = fill(*(args+1), jars, jar5, jar7, jm);
    return ret;
  }
  else if(strcmp(*(args),"transfer")==0) {
    ret = transfer(*(args+1), *(args+2), jar5, jar7, jars, jm);
    return ret;
  }
  else if(strcmp(*(args),"empty")==0) {
    ret = empty(*(args+1), jars, jar5, jar7);
    return ret;
  }
  else if(strcmp(*(args),"status")==0) {
    status(jars, jar5, jar7);
    return 1;
  }
  else if(strcmp(*(args), "exit")==0) return 0;
  else if(strcmp(*(args), "Hello")==0) {
    printf("Hello!\n");
    return 2;
  }
  else if(strcmp(*(args), "Bye")==0) {
    ret = bye();
    return ret;
  }
  else {
    printf("Invalid command. Enter again. \n");
    return 2;
  }
}

int equate(char *name1, char *name2, char *jar5, char *jar7, int *inc, int *jm) {
  int i;
  int j;
  int inv;
  i = 0;
  j = 0;
  inv = *inc;
  if(fmod(inv,2) == 0) {
    jm[0] = atoi(name1);
    while(*(name2+i) != '\0' && *(name2+i) != '\n') {
      *(jar5+i) = *(name2+i);
      i++;
    }
    *(jar5+i) = '\0';
  }

  else if(fmod(inv,2) != 0) {
    jm[1] = atoi((name1));
    while(*(name2+i) != '\0' && *(name2+i) != '\n') {
      *(jar7+i) = *(name2+i);
      i++;
    }
    *(jar7+i) = '\0';
  }
  *inc = *inc + 1;
  return *inc;
}

int fill(char *name, int *jars, char *jar5, char *jar7, int *jm) {
  if(strcmp(name, jar5)==0) {
    if(jars[0] != jm[0]) {
      jars[0] = jm[0];
      status(jars, jar5, jar7);
      return 1;
    }
    else {
      printf("Cannot fill. Make sure that the jar is not already filled.\n");
      return 2;
    }
  }
  else if(strcmp(name, jar7)==0) {
    if(jars[1] != jm[1]) {
      jars[1] = jm[1];
      status(jars, jar5, jar7);
      return 1;
    }
    else {
      printf("Cannot fill. Make sure that the jar is not already filled.\n");
      return 2;
    }
  }
  else {
    printf("Uninitialized Variable. Enter again.\n");
    return 2;
  }
}

void status(int *jars, char *jar5, char *jar7) {
  printf("%s has %d litres and %s has %d litres.\n", jar5, jars[0], jar7, jars[1]);
}

int empty(char *name, int *jars, char *jar5, char *jar7) {
  if(strcmp(name, jar5)==0) {
    if(jars[0] != 0) {
      jars[0]=0;
      status(jars, jar5, jar7);
      return 1;
    }
    else {
      printf("Cannot empty. Make sure that the jar is not already emptied.\n");
      return 2;
    }
  }
  else if(strcmp(name, jar7)==0) {
    if(jars[1] != 0) {
      jars[1]=0;
      status(jars, jar5, jar7);
    }
    else {
      printf("Cannot empty. Make sure that the jar is not already emptied.\n");
      return 2;
    }
  }
  else {
    printf("Uninitialized Variable. Enter again.\n");
    return 2;
  }
}

int transfer(char *name1, char *name2, char *jar5, char *jar7, int *jars, int *jm) {
  if(strcmp(name2, jar5)==0 && strcmp(name1, jar7)==0) {
    if(jars[1] != 0 && jars[0] != jm[0]) {
      while(jars[0]<jm[0] && jars[1]>0) {
        jars[0] = jars[0] + 1;
        jars[1] = jars[1] - 1;
      }
      status(jars, jar5, jar7);
      return 1;
    }
    else {
      printf("Cannot transfer. Make sure you aren't transfering from an empty jar or to a filled jar.\n");
      return 2;
    }
  }
  else if(strcmp(name2, jar7)==0 && strcmp(name1, jar5)==0) {
    if(jars[0] != 0 && jars[1] != jm[1]) {
      while(jars[0]>0 && jars[1]<jm[1]) {
        jars[0] = jars[0] - 1;
        jars[1] = jars[1] + 1;
      }
      status(jars, jar5, jar7);
      return 1;
    }
    else {
      printf("Cannot transfer. Make sure you aren't transfering from an empty jar or to a filled jar.\n");
      return 2;
    }
  }
  else {
    printf("Unitialized Variable. Enter again.\n");
    return 2;
  }
}

int bye() {
  char dec[2];
  printf("Bye!\nAre you sure you want to exit(y/n)?");
  dec[0] = fgetc(stdin);
  if(strcmp(dec, "y")==0) return 0;
  else if(strcmp(dec,"n")==0) return 2;
}
