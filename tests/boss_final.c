// crear archivos (ver comandos python arriba)
// ../selfie -c mega_final.c -m 10000
uint64_t main() {
  uint64_t fd1;
  uint64_t fd2;
  uint64_t pid;
  uint64_t score;

  score = 0;

  // ===== FASE 1: mmap multi-page, ARCHIVO 1 (4 pages, marcadores 10/20/30/40) =====
  fd1 = open("datos.txt", 2, 0);
  mmap(1073741824, 16384, 2, fd1, 0);            // 0x40000000

  if (*((uint64_t*) 1073741824) == 10)
    if (*((uint64_t*) (1073741824 + 4096)) == 20)
      if (*((uint64_t*) (1073741824 + 8192)) == 30)
        if (*((uint64_t*) (1073741824 + 12288)) == 40)
          score = score + 1;                     // +1: archivo1 multi-page lee OK

  // ===== FASE 2: SEGUNDO ARCHIVO simultaneo (2 pages, otra direccion) =====
  fd2 = open("datos2.txt", 2, 0);
  mmap(1342177280, 8192, 2, fd2, 0);             // 0x50000000

  if (*((uint64_t*) 1342177280) == 100)
    if (*((uint64_t*) (1342177280 + 4096)) == 200)
      score = score + 10;                        // +10: segundo archivo lee OK

  // verificar que mapear archivo2 NO corrompio archivo1
  if (*((uint64_t*) 1073741824) == 10)
    score = score + 100;                         // +100: los dos coexisten sin interferir

  // ===== FASE 3: ESCRITURA en ambos archivos =====
  *((uint64_t*) 1073741824) = 1111;              // archivo1
  *((uint64_t*) 1342177280) = 2222;              // archivo2

  if (*((uint64_t*) 1073741824) == 1111)
    if (*((uint64_t*) 1342177280) == 2222)
      score = score + 1000;                      // +1000: escritura en ambos, sin cruce

  // ===== FASE 4: FORK + COMPARTIR ambos archivos =====
  pid = fork();
  if (pid == 0) {
    // HIJO: escribe en ambos archivos compartidos
    *((uint64_t*) 1073741824) = 5555;
    *((uint64_t*) 1342177280) = 6666;
    return 0;
  } else {
    wait((uint64_t*) 0);
    // PADRE: ve los cambios del hijo en AMBOS?
    if (*((uint64_t*) 1073741824) == 5555)
      if (*((uint64_t*) 1342177280) == 6666)
        score = score + 10000;                   // +10000: comparten frames de ambos archivos
  }

  // ===== FASE 5: msync + munmap + remapeo (persistencia multi-archivo) =====
  msync(1073741824);
  msync(1342177280);
  munmap(1073741824);
  munmap(1342177280);

  mmap(1073741824, 16384, 2, fd1, 0);
  mmap(1342177280, 8192, 2, fd2, 0);

  if (*((uint64_t*) 1073741824) == 5555)
    if (*((uint64_t*) 1342177280) == 6666)
      score = score + 100000;                    // +100000: persistencia multi-archivo

  // ===== FASE 6: offset != 0 (mapear archivo1 desde page 2, marcador 30) =====
  munmap(1073741824);
  mmap(1073741824, 8192, 2, fd1, 8192);          // offset 8192 -> empieza en page 2

  if (*((uint64_t*) 1073741824) == 30) {         // lee la page de offset 8192
    *((uint64_t*) 1073741824) = 3030;
    msync(1073741824);                            // persiste en offset 8192
    score = score + 1000000;                     // +1000000: offset!=0 read+write+msync
  }

  // perfecto: 1+10+100+1000+10000+100000+1000000 = 1111111
  return score;
}