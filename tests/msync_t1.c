// accede a msync sin crashear y modifica memoria
uint64_t main() {
  uint64_t fd;
  uint64_t valor;

  fd = open("ejemplo.txt", 2, 0);          // O_RDWR = 2

  mmap(1073741824, 4096, 2, fd, 0);      // prot = 2 (permite escritura)

  // leer antes (deberia dar el contenido original)
  valor = *((uint64_t*) 1073741824);

  // modificar la memoria mapeada
  *((uint64_t*) 1073741824) = 9999;

  // sincronizar
  msync(1073741824);

  // devolver lo que leemos AHORA de la memoria (deberia ser 9999)
  return *((uint64_t*) 1073741824);
}