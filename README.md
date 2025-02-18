# Projeto utilizando ADC (Conversor Analógico Digital) e PWM (Pulse Width Modulation)

## 1. Propósito
O objetivo desse projeto é controlar um joystick através de conversão Analógico Digital, exibir um cursor 8x8 no display SSD1306 e controlar intensidade de 2 LEDs demonstrando domínio em conversões analógico digitais, PWM,
interrupções e debouncing.

## 2. Vídeo de demonstração



## 3. Funcionalidades
1. Botão A - Ativa ou desativa o PWM do LED RGB.
2. Botão Joy - Alterna entre 3 bordas do display SSD1306 e altera o estado do LED verde do RGB.
3. Joystick - Exibe um cursor 8x8 no display SSD 1306 e controla a intensidade dos LEDs azul e vermelho.
  Eixo X: Inicialmente no meio permanece desligado, ao movimentar para esquerda ou direita ele vai aumentando seu
  brilho.
  Eixo Y: Inicialmente no meio permanece desligado, ao movimentar para cima ou para baixo ele vai aumentando seu
  brilho.
   
   
## 4. Pré-requisitos
1. Ter o [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado na máquina;
2. Ter o [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) instalado;
3. Ter o [Visual Studio Code](https://code.visualstudio.com/download) instalado;
4. Ter este repositório clonado na sua máquina;
5. Ter as seguintes extensões instaladas no seu VS Code para o correto funcionamento:
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools);
- [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake);
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools);
- [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico);

##  5. Como executar o projeto e simular:
1. Clonar o repositório: https://github.com/Astorolus-11/embarcatech-conversor-adc
2. Abrir a pasta clonada através do import project:

   ![image](https://github.com/user-attachments/assets/9ea528e1-0253-4cf8-b6c6-8532be0fc1b4)
   

3. Para executar na placa clique em Run que está localizada no rodapé do vscode (A placa precisa já está conectada e com o BootSel ativado):

   ![image](https://github.com/user-attachments/assets/36b14dce-1309-4f0c-a7f3-3cd7edb2b336)
   
  
  # Pronto! já está tudo pronto para testar o projeto!
