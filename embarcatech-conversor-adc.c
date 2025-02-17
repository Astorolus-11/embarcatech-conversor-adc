//Bibliotecas:
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h" //Configura o conversor Analógico Digital
#include "hardware/pwm.h" //Configura o PWM

//Pinos:
const uint led_azul = 12;
const uint led_vermelho = 13;

//Variáveis Globais:
const uint32_t wrap = 4000; //Período de 4000 (wrap)
const float divisor = 4.0; //Divisor



//Protótipos
void pwm_setup();




int main()
{   pwm_setup();
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
void pwm_setup(){

    //Ativando o periférico como PWM:
    gpio_set_function(led_azul,GPIO_FUNC_PWM);
    gpio_set_function(led_vermelho, GPIO_FUNC_PWM);
    //Slices:
    uint slice_azul = pwm_gpio_to_slice_num(led_azul);
    uint slice_vermelho = pwm_gpio_to_slice_num(led_vermelho);
    //Configuração LED azul:
    pwm_set_clkdiv(slice_azul,divisor); // 4.0
    pwm_set_wrap(slice_azul,wrap);  // 4000
    pwm_set_gpio_level(led_azul,0); //Inicialmente desligado
    //Configuração LED vermelho: (A mesma configuração do LED azul)
    pwm_set_clkdiv(slice_vermelho,divisor); // 4.0
    pwm_set_wrap(slice_vermelho,wrap); //4000
    pwm_set_gpio_level(led_vermelho,0); //Desligado


}
