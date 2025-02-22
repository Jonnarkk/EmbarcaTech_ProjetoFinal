#include "./led_matriz.h"

uint32_t matriz_rgb(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

void limpar_todos_leds(PIO pio, uint sm) {
    for (int i = 0; i < num_pixels; i++) {
        pio_sm_put_blocking(pio, sm, matriz_rgb(0.0, 0.0, 0.0));
    }
}

void acender_leds(PIO pio, uint sm, double r, double g, double b) {
    for (int i = 0; i < num_pixels; i++) {
        pio_sm_put_blocking(pio, sm, matriz_rgb(b, r, g));
    }
}
