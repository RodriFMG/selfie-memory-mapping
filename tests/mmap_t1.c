uint64_t main() {
  uint64_t fd;

  fd = open("datos.txt", 32768, 0);   // O_RDONLY = 32768 en selfie

  mmap(1073741824, 4096, 0, fd, 0);   // addr alto y alineado a PAGESIZE

  return 0;
}