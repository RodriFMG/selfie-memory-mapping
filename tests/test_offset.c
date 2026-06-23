// datos a usar en test: python3 -c "open('datos.txt','wb').write(b'A'*4096+b'B'*4096+b'C'*4096)"
// testear para cuando se coloca un offset != 0

uint64_t main() {
  uint64_t fd;
  uint64_t valor;
  uint64_t score;

  score = 0;

  fd = open("datos.txt", 2, 0);

  // mapear desde offset 4096 (page 1 del archivo = las 'B')
  mmap(1073741824, 8192, 2, fd, 4096);

  // escribir en la region mapeada (que corresponde a offset 4096 del archivo)
  *((uint64_t*) 1073741824) = 222;

  // verificar la escritura en memoria
  if (*((uint64_t*) 1073741824) == 222)
    score = score + 1;

  // msync: persistir al archivo EN EL OFFSET 4096 (no en 0)
  msync(1073741824);

  // desmapear
  munmap(1073741824);

  // remapear desde el MISMO offset y verificar que persistio
  mmap(1073741824, 8192, 2, fd, 4096);
  valor = *((uint64_t*) 1073741824);
  if (valor == 222)
    score = score + 10;        // el write con offset 4096 persistio

  // perfecto: 1 + 10 = 11
  return score;
}
