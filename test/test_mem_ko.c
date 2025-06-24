#include <stdio.h>
#include "trasher.h"

// Power Printing macro to make pretty tests
#ifdef SUPPRESS_PRINT
#define PRINT(fmt, ...) ((void)0)
#define POOL_STATUS() ((void)0)
#else
#define PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define POOL_STATUS() pool_status()
#endif


int main() {
  PRINT("Hello World!\n\n");

  size_t buffer_size = 10000;
  char* buffer = mem(buffer_size);

  int j = 0;
  for (int i = 0; i < 9999; i++) {
    buffer[i] = 65 + (j % 24);
    j++;
  }

  // PRINT("%s\n", buffer);

  int* array_IDS = mem_id(33, 5);

  int* array_ids_2 = mem_id(22, 2);

  array_IDS[4] = 25;
  array_ids_2[3] = 232;

  PRINT("%d\n", array_ids_2[3]);

  free_id(2);

  PRINT("array_IDS access : %d\n", array_IDS[4]);
  PRINT("array_ids_2 : %d", array_ids_2[3]); // Memcheck Valgrind shoudl notify invalid read!

  // Named Pools : Tag
  int *speed_array = mem_name(20, "cars");
  for (int i = 0; i < 20; i++)
    speed_array[i] = i * 4;


  int *fuel_array = mem_name(25, "cars");
  fuel_array[0] = 10;
  fuel_array[1] = 20;

  free_name("cars");

  POOL_STATUS();


  free_name("cars");

  free_pool_all();

  return 0;
}
