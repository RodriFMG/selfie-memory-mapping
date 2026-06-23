// test 1 que pide el pdf del proyecto
uint64_t main() {
  uint64_t fd;
  uint64_t valor;

  fd = open("datos.txt", 32768, 0);

  mmap(1073741824, 4096, 0, fd, 0);

  valor = *((uint64_t*) 1073741824);

  // devolver el valor leido como exit code
  return valor;
}