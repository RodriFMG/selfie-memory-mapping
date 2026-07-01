uint64_t main() {
  uint64_t pid;

  pid = fork();

  if (pid == 0)
    exit(1);     // contexto B

  exit(2);       // contexto A
}