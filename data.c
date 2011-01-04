#include <stdio.h>
void bar(char* p) {
  printf("Succeed in Jitting!\n");
}

void foo() {
  char* kerker;
  bar(kerker);
}

int main(int argc, char** argv) {
  foo();
}

