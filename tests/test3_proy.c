// test 3 del pdf del proy
uint64_t main() {
  uint64_t fd;
  uint64_t pid;
  uint64_t valor;

  // ambos procesos van a mapear el mismo archivo
  fd = open("datos.txt", 2, 0);          // O_RDWR
  mmap(1073741824, 4096, 2, fd, 0);      // mapear ANTES del fork

  pid = fork();

  if (pid == 0) {
    // proceso HIJO: escribe en la memoria compartida
    *((uint64_t*) 1073741824) = 7777;
    // el hijo termina
    return 0;
  } else {
    // proceso PADRE: espera a que el hijo termine
    wait((uint64_t*) 0);

    // despues de que el hijo escribio, el padre LEE la misma direccion
    valor = *((uint64_t*) 1073741824);

    // si ve 7777, comparten el cache frame (el cambio del hijo es visible)
    if (valor == 7777)
      return 1;    // COMPARTEN: el padre ve lo que escribio el hijo
    else
      return 0;    // NO comparten: cada uno tiene su frame
  }
}