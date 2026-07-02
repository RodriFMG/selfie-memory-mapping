// test 4: offsets NO alineados (offset % PAGESIZE != 0)
// requiere datos.txt con marcadores 10/20/30/40 al inicio de cada page (el del boss)
//
// CASO A: offset=100, length=4097 -> 100%4096=100, 100+4097=4197 -> 2 pages (cruza el borde)
// CASO B: offset=2096, length=2000 -> 2096%4096=2096, 2096+2000=4096 -> 1 page (no cruza)
// A y B caen en la misma file page 0 -> deben COMPARTIR el mismo cache frame
//
// score perfecto = 111111

/* Datos a usar...  >:3

python3 -c "
import struct
open('datos.txt','wb').write(b''.join(struct.pack('<Q',v)+b'\0'*4088 for v in [10,20,30,40]))
"

*/

uint64_t main() {
  uint64_t fd;
  uint64_t* buffer;
  uint64_t score;

  score = 0;
  fd = open("datos.txt", 2, 0);

  // ===== CASO A: offset no alineado + length que CRUZA a otra page (2 pages) =====
  mmap(1073741824, 4097, 2, fd, 100);              // 0x40000000

  if (*((uint64_t*) 1073741824) == 10)             // file page 0 (100 alineado hacia abajo -> 0)
    score = score + 1;

  if (*((uint64_t*) (1073741824 + 4096)) == 20)    // cruce: file page 1, mapeada gracias a n_pages=2
    score = score + 10;

  // ===== CASO B: offset no alineado + length que NO cruza (1 page) =====
  mmap(1342177280, 2000, 2, fd, 2096);             // 0x50000000, 2096/4096 = file page 0

  if (*((uint64_t*) 1342177280) == 10)             // misma file page 0 que A
    score = score + 100;

  // ===== COMPARTICION: A y B apuntan al MISMO cache frame =====
  *((uint64_t*) 1073741824) = 7777;                // escribo por A...
  if (*((uint64_t*) 1342177280) == 7777)           // ...y lo veo por B
    score = score + 1000;

  // ===== ESCRITURA en la page cruzada, visible en memoria =====
  *((uint64_t*) (1073741824 + 4096)) = 2020;
  if (*((uint64_t*) (1073741824 + 4096)) == 2020)
    score = score + 10000;

  // ===== PERSISTENCIA: msync escribe las 2 pages del mapping A =====
  msync(1073741824);

  buffer = malloc(8);
  *buffer = 0;                                     // tocar la pagina para que selfie la mapee
  fd = open("datos.txt", 32768, 0);                // reabrir, cursor en 0
  read(fd, buffer, 8);                             // primeros 8 bytes del DISCO

  if (*buffer == 7777)                             // el 7777 escrito por A persistio
    score = score + 100000;

  return score;
}
