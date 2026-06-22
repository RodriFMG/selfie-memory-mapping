uint64_t main() {
  uint64_t fd;
  uint64_t ret;

  fd = open("datos.txt", 32768, 0);
  mmap(1073741824, 4096, 0, fd, 0);

  ret = munmap(1073741824);

  // si munmap encontro el mapping, ret == addr (!= -1)
  // devolvemos un codigo claro: 100 si ok, 200 si fallo
  if (ret == 1073741824)
    return 100;   // munmap encontro y desmapeo
  else
    return 200;   // munmap fallo (no encontro el mapping)
}