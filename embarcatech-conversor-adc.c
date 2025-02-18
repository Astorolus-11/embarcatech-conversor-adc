//Bibliotecas:
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h" //Configura o conversor Analógico Digital
#include "hardware/pwm.h" //Configura o PWM
#include "hardware/i2c.h" //Permite utilizar o I2C para controlar o display SSD1306
#include "ssd1306.h" //Funções para o display SSD1306

//Pinos:
const uint led_verde = 11;
const uint led_azul = 12;
const uint led_vermelho = 13;
const uint botao_a = 5;
const uint botao_joy = 22;
#define I2C_porta i2c1 //Qual i2c vou utilizar, nesse caso o 1
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint8_t endereco = 0x3C; //Endereço do display
const uint joy_x = 26;
const uint joy_y = 27;



//Variáveis Globais:
const uint32_t wrap = 4096; //Período de 4096 = max joy_x_value e joy_y_value
const float divisor = 4.0; //Divisor
static volatile uint32_t last_time = 0; //Para o debouncing
static uint slice_azul;//Slice para o LED azul
uint slice_vermelho;  //Slice para o LED vermelho
static bool estado_pwm = true;
static bool estado_led = false;
ssd1306_t ssd;
static uint8_t borda = 0;




//Protótipos das funções:
void pwm_setup();
void setup();
static void gpio_irq_handler(uint gpio, uint32_t events); //Função de Callback da interrupção
void i2c_setup();
void adc_setup();
void ifs_joy(uint16_t joy_x_value, uint16_t joy_y_value);



int main()
{   pwm_setup();
    setup();
    i2c_setup();
    adc_setup();
    stdio_init_all();

    //Retângulo inicial:
    // (SSD , Xi , Yi , Xf , Yf , Valor , Preencher retângulo)
    ssd1306_rect(&ssd,0,0,128,64,true,false);
    ssd1306_rect(&ssd,1,1,126,62,true,false);// Aumenta mais uma camada, as outras diminuo 2 do valor

    ssd1306_send_data(&ssd); //Envia os dados pro display

    //Interrupções:
    gpio_set_irq_enabled_with_callback(botao_a,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);//Ativar/Desativar PWM
    gpio_set_irq_enabled_with_callback(botao_joy,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);//Altera o estado do LED verde
    
    while (true) {
        //Eixo Y: 
        adc_select_input(0); //Pino 26 canal 0
        uint16_t joy_y_value = adc_read(); //Lê o valor do eixo X
        
        //Eixo Y:
        adc_select_input(1); //Pino 27 canal 1
        uint16_t joy_x_value = adc_read();
       
        printf("Valor Y = %d e Valor X = %d\n",joy_y_value,joy_x_value);
        ifs_joy(joy_x_value,joy_y_value);
        
        uint16_t y_cursor = ((4095 - joy_y_value)*52)/4095;
        uint16_t x_cursor = (joy_x_value * 113)/4095;

        ssd1306_fill(&ssd,false); //Limpa a tela para atualizar a posição do quadrado 8x8

        if(borda==0){
            ssd1306_rect(&ssd, 3, 3 , 122, 58, true, false);
            ssd1306_rect(&ssd, 4, 4 , 120, 56, true, false);
            
        }
        else if(borda==1){
            ssd1306_rect(&ssd, 2, 2, 124, 60, true ,false);
        }
        else if(borda==2){
        
            ssd1306_rect(&ssd, 1, 1 , 126, 62, true, false);
                
                
    
            
        }

        ssd1306_rect(&ssd,y_cursor,x_cursor,8,8,true,true); // Quadrado 8x8
        ssd1306_send_data(&ssd); //Envia os dados

        sleep_ms(10);//Quanto menor o tempo de espera, mais fluido o movimento
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
    pwm_set_wrap(slice_vermelho,wrap); //4096
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
            pwm_set_gpio_level(led_azul,0);
            pwm_set_gpio_level(led_vermelho,0);
            estado_pwm = !estado_pwm; //inverte o valor do pwm
            printf("Estado pwm %d\n",estado_pwm);
            pwm_set_enabled(slice_azul,estado_pwm);
            pwm_set_enabled(slice_vermelho,estado_pwm);

        }
        if(gpio_get(botao_joy)==0){

            borda++;
            if(borda>2){
                borda=0;

            }
            gpio_put(led_verde,estado_led);
            

            
            
           

        }
    }
}
void i2c_setup(){
    //Inicializa o I2C com 400KHz:
    i2c_init(I2C_porta, 400*1000);

    //Habilitando a função de I2C nos pinos 14 e 15:
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
    gpio_set_function(I2C_SCL,GPIO_FUNC_I2C);

    //Habilitando o Pull-up, pois é necessário para utilizar o I2C:
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    //Inicializando o display:
    // Inicializa o display:
    //           SSD   128    64            0x3C
    ssd1306_init(&ssd,WIDTH,HEIGHT,false,endereco,I2C_porta);
    //Configura o display:
    ssd1306_config(&ssd);
    //Envia os dados ao display:
    ssd1306_send_data(&ssd);


}
void adc_setup(){

    adc_init(); //Inicializa o ADC
    adc_gpio_init(joy_x); //Inicializa o adc pro pino 26
    adc_gpio_init(joy_y); //Inicializa o adc pro pino 27
    //Inicializando os pinos:
    gpio_init(joy_x);
    gpio_init(joy_y);
    //Configurando como Entrada:
    gpio_set_dir(joy_x,GPIO_IN);
    gpio_set_dir(joy_y,GPIO_IN);
    //Habilitando Pull-up:
    gpio_pull_up(joy_x);
    gpio_pull_up(joy_y);


}

void ifs_joy(uint16_t joy_x_value, uint16_t joy_y_value){
    if(joy_y_value>=2000 && joy_y_value<=2300){ //Desliga O LED azul quando estiver no meio
        pwm_set_gpio_level(led_azul,0);
        }

        if(joy_x_value>=2000 && joy_x_value<=2300){ //Desliga o LED vermelho quando estiver no meio
            pwm_set_gpio_level(led_vermelho,0);
        }

        if(joy_x_value<2000){
            pwm_set_gpio_level(led_vermelho,4096-joy_x_value);//Como está diminuindo coloco 4095 para continuar aumentando

        }
        if(joy_y_value<2000){ 
            pwm_set_gpio_level(led_azul,4096-joy_y_value);
        }
        if(joy_x_value>2300 && joy_x_value>joy_y_value){ //Acende apenas o vermelho
            pwm_set_gpio_level(led_vermelho,joy_x_value);
        }
        if(joy_y_value>2300 && joy_y_value>joy_x_value){ //Acende apenas o azul
            pwm_set_gpio_level(led_azul,joy_y_value);
            
        }
}


