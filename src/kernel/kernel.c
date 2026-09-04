#include <libk.h>
#include <vga.h>

int main() {
  term_init();
  term_clear();
  term_print("Hello, kernel!");
  return 0;
}
