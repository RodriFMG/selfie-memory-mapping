uint64_t main() {
  uint64_t fd;
  uint64_t valor;

  // mapear, escribir, persistir
  fd = open("datos.txt", 2, 0);
  mmap(1073741824, 4096, 2, fd, 0);
  *((uint64_t*) 1073741824) = 9999;
  msync(1073741824);

  // desmapear
  munmap(1073741824);

  // remapear el mismo archivo y leer
  mmap(1073741824, 4096, 2, fd, 0);
  valor = *((uint64_t*) 1073741824);

  // comparar
  if (valor == 9999)
    return 1;
  else
    return 0;
}