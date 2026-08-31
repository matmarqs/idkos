int main() {
  char *video_memory = (char *)0xb8000;

  for (int i = 0; i < 80 * 25; i++) {
    video_memory[i] = ' ';
    video_memory[i + 1] = 0xff; /* white on black */
  }

  *video_memory = 'O';
  *(video_memory + 2) = 'K';

  while (1) {
    while ((port_byte_in(0x64) & 1) == 0) {
      ;
    }
    *video_memory = port_byte_in(0x60);
  }

  return 0;
}
