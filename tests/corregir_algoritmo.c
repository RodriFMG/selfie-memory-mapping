uint64_t main() {
  uint64_t fd;

  fd = open("algoritmo.py", 2, 0);
  mmap(1073741824, 4096, 2, fd, 0);

  // reemplazar los primeros 8 bytes "zzzzzzz\n" por "#fixed!\n"
  // offset 0 = alineado
  *((uint64_t*) 1073741824) = 730238104550975011;

  msync(1073741824);
  return 0;
}