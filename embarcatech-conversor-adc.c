//Bibliotecas:
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h" //Configura o conversor Analógico Digital
#include "hardware/pwm.h" //Configura o PWM

//Pinos:
const uint led_verde = 11;
const uint led_azul = 12;
const uint led_vermelho = 13;
const uint botao_a = 5;
const uint botao_joy = 22;

//Variáveis Globais:
const uint32_t wrap = 4000; //Período de 4000 (wrap)
const float divisor = 4.0; //Divisor
static volatile uint32_t last_time = 0; //Para o debouncing
static uint slice_azul;//Slice para o LED azul
uint slice_vermelho;  //Slice para o LED vermelho
static bool estado_pwm = false;
static bool estado_led = false;



//Protótipos das funções:
void pwm_setup();
void setup();
static void gpio_irq_handler(uint gpio, uint32_t events); //Função de Callback da interrupção




int main()
{   pwm_setup();
    setup();
    stdio_init_all();

    //Interrupções:
    gpio_set_irq_enabled_with_callback(botao_a,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);//Ativar/Desativar PWM
    gpio_set_irq_enabled_with_callback(botao_joy,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);//Altera o estado do LED verde
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
    slice_azul = pwm_gpio_to_slice_num(led_azul);
    slice_vermelho = pwm_gpio_to_slice_num(led_vermelho);
    
    //Configuração LED azul:
    pwm_set_clkdiv(slice_azul,divisor); // 4.0
    pwm_set_wrap(slice_azul,wrap);  // 4000
    pwm_set_gpio_level(led_azul,0); //Inicialmente desligado
    pwm_set_enabled(slice_azul,true); //PWM ATIVADO

    //Configuração LED vermelho: (A mesma configuração do LED azul)
    pwm_set_clkdiv(slice_vermelho,divisor); // 4.0
    pwm_set_wrap(slice_vermelho,wrap); //4000
    pwm_set_gpio_level(led_vermelho,0); //Desligado
    pwm_set_enabled(slice_vermelho,true); //PWM ATIVADO


}

void setup(){
    //Inicializando
    gpio_init(led_verde);
    gpio_init(botao_a);
    gpio_init(botao_joy);
    //Definindo Entrada/Saida
    gpio_set_dir(led_verde,GPIO_OUT);
    gpio_set_dir(botao_a,GPIO_IN);
    gpio_set_dir(botao_joy,GPIO_IN);
    //Habilitando o Pull-up
    gpio_pull_up(botao_a);
    gpio_pull_up(botao_joy);
    //LED verde desligado inicialmente
    gpio_put(led_verde,false);//Inicialmente desligado

}
void gpio_irq_handler(uint gpio, uint32_t events){

    uint32_t tempo_atual = to_us_since_boot(get_absolute_time()); //Tempo atual desde o inicio do sistema, convertendo para us
    if(tempo_atual-last_time>200000){
        last_time=tempo_atual; //Atualiza o last_time

        if(gpio_get(botao_a)==0){ //Botão A pressionado

            estado_pwm = !estado_pwm; //inverte o valor do pwm
            pwm_set_enabled(slice_azul,estado_pwm);
            pwm_set_enabled(slice_vermelho,estado_pwm);

        }
        if(gpio_get(botao_joy)==0){

            estado_led = !estado_led; //Inverte o valor do led

            gpio_put(led_verde,estado_led);


        }
    }
}
