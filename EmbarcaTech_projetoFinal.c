#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "./display_Files/font.h"
#include "./display_Files/ssd1306.h"
#include "./ledMatriz_Files/led_matriz.h"
#include "pio_matriz.pio.h"

// Defines de pinos gerais
#define LED_RED 13
#define LED_GREEN 11
#define joyX 26
#define joyY 27
#define BUZZER 10

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
    gpio_put(LED_GREEN, 1);

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

    // Inicializa o PWM do buzzer
    gpio_init(BUZZER);
    gpio_set_function(BUZZER, GPIO_FUNC_PWM);
}

// Função para soar o alarme
void sirene(uint freq_grave, uint freq_agudo, uint duration) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    pwm_set_enabled(slice_num, true);
    uint clock = 125000000; // Frequência do clock do PWM (125 MHz)
    uint wrap_value_grave = clock / freq_grave; // Valor de wrap para a frequência grave
    uint wrap_value_agudo = clock / freq_agudo; // Valor de wrap para a frequência "aguda"

    absolute_time_t start_time = get_absolute_time();
    while (absolute_time_diff_us(start_time, get_absolute_time()) < duration * 1000) {
        // Toca a frequência grave (PEN)
        pwm_set_wrap(slice_num, wrap_value_grave);
        pwm_set_gpio_level(BUZZER, wrap_value_grave/3); // Ciclo de trabalho de 50%
        sleep_ms(200); // Tempo de cada "PEN"

        // Toca a frequência "aguda" (PEN)
        pwm_set_wrap(slice_num, wrap_value_agudo);
        pwm_set_gpio_level(BUZZER, wrap_value_agudo/3); // Ciclo de trabalho de 50%
        sleep_ms(200); // Tempo de cada "PEN"
    }

    // Desliga o BUZZER
    pwm_set_gpio_level(BUZZER, 0);
}

int main(){
    // Variáveis referentes a matriz de LED's
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &pio_matriz_program);
    pio_matriz_program_init(pio, sm, offset, pino_matriz);

    stdio_init_all();
    setup_inicial();

    while (true) {
        // Lê os valores do eixo X e Y
        adc_select_input(0);
        uint16_t joyX_valor = adc_read();
        adc_select_input(1);
        uint16_t joyY_valor = adc_read();
        printf("VRX: %d | VRY: %d\n", joyX_valor, joyY_valor);
        if(joyX_valor >= 3000 || joyY_valor >= 3000){ // Verifica se o nível de GLP está acima do limiar
            // Liga o LED vermelho
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 1);

            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "CUIDADO!!", centralizar_texto("CUIDADO!!"), 20);
            ssd1306_draw_string(&ssd, "NIVEIS DE GAS", centralizar_texto("NIVEIS DE GAS"), 30);
            ssd1306_draw_string(&ssd, "ELEVADOS", centralizar_texto("ELEVADOS"), 40);
            ssd1306_send_data(&ssd);
            
            // Soa o alarme e pisca a matriz de LED's, além de mostrar mensagem de cuidado
            while(joyX_valor >= 3000 || joyY_valor >= 3000){
                acender_leds(pio, sm, 0.0, 0.0, 1.0);
                sleep_ms(250);
                limpar_todos_leds(pio, sm);
                sirene(30, 700, 1300);
                adc_select_input(0);
                joyX_valor = adc_read();
                adc_select_input(1);
                joyY_valor = adc_read();
            }
        }
        else{// Continua com o LED verde ligado e mensagem de níveis normais
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            ssd1306_fill(&ssd, false);

            ssd1306_draw_string(&ssd, "Niveis normais", centralizar_texto("Niveis normais"), 20);
            ssd1306_draw_string(&ssd, "de gas", centralizar_texto("de gas"), 30);
            ssd1306_draw_string(&ssd, "Bom dia!", centralizar_texto("Bom dia!"), 40);
            ssd1306_send_data(&ssd);

            limpar_todos_leds(pio, sm);
        }
        sleep_ms(100); // Delay para diminuir frêquencia excessiva da CPU
    }

    return 0;
}
