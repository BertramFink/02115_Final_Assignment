#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int executeBin(char *argument) {
  pid_t pid = fork();
  if (pid == 0) {
    char *simulator = "simulator";
    char *arguments[] = {simulator, argument, NULL};
    execv(simulator, arguments);
    printf("Failed to execute simulator");
    return 1;
  } else {
    int result;
    waitpid(pid, &result, 0);
    printf("program executed with exit code: %d\n", result);
  }
}

int main() {
  executeBin("tests/task1/addlarge.bin");
}