uint64_t main() {
  uint64_t pid;
  uint64_t* status;

  status = malloc(8);

  pid = fork();

  if (pid == 0)
    exit(42);        // hijo: sale con 42

  wait(status);      // padre: espera al hijo

  exit(*status);     // padre sale con lo que cosecho -> deberia ser 42
}