uint64_t main() {
  uint64_t fd;
  uint64_t valor;

  fd = open("datos.txt", 32768, 0);
  mmap(1073741824, 4096, 0, fd, 0);

  // leer ANTES: debe andar
  valor = *((uint64_t*) 1073741824);

  // desmapear
  munmap(1073741824);

  // leer DESPUES: debe DAR SEGFAULT
  valor = *((uint64_t*) 1073741824);

  return valor;
}