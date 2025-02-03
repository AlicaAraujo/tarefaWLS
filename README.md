# Números na Matriz de LED :desktop_computer:

## Sobre :pencil:

Este repositório tem como objetivo consolidar a compreensão dos conceitos relacionados ao uso de interrupções no microcontrolador RP2040, bem como explorar as funcionalidades da placa de desenvolvimento BitDogLab. Este projeto foi desenvolvido como parte de uma tarefa na Unidade 4 do Capítulo 4 da EmbarcaTech, residência de Sistemas Embarcados.

### Descrição da Tarefa

A tarefa envolve a utilização dos seguintes componentes:

1. **Matriz 5x5 de LEDs (endereçáveis) WS2812**, conectada à GPIO 7;
2. **LED RGB**, com os pinos conectados às GPIOs (11, 12 e 13);
3. **Botão A**, conectado à GPIO 5;
4. **Botão B**, conectado à GPIO 6.

As funcionalidades implementadas são as seguintes:

- O **LED vermelho** deve piscar continuamente 5 vezes por segundo.
- O **botão A** deve incrementar o número exibido na matriz de LEDs a cada vez que for pressionado.
- O **botão B** deve decrementar o número exibido na matriz de LEDs a cada vez que for pressionado.
- A **matriz de LEDs WS2812** deve ser usada para criar efeitos visuais representando números de 0 a 9, com a formatação em estilo digital ou de forma criativa, desde que os números sejam claramente identificáveis.

### Estruturação dos Arquivos

O projeto é composto pelos seguintes arquivos e diretórios:

- **Arquivos principais** do projeto Pico W, incluindo o código de configuração.
- **Pasta `wokwi`**: Contém os arquivos `diagram.json` e `wokwi.toml` para a simulação no Wokwi.
- **Pasta `include`**: Contém os arquivos de código fonte:
  - Arquivos `.c`:
    - `animation.c`: Funções relacionadas à animação.
    - `button.c`: Funções relacionadas aos botões.
    - `led-rgb.c`: Funções relacionadas ao LED RGB.
    - `matriz_led.c`: Funções relacionadas à matriz de LEDs.
  - Arquivos `.h`:
    - `animation.h`, `button.h`, `led-rgb.h`, `matriz_led.h`: Cabeçalhos das funções acima.
- **Arquivo `ws2818b.pio`**: Contém o código PIO para controlar os LEDs WS2812.
- **Arquivo `contador-embarcatech.c`**: Gerencia todo o projeto.

## Vídeo :video_camera:

Aqui está o link para o vídeo com uma prática de simulação associada a esta tarefa:

[Assista ao vídeo](https://youtube.com/shorts/0zFoVwcjHgA?feature=share)

## Como Utilizar :computer_mouse:

Para utilizar o programa, siga os seguintes passos:

1. Clone o repositório em uma pasta separada:
   ```bash
   git clone https://github.com/AlicaAraujo/tarefaWLS.git
