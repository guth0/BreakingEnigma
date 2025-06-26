#include "enigma.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h> // For __rdtsc()

#define NUM_RUNS 1000000

#define NUM_CHARS 1
// Number of chars in the plaintext

// Serialize rdtsc using cpuid
static inline uint64_t rdtsc_start() {
  unsigned dummy;
  __asm__ volatile("cpuid" : "=a"(dummy) : "a"(0) : "rbx", "rcx", "rdx");
  return __rdtsc();
}

static inline uint64_t rdtsc_end() {
  uint64_t t = __rdtsc();
  unsigned dummy;
  __asm__ volatile("cpuid" : "=a"(dummy) : "a"(0) : "rbx", "rcx", "rdx");
  return t;
}

// Get wall-clock time in microseconds
uint64_t get_time_us() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000;
}

// The function you want to benchmark
inline void target_function(char *str, struct Config *cfg) { Enigma(str, cfg); }

int main() {
  uint64_t total_cycles = 0;
  uint64_t total_us = 0;

  // FUNCTION SETUP
  // Initalize Rotors
  char ROTOR_1[27] = "EKMFGHDQVZXRUTIABJSYCLWPON";
  char ROTOR_2[27] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
  char ROTOR_3[27] = "BDFHJLNPRTVKXZMOQSUWYACEGI";

  char REFLECTOR_1[27] = "COAHIJRDEFQZWYBUKGXVPTMSNL";

  char ALPHABET[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for (int i = 0; i < 26; ++i) {
    REFLECTOR_1[i] -= 'A';
    ALPHABET[i] -= 'A';
    ROTOR_1[i] -= 'A';
    ROTOR_2[i] -= 'A';
    ROTOR_3[i] -= 'A';
  }

  char *string;

  struct Config cfg;

  // copy in default cyphers
  memcpy(cfg.r1, ROTOR_1, 27);
  memcpy(cfg.r2, ROTOR_2, 27);
  memcpy(cfg.r3, ROTOR_3, 27);
  memcpy(cfg.rfl, REFLECTOR_1, 27);
  memcpy(cfg.plugboard, ALPHABET, 27);

  // Random Default Values
  cfg.r1pos = 0;
  cfg.r2pos = 0;
  cfg.r3pos = 0;
  cfg.notch1 = 5;
  cfg.notch2 = 10;

  // fill plaintext with random letters so there is no pattern
  srand(time(NULL));
  char *plaintext = (char *)malloc(NUM_CHARS * sizeof(char));
  for (int i = 0; i < NUM_CHARS; i++)
  {
    plaintext[i] = 'A' + (rand() % 26);
  }

    invertConfig(&cfg);
  // FUNCTION SETUP (end)

  for (int i = 0; i < NUM_RUNS; i++) {
    uint64_t start_us = get_time_us();
    uint64_t start_cycles = rdtsc_start();

    target_function(plaintext, &cfg);

    uint64_t end_cycles = rdtsc_end();
    uint64_t end_us = get_time_us();

    uint64_t elapsed_cycles = end_cycles - start_cycles;
    uint64_t elapsed_us = end_us - start_us;

    total_cycles += elapsed_cycles;
    total_us += elapsed_us;
  }

  printf("Average CPU cycles: %llu\n",
         (unsigned long long)(total_cycles / NUM_RUNS));
  printf("Average time: %Lf µs\n",
         (long double)(total_us / (long double)NUM_RUNS));

  return 0;
}
