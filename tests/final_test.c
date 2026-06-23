uint64_t main() {
  uint64_t fd;
  uint64_t p1;
  uint64_t p2;
  uint64_t p3;
  uint64_t score;

  score = 0;

  fd = open("datos.txt", 2, 0);
  mmap(1073741824, 16384, 2, fd, 0);

  // 4 generaciones, cada una escribe su page
  p1 = fork();
  if (p1 == 0) {
    *((uint64_t*) (1073741824 + 4096)) = 222;     // bisabuelo
    p2 = fork();
    if (p2 == 0) {
      *((uint64_t*) (1073741824 + 8192)) = 333;   // abuelo
      p3 = fork();
      if (p3 == 0) {
        *((uint64_t*) (1073741824 + 12288)) = 444; // nieto
        msync(1073741824);                          // el NIETO persiste su escritura
        return 0;
      } else {
        wait((uint64_t*) 0);
        msync(1073741824);                          // el abuelo persiste
        return 0;
      }
    } else {
      wait((uint64_t*) 0);
      msync(1073741824);                            // el bisabuelo persiste
      return 0;
    }
  } else {
    // TATARABUELO: espera, luego MSYNC, MUNMAP, REMMAP, y AHI verifica
    wait((uint64_t*) 0);
    msync(1073741824);

    // desmapear y remapear: leer del page cache fresco
    munmap(1073741824);
    mmap(1073741824, 16384, 2, fd, 0);

    // verificar tras remapeo: ahora SI todos escribieron (cada uno hizo su msync)
    if (*((uint64_t*) (1073741824 + 4096)) == 222)
      score = score + 1;
    if (*((uint64_t*) (1073741824 + 8192)) == 333)
      score = score + 10;
    if (*((uint64_t*) (1073741824 + 12288)) == 444)
      score = score + 100;

    // mas ciclos de syscalls
    munmap(1073741824);
    mmap(1073741824, 16384, 2, fd, 0);
    score = score + 1000;

    msync(1073741824);
    score = score + 10000;

    munmap(1073741824);
    score = score + 100000;
  }

  // perfecto: 1+10+100+1000+10000+100000 = 111111
  return score;
}