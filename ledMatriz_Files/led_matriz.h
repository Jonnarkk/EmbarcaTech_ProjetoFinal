#ifndef LED_MATRIZ_H
#define LEd_MATRIZ_H

#include "hardware/pio.h"

#define pino_matriz 7
#define num_pixels 25

uint32_t matriz_rgb(double b, double r, double g);
void limpar_todos_leds(PIO pio, uint sm);
void acender_leds(PIO pio, uint sm, double g, double b, double r);

#endif // LED_MATRIZ_H