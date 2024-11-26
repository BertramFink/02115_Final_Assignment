#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>

int executeBin(char *binFile) {
  pid_t pid = fork();
  if (pid == 0) {
    printf("Running binary file: %s - ", binFile);
    // Throw away standard output and errors of simulator
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    execl("simulator", "simulator", binFile, NULL);
    printf("Failed to execute simulator\n");
    exit(1);
  } else {
    int result;
    waitpid(pid, &result, 0);
    printf("%s\n", result ? "FAILED" : "SUCCESS");
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
    if (dir->d_type == DT_DIR) {
      if (dir->d_name[0] != '.') {
        char newDirName[1000];
        sprintf(newDirName, "%s/%s", dirName, dir->d_name);
        executeAllBinInDir(newDirName, attempts, successes);
      }
    }
    else if (dir->d_type == DT_REG) {
      char *extension = strrchr(dir->d_name, '.');
      if (strcmp(extension,".bin") == 0) {
        char fileName[1000];
        sprintf(fileName, "%s/%s", dirName, dir->d_name);
        int result = executeBin(fileName);
        (*attempts) ++;
        (*successes) += !result;
      }
    }
  }
  closedir(d);
}

int main() {
  int attempts = 0;
  int successes = 0;
  executeAllBinInDir("./tests", &attempts, &successes);
  printf("\nSummary:\nSucesses: %d\nTotal: %d\n", successes, attempts);
  return 0;
}