// mapea el archivo, escribe en memoria.
// re abre el archivo y lo relee para confirmar que el cambio quedó.
uint64_t main() {
  uint64_t fd;
  uint64_t buffer;

  // fase 1: mapear, modificar, sincronizar
  fd = open("datos.txt", 2, 0);
  mmap(1073741824, 4096, 2, fd, 0);
  *((uint64_t*) 1073741824) = 9999;
  msync(1073741824);

  // fase 2: reabrir (cursor en 0) y leer con read normal
  fd = open("datos.txt", 32768, 0);    // O_RDONLY, fd nuevo, cursor en 0
  read(fd, (uint64_t*) 1073741824, 8); // leer 8 bytes a la region mapeada

  return *((uint64_t*) 1073741824);    // deberia ser 9999 si persistio
}