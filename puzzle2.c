#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>

typedef struct _JAR {
  char *name;
  int *lvl;
  int *mlv;
  int cur;
}JAR;


int incommands(char **args, char *instr);
int commands(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr); 
int equate(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr);
int fill(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr);
int transfer(JAR *jars, char **args, int *inc, int argc, int *mod, int *ptr);
int empty(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr);
void undo(JAR *jars, int *mod, int *ptr, int n);
void redo(JAR *jars, int *mod, int *ptr, int n);
void status(JAR *jars, char **args, int *inc, int argc);
int bye();
int find(char **args, JAR *jars, int argc, int *inc, int i);
void sprint(char *str, int time);

#define RED     "\x1b[1;3;31m"
#define GREEN   "\x1b[1;3;32m"
#define YELLOW  "\x1b[1;3;33m"
#define BLUE    "\x1b[1;3;34m"
#define MAGENTA "\x1b[1;3;35m"
#define CYAN    "\x1b[1;3;36m"
#define RESET   "\x1b[0m"

int main(int count, char **argument) {
  JAR *jars;
  char instr[100];
  char **args;
  int argc;
  int *inc;
  int ret;
  int j = 0;
  int i;
  int *mod;
  int *ptr;
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
  jars = malloc(sizeof(JAR)*100);
  inc = malloc(sizeof(int));
  ptr = malloc(sizeof(int));
  mod = malloc(sizeof(int)*100);
  *inc = 0;
  *ptr = 0;
  for(j=0; j<100; j++) {
    jars[j].name = NULL;
    jars[j].lvl = NULL;
    jars[j].mlv = NULL;
    jars[j].cur = 0;
  }
  j=0;
  while(j==0) {
    i=0;
    args = malloc(sizeof(char *)*100);
    while(i<100) {
      *(args+i) = malloc(sizeof(char)*20);
      i++;
    }
    printf("»———➤ ");
    fgets(instr, 50, stdin);
    argc = incommands(args, instr);
    ret = commands(jars, args, argc, inc, mod, ptr);
    if(ret==0) j=1;
  }
}

