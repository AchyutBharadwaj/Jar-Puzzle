#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>

int incommands(char **args, char *instr);
int commands(char **args, int argc, char **jnames, int *jars, int *inc, int *jm);
int equate(char *name1, char *name2, char **jnames, int *inc, int *jm, int argc, char **args, int *jars);
int fill(char **args, int *jars, char **jnames, int *jm, int *inc, int argc);
int transfer(char *name1, char *name2, char **jnames, int *jars, int *jm, int pos1, int pos2, int *inc, int argc, char **args);
void status(int *jars, char **jnames, int *inc, int *jm);
int empty(char **args, int *jars, char **jnames, int *jm, int *inc, int argc);
int bye();
int find(char **args, char **jnames, int argc, int *inc, int i);
void sprint(char *str, int time);

#define RED     "\x1b[1;3;31m"
#define GREEN   "\x1b[1;3;32m"
#define YELLOW  "\x1b[1;3;33m"
#define BLUE    "\x1b[1;3;34m"
#define MAGENTA "\x1b[1;3;35m"
#define CYAN    "\x1b[1;3;36m"
#define RESET   "\x1b[0m"

int main(int count, char **argument) {
  char instr[100];
  int argc;
  char **args;
  int i;
  int j;
  int jars[100];
  char com[20];
  char **jnames;
  int *inc;
  int jm[100];
  int ret;
  char welcome[] = "Welcome to JARS! Type 'help' for help, and 'helppdf' to open help as a PDF.";
  if(count > 1) {
    if(strcmp(argument[1], "-help") == 0) {
      j = 0;
      system("cat help.txt");
    }
    else if(strcmp(argument[1], "-helppdf") == 0) {
      j = 0;
      system("xdg-open /home/achyut/latex/help.pdf");
    }
    else if(strcmp(argument[1], "-welc") == 0) {
      j = 0;
      sprint(welcome, 100000);
    }
    else {
      j = 1;
      printf(RED "ERROR: " RESET "Invalid argument. The only extra arguments that this program takes are '-help' and \
'-helppdf'.\n");
    }
    if(count > 2) {
      if(strcmp(argument[2], "-help") == 0) {
        j = 0;
        system("cat help.txt");
      }
      else if(strcmp(argument[2], "-helppdf") == 0) {
        j = 0;
        system("xdg-open /home/achyut/latex/help.pdf");
      }
      else if(strcmp(argument[2], "-welc") == 0) {
        j = 0;
        sprint(welcome, 100000);
      }
    }
  }
  else if(count == 1) {
    j=0;
  }
  for(i=0; i<100; i++) jars[i] = 0;
  inc = malloc(sizeof(int));
  *inc = 0;
  jnames = malloc(sizeof(char *)*100);
  i=0;
  for(i=0; i<100; i++) {
    *(jnames+i) = malloc(sizeof(char)*20);
  }
  while(j==0) {
    i=0;
    args = malloc(sizeof(char *)*100);
    while(i<100) {
      *(args+i) = malloc(sizeof(char)*20);
      i++;
    }
    printf("»———➤ ");
    fgets(instr, 100, stdin);
    argc = incommands(args, instr);
    ret = commands(args, argc, jnames, jars, inc, jm);
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

int commands(char **args, int argc, char **jnames, int *jars, int *inc, int *jm) {
  int ret;
  int pos1;
  int pos2;
  if(strcmp(*(args+1), "=")==0) {
    *inc = equate(*(args+2), *(args), jnames, inc, jm, argc, args, jars);
    return 1;
  }
  else if(strcmp(*(args),"fill")==0) {
    ret = fill(args, jars, jnames, jm, inc, argc);
    return ret;
  }
  else if(strcmp(*(args),"transfer")==0) {
    pos1 = find(args, jnames, argc, inc, 1);
    pos2 = find(args, jnames, argc, inc, 2);
    ret = transfer(*(args+1), *(args+2), jnames, jars, jm, pos1, pos2, inc, argc, args);
    return ret;
  }
  else if(strcmp(*(args),"empty")==0) {
    ret = empty(args, jars, jnames, jm, inc, argc);
    return ret;
  }
  else if(strcmp(*(args),"status")==0) {
    status(jars, jnames, inc, jm);
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
  else if(strcmp(*(args), "help")==0) {
    system("cat help.txt");
    return 1;
  }
  else if(strcmp(*(args), "helppdf")==0) {
    system("xdg-open /home/achyut/latex/help.pdf");
    return 1;
  }
  else {
    printf(GREEN "SYNTAX ERROR: " RESET "Invalid command. Type 'help' to learn more about the commands.\n");
    return 2;
  }
}

int equate(char *name1, char *name2, char **jnames, int *inc, int *jm, int argc, char **args, int *jars) {
  int i;
  int inv;
  int j;
  j = find(args, jnames, argc, inc, 0);
  i = 0;
  inv = *inc;
  if(j>=*inc) {
    jm[inv] = atoi(name1);
    while(name2[i] != '\0') {
      *(*(jnames+inv)+i) = name2[i];
      i++;
    }
    *(*(jnames+inv)+i) = '\0';
    *inc = *inc + 1;
  }
  else {
    jm[j] = atoi(args[2]);
    if(jars[j] > jm[j]) jars[j] = jm[j];
  }
  return *inc;
}

int fill(char **args, int *jars, char **jnames, int *jm, int *inc, int argc) {
  int i;
  int j;
  for(j=1; j<argc; j++) {
    i = find(args, jnames, argc, inc, j);
    if(i < *inc) {
      if(jars[i] == jm[i]) printf(RED "ERROR: " RESET "%s is already filled\n", jnames[i]); 
      else jars[i] = jm[i];
    }
    else printf(RED "ERROR: " RESET "Uninitialized variable %s\n", args[j]);
  }
  status(jars, jnames, inc, jm);
  return 1;
}

void status(int *jars, char **jnames, int *inc, int *jm) {
  int i = 0;
  while(i<*inc) {
    if(jars[i] == jm[i]) printf("%s : " RED "%d/%d" RESET "\n", jnames[i], *(jars+i), *(jm+i));
    else if(jars[i] == 0) printf("%s : " BLUE "%d/%d" RESET "\n", jnames[i], *(jars+i), *(jm+i));
    else if(jars[i] > jm[i]/2) printf("%s : " YELLOW "%d/%d" RESET "\n", jnames[i], *(jars+i), *(jm+i));
    else if(jars[i] <= jm[i]/2)printf ("%s : " GREEN "%d/%d" RESET "\n", jnames[i], *(jars+i), *(jm+i));
    i++;
  }
}

int empty(char **args, int *jars, char **jnames, int *jm, int *inc, int argc) {
  int i;
  int j;
  for(j=1; j<argc; j++) {
    i = find(args, jnames, argc, inc, j);
    if(i < *inc) {
      if(jars[i] == 0) printf(RED"ERROR: "RESET"%s is already empty\n", jnames[i]);
      else jars[i] = 0;
    }
    else printf(RED"ERROR: "RESET"Uninitialized variable %s\n", args[j]);
  }
  status(jars, jnames, inc, jm);
  return 1;
}

int transfer(char *name1, char *name2, char **jnames, int *jars, int *jm, int pos1, int pos2, int *inc, int argc, char **args) {
  int j;
  int i;
  if(argc > 3) {
    printf(GREEN "SYNTAX ERROR: "RESET"Too many arguments. 'transfer' takes only 2 arguments. Type 'help' to learn more about the commands.\n");
    return 1;
  }
  else if(argc < 3) {
    printf(GREEN "SYNTAX ERROR: " RESET"Too few arguments. 'transfer' takes 2 argumetns. Type'help' to learn more about the commands.\n");
    return 1;
  }
  else {
    j = find(args, jnames, argc, inc, 1);
    i = find(args, jnames, argc, inc, 2);
    if(strcmp(name2, jnames[pos1])==0 && strcmp(name1, jnames[pos2])==0) {
      if(jars[pos2] != 0 && jars[pos1] != jm[pos1]) {
        while(jars[pos1]<jm[pos1] && jars[pos2]>0) {
          jars[pos1] = jars[pos1] + 1;
          jars[pos2] = jars[pos2] - 1;
        }
        status(jars, jnames, inc, jm);
        return 1;
      }
      else {
        printf(RED"ERROR: "RESET"Cannot transfer. Make sure you aren't transfering from an empty jar or to a filled jar.\n");
        return 2;
      }
    }
    else if(strcmp(name2, jnames[pos2])==0 && strcmp(name1, jnames[pos1])==0) {
      if(jars[pos1] != 0 && jars[pos2] != jm[pos2]) {
        while(jars[pos1]>0 && jars[pos2]<jm[pos2]) {
          jars[pos1] = jars[pos1] - 1;
          jars[pos2] = jars[pos2] + 1;
        }
        status(jars, jnames, inc, jm);
        return 1;
      }
      else {
        printf(RED"ERROR: "RESET"Cannot transfer. Make sure you aren't transfering from an empty jar or to a filled jar.\n");
        return 2;
      }
    }
    else {
      if(i>=*inc) printf(RED"ERROR: "RESET"Unitialized variable %s\n", args[2]);
      if(j>=*inc) printf(RED"ERROR: "RESET"Unitialized variable %s\n", args[1]);
      return 2;
    }
  }
}

int bye() {
  char dec[2];
  sprint("Bye!\nAre you sure you want to exit(y/n)?", 100000);
  dec[0] = fgetc(stdin);
  if(strcmp(dec, "y")==0) return 0;
  else if(strcmp(dec,"n")==0) return 2;
}

int find(char **args, char **jnames, int argc, int *inc, int i) {
  int j=0;
  while(strcmp(*(args+i), *(jnames+j)) != 0 && j != *inc) j++;
  return j;
}

void sprint(char *str, int time) {
  int i = 0;
  while(i<strlen(str)) {
    printf("%c", str[i]);
    if(str[i-1] == '.' || str[i-1] == '!') usleep(time+1000000);
    else if(str[i-1] == ',') usleep(time+500000);
    else usleep(time);
    fflush(stdout);
    i++;
  }
  printf("\n");
}
