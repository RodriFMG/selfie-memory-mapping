// lo mismo que t1 pero only write
uint64_t main() {
  uint64_t fd;
  fd = open("datos.txt", 2, 0);
  mmap(1073741824, 4096, 1, fd, 0);    // prot = 1, write-only
  *((uint64_t*) 1073741824) = 9999;    // SOLO escribir, NO leer
  msync(1073741824);
  return 0;                             // NO leer la memoria mapeada
}