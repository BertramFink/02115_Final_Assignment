#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>

int executeBin(char *binFile) {
  printf("\nRunning simulator with binary file: %s\n", binFile);
  pid_t pid = fork();
  if (pid == 0) {
    char *simulator = "simulator";
    char *arguments[] = {simulator, binFile, NULL};
    execv(simulator, arguments);
    printf("Failed to execute simulator");
    exit(1);
  } else {
    int result;
    waitpid(pid, &result, 0);
    printf("program executed with exit code: %d\n", result);
    return !!result;
  }
}

void executeAllBinInDir(char *dirName, int *attempts, int *successes) {
  DIR *d = opendir(dirName);
  if (d == NULL) {
    char error[100];
    sprintf(error, "Unable to find directory %s", dirName);
    perror(error);
    exit(1);
  }
  struct dirent *dir;
  while ((dir = readdir(d)) != NULL){
    if (dir->d_type == DT_REG) {
      char *extension = strrchr(dir->d_name, '.');
      if (strcmp(extension,".bin") == 0) {
        char fileName[1000];
        sprintf(fileName, "%s/%s", dirName, dir->d_name);
        int result = executeBin(fileName);
        (*attempts) ++;
        if (result == 0) {
          (*successes) ++;
        }
      }
    }
  }
  closedir(d);
}

int main() {
  int attempts = 0;
  int successes = 0;

  executeAllBinInDir("./tests/task1", &attempts, &successes);
  executeAllBinInDir("./tests/task2", &attempts, &successes);
  executeAllBinInDir("./tests/task3", &attempts, &successes);
  executeAllBinInDir("./tests/task4", &attempts, &successes);

  printf("\nSummary:\nSucesses: %d\nTotal: %d\n", successes, attempts);
  //executeBin("tests/task1/addlarge.bin");
}