// escribe en el file
uint64_t main() {
  uint64_t fd;

  fd = open("datos.txt", 2, 0);

  mmap(1073741824, 4096, 2, fd, 0);

  // modificar la memoria mapeada
  *((uint64_t*) 1073741824) = 9999;

  // persistir al archivo
  msync(1073741824);

  return 0;
}