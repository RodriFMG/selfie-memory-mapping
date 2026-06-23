// testeo de aislamiento, si se hace fork y en el hijo se hace munmap, no afecta al padre
uint64_t main() {
  uint64_t fd;
  uint64_t pid;
  uint64_t valor;

  fd = open("datos.txt", 2, 0);
  mmap(1073741824, 4096, 2, fd, 0);          // mapear antes del fork
  *((uint64_t*) 1073741824) = 100;           // padre escribe 100

  pid = fork();
  if (pid == 0) {
    // HIJO: desmapea SU acceso (no deberia afectar al padre)
    munmap(1073741824);
    return 0;
  } else {
    // PADRE: espera, luego intenta acceder (su mapping sigue vivo?)
    wait((uint64_t*) 0);
    valor = *((uint64_t*) 1073741824);       // el padre lee: deberia seguir andando
    if (valor == 100)
      return 1;     // el munmap del hijo NO afecto al padre
    else
      return 0;
  }
}