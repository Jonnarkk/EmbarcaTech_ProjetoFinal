#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "./display_Files/font.h"
#include "./display_Files/ssd1306.h"

// Defines de pinos gerais
#define LED_RED 13
#define LED_GREEN 11
#define joyX 26
#define joyY 27

// Defines específicos do display
#define I2C_PORT i2c1
#define ADDR 0x3c
#define I2C_SDA 14
#define I2C_SCL 15

// Variáveis globais
ssd1306_t ssd;

// Função para fazer o setup inicial
void setup_inicial(){
    // Inicialização do LED's

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Inicializa o I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);

    // Inicializa o Display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDR, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicializa o ADC e os eixos do joystick
    adc_init();
    adc_gpio_init(joyX);
    adc_gpio_init(joyY);
}

int main(){

    char caracter = 'A';
    stdio_init_all();
    setup_inicial();

    ssd1306_fill(&ssd, false); 
    ssd1306_draw_char(&ssd, caracter, 64, 32); // Imprime no display
    ssd1306_send_data(&ssd);

    while (true) {
        adc_select_input(0);
        uint16_t joyX_valor = adc_read();
        adc_select_input(1);
        uint16_t joyY_valor = adc_read();
        printf("VRX: %d | VRY: %d\n", joyX_valor, joyY_valor);
        sleep_ms(1000);
    }
}