int incommands(char **args, char *instr) {
  int i;
  int j;
  int argc;
  char x;
  i=0;
  argc = 0;
  while(instr[i] != '\n' && instr[i] != '\0') {
    j=0;
    while(instr[i] != ' ' && instr[i] != '\n' && instr[i] != '\0' && instr[i] != '=') {
      x = instr[i];
      if(x >= 65 && x <= 90) x = x + 32;
      *(*(args+argc)+j) = x;
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

int commands(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr) {
  int ret;
  if(strcmp(args[1], "=") == 0) {
    ret = equate(jars, args, argc, inc, mod, ptr);
    return ret;
  }
  else if(strcmp(args[0], "fill") == 0) {
    ret = fill(jars, args, argc, inc, mod, ptr);
    return ret;
  }
  else if(strcmp(args[0], "empty") == 0) {
    ret = empty(jars, args, argc, inc, mod, ptr);
    return ret;
  }
  else if(strcmp(args[0], "transfer") == 0) {
    ret = transfer(jars, args, inc, argc, mod, ptr);
    return ret;
  }
  else if(strcmp(args[0], "undo") == 0) {
    if(argc > 1) undo(jars, mod, ptr, atoi(args[1]));
    else undo(jars, mod, ptr, 1);
    status(jars, args, inc, argc);
    return 1;
  }
  else if(strcmp(args[0], "redo") == 0) {
    if(argc > 1) redo(jars, mod, ptr, atoi(args[1]));
    else redo(jars, mod, ptr, 1);
    status(jars, args, inc, argc);
    return 1;
  }
  else if(strcmp(args[0], "status") == 0) {
    status(jars, args, inc, argc);
    return 1;
  }
  else if(strcmp(args[0], "exit") == 0) return 0;
  else if(strcmp(args[0], "help") == 0) {
    system("cat help.txt");
    return 1;
  }
  else if(strcmp(args[0], "helppdf") == 0) {
    system("xdg-open /home/achyut/latex/help.pdf");
    return 1;
  }
  else if(strcmp(args[0], "bye") == 0) {
    ret = bye();
    return ret;
  }
  else if(strcmp(args[0], "hello") == 0) {
    sprint("Hello! How do you do? To learn more about the program, type 'help' or 'helppdf'. 'help' will print help inside the program and 'helppdf' will open help as a PDF.", 100000);
    return 1;
  }
  else {
    printf(GREEN "SYNTAX ERROR: " RESET "Invalid command.\n");
    return 1;
  }
}

int equate(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr) {
  int i;
  int j = 0;
  i = find(args, jars, argc, inc, 0);
  if(i >= *inc) {
    jars[*inc].name = malloc(sizeof(char)*20);
    jars[*inc].lvl = malloc(sizeof(int)*100);
    jars[*inc].mlv = malloc(sizeof(int)*100);
    while(args[0][j] != '\0') {
      jars[*inc].name[j] = args[0][j];
      j++;
    }
    jars[*inc].name[j] = '\0';
    jars[*inc].mlv[jars[*inc].cur] = atoi(args[2]);
    *inc = *inc + 1;
  }
  else {
    jars[i].cur = jars[i].cur + 1;
    jars[i].mlv[jars[i].cur] = atoi(args[2]);
    if(jars[i].lvl[jars[i].cur-1] > jars[i].mlv[jars[i].cur]) jars[i].lvl[jars[i].cur] = jars[i].mlv[jars[i].cur];
    mod[*ptr] = i;
    *ptr = *ptr + 1;
  }
  return 1;
}

int fill(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr) {
  int i;
  int j;
  for(j=1; j<argc; j++) {
    i = find(args, jars, argc, inc, j);
    jars[i].cur = jars[i].cur + 1;
    if(i >= *inc) printf(RED "ERROR: " RESET "Uninitialized variable %s.\n", args[j]);
    else {
      jars[i].lvl[jars[i].cur] = jars[i].lvl[jars[i].cur-1];
      jars[i].mlv[jars[i].cur] = jars[i].mlv[jars[i].cur-1];
      if(jars[i].lvl[jars[i].cur] == jars[i].mlv[jars[i].cur]) printf(RED "ERROR: " RESET "%s already filled.\n", jars[i].name);
      else jars[i].lvl[jars[i].cur] = jars[i].mlv[jars[i].cur];
    }
    mod[*ptr] = i;
    *ptr = *ptr + 1;
  }
  status(jars, args, inc, argc);
  return 1;
}


int empty(JAR *jars, char **args, int argc, int *inc, int *mod, int *ptr) {
  int i;
  int j;
  for(j=1; j<argc; j++) {
    i = find(args, jars, argc, inc, j);
    jars[i].cur = jars[i].cur + 1;
    if(i >= *inc) printf(RED "ERROR: " RESET "Uninitialized variable %s.\n", args[j]);
    else {
      jars[i].lvl[jars[i].cur] = jars[i].lvl[jars[i].cur-1];
      jars[i].mlv[jars[i].cur] = jars[i].mlv[jars[i].cur-1];
      if(jars[i].lvl[jars[i].cur] == 0) printf(RED "ERROR: " RESET "%s already empty.\n", jars[i].name);
      else jars[i].lvl[jars[i].cur] = 0;
    }
    mod[*ptr] = i;
    *ptr = *ptr + 1;
  }
  status(jars, args, inc, argc);
  return 1;
}

int transfer(JAR *jars, char **args, int *inc, int argc, int *mod, int *ptr) {
  int i;
  int j;
  if(argc > 3) printf(GREEN "SYNTAX ERROR: " RESET "Too many arguments. Type \"help\" for help.\n");
  else if(argc < 3) printf(GREEN "SYNTAX ERROR: " RESET "Too few arguments. Type \"help\" for help.\n");
  else {
    i = find(args, jars, argc, inc, 1);
    j = find(args, jars, argc, inc, 2);
    jars[i].cur = jars[i].cur + 1;
    jars[j].cur = jars[j].cur + 1;
    mod[*ptr] = i;
    *ptr = *ptr + 1;
    mod[*ptr] = j;
    *ptr = *ptr + 1;
    if(i >= *inc) {
      printf(RED "ERROR: " RESET "Uninitialized variable %s.\n", args[1]);
      if(j >= *inc) printf(RED "ERROR: " RESET "Uninitialized vaariable %s.\n", args[2]);
      return 1;
    }
    else {
      jars[i].lvl[jars[i].cur] = jars[i].lvl[jars[i].cur-1];
      jars[i].mlv[jars[i].cur] = jars[i].mlv[jars[i].cur-1];
      jars[j].lvl[jars[j].cur] = jars[j].lvl[jars[j].cur-1];
      jars[j].mlv[jars[j].cur] = jars[j].mlv[jars[j].cur-1];
      if(jars[i].lvl[jars[i].cur] == 0 || jars[j].lvl[jars[j].cur] == jars[j].mlv[jars[j].cur]) printf("Cannot transfer. Make sure that you aren't transfering from an empty jar or to a filled jar.\n");
      else {
        while(jars[i].lvl[jars[i].cur] != 0 && jars[j].lvl[jars[j].cur] != jars[j].mlv[jars[j].cur]) {
          jars[i].lvl[jars[i].cur] = jars[i].lvl[jars[i].cur] - 1;
          jars[j].lvl[jars[j].cur] = jars[j].lvl[jars[j].cur] + 1;
        }
      }
    }
  }
  status(jars, args, inc, argc);
  return 1;
}

void undo(JAR *jars, int *mod, int *ptr, int n) {
  int i = 0;
  while(i < n) {
    jars[mod[*ptr-1]].cur = jars[mod[*ptr-1]].cur - 1;
    *ptr = *ptr - 1;
    i++;
  }
}

void redo(JAR *jars, int *mod, int *ptr, int n) {
  int i = 0;
  while(i < n) {
    jars[mod[*ptr]].cur = jars[mod[*ptr]].cur + 1;
    *ptr = *ptr + 1;
    i++;
  }
}

void status(JAR *jars, char **args, int *inc, int argc) {
  int i;
  for(i=0; i<*inc; i++) {
    if(jars[i].lvl[jars[i].cur] == jars[i].mlv[jars[i].cur]) printf(RED "%s : %d/%d" RESET "\n", jars[i].name, jars[i].lvl[jars[i].cur], jars[i].mlv[jars[i].cur]);
    else if(jars[i].lvl[jars[i].cur] == 0) printf(BLUE "%s : %d/%d" RESET "\n", jars[i].name, jars[i].lvl[jars[i].cur], jars[i].mlv[jars[i].cur]);
    else if(jars[i].lvl[jars[i].cur] <= (jars[i].mlv[jars[i].cur])/2) printf(GREEN "%s : %d/%d" RESET "\n", jars[i].name, jars[i].lvl[jars[i].cur], jars[i].mlv[jars[i].cur]);
    else if(jars[i].lvl[jars[i].cur] > (jars[i].mlv[jars[i].cur])/2) printf(YELLOW "%s : %d/%d" RESET "\n", jars[i].name, jars[i].lvl[jars[i].cur], jars[i].mlv[jars[i].cur]);
  }
}

int bye() {
  char dec[2];
  sprint("Bye!\nAre you sure you want to exit(y/n)?", 100000);
  dec[0] = fgetc(stdin);
  if(strcmp(dec, "y")==0) return 0;
  else if(strcmp(dec,"n")==0) return 2;
}

int find(char **args, JAR *jars, int argc, int *inc, int i) {
  int j=0;
  while(jars[j].name != NULL, j != *inc) {
    if(strcmp(jars[j].name, args[i]) != 0) j++;
    else return j;
  }
}

void sprint(char *str, int time) {
  int i = 0;
  while(i<strlen(str)) {
    printf("%c", str[i]);
    if(str[i-1] == '.' || str[i-1] == '!' || str[i-1] == '?') usleep(time+1000000);
    else if(str[i-1] == ',') usleep(time+500000);
    else usleep(time);
    fflush(stdout);
    i++;
  }
  printf("\n");
}
