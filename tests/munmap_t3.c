uint64_t main() {
  uint64_t ret;

  // munmap de una direccion nunca mapeada
  ret = munmap(1073741824);

  if (ret == 4294967295)   // -1 truncado a 32 bits
    return 100;            // detecto que no existe el mapping
  else
    return 200;            // no manejo bien el caso de error
}