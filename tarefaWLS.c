#include <stdio.h>
#include <stdlib.h>
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "tarefaWLS.pio.h"

// Definição dos pinos para LEDs e botões
#define LED_VERMELHO 13
#define LED_VERDE 11
#define LED_AZUL 12
#define BOTAO_A 5
#define BOTAO_B 6
#define PIN_LED_MATRIZ 7
#define NUM_PIXELS 25

// Declaração de variáveis globais
static volatile uint32_t last_time = 0;
PIO pio = pio0;
uint sm;
uint8_t cont = 0; // Variável que armazena o valor que será exibido na matriz de LEDs
// Matriz de animação dos números
const uint32_t desenhoNum[10][25] = {
    { // Número 0
        0, 1, 1, 1, 0,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        0, 1, 1, 1, 0
    },
    { // Número 1
        0, 0, 1, 0, 0,
        0, 1, 1, 0, 0,
        1, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        1, 1, 1, 1, 1
    },
    { // Número 2
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1
    },
    { // Número 3
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 4
        1, 0, 0, 1, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 1, 1,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 0
    },
    { // Número 5
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 6
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 7
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0
    },
    { // Número 8
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 9
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    }
};

// Estrutura que armazena informações sobre o temporizador configurado
struct repeating_timer timer;

// Protótipo/Cabeçalho de funções
void gpio_irq_handler(uint gpio, uint32_t events);

// Função que inicializa o pino do LED e configura como saída
void led_init(uint gpio) {
    gpio_init(gpio); // Inicializa o LED
    gpio_set_dir(gpio, GPIO_OUT); // Configura o LED como saída
}

// Função que inicializa o pino do botão e configura como entrada com resistores pull-up
void botao_init(uint gpio) {
    gpio_init(gpio); // Inicializa o botão 
    gpio_set_dir(gpio, GPIO_IN); // Configura botão como entrada
    gpio_pull_up(gpio); // Habilita o pull-up interno do botão
}

// Função de callback chamada repetidamente pelo temporizador.
// Ela é usada na estrutura de repetição para mudar o estado do LED vermelho
bool repeating_timer_callback(struct repeating_timer *t) {
    // Verifica o estado do LED vermelho e alterna o mesmo, através da negação
    gpio_put(LED_VERMELHO, !gpio_get(LED_VERMELHO));

    // Retorna true (verdadeiro) para manter o temporizador funcionando
    return true;
}

uint matriz_init(uint pin_out){
    bool ok;

    ok = set_sys_clock_khz(133000, false);  // Configura o clock para 133 MHz

    // Configurações da PIO
    uint offset = pio_add_program(pio, &tarefaWLS_program);
    uint sm = pio_claim_unused_sm(pio, true);
    tarefaWLS_program_init(pio, sm, offset, PIN_LED_MATRIZ);

    return sm;
}

/* Função que calcula o índice de um LED em uma matriz 5x5, levando em consideração a disposição das linhas
 (alternando a direção da esquerda para a direita) dependendo se a linha é par ou ímpar. */
int getIndex(int num) {
  int x = num % 5;    
  int y = num / 5;    

  if (y % 2 == 0)
    return y * 5 + x; 
  else 
    return y * 5 + (4 - x); 
}

// Função para definir a intensidade das cores do LED
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (R << 16) | (G << 8) | (B); 
}


// Função para acionar a matriz de LEDs WS2812B
void acionaMatriz(const uint32_t *desenho, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        uint32_t valor_led;
        
        // Calcula o índice correto de cada LED na matriz
        int index = getIndex(i);

        // Se o LED faz parte do número, acende na cor azul bebê
        if (desenho[24 - index] == 1) {
            valor_led = matrix_rgb(r, g, b); // Azul bebê
        } else {
            valor_led = matrix_rgb(0, 0, 0); // Cor preta (desligado)
        }

        // Envia o valor para a matriz de LEDs
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}


// Função que desenha um número na matriz de LEDs
void desenhaNumero(uint8_t numero) {
    const uint32_t *desenho = desenhoNum[numero];
    // Chama a função para desenhar o número na matriz
    acionaMatriz(desenho, pio, sm, 0.53, 0.81, 0.98); // Azul bebê
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial
    sleep_ms(2000);    // Aguarde um pouco para evitar travamento no boot
    
    // Configuração dos LEDs 
    led_init(LED_VERMELHO);
    led_init(LED_VERDE);
    led_init(LED_AZUL);
    
    // Configuração dos botões A e B e das interrupções nos mesmos
    botao_init(BOTAO_A);
    botao_init(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicializa o PIO e configura o programa para controlar a matriz de LEDs
    sm = matriz_init(PIN_LED_MATRIZ);

    // Configura um temporizador para chamar a função de callback a cada 200 milissegundos, resultando em 5 chamadas por segundo.
    add_repeating_timer_ms(200, repeating_timer_callback, NULL, &timer);

    // Chama a função para exibir o número na matriz de LEDs, passando o padrão
    // definido no array e o contador atual (cont) que representa o número desejado.
    desenhaNumero(cont);
    
    while (true) {
        sleep_ms(100);
    }
}

// Função que maneja as interrupções para dois botões, incrementando ou decrementando um contador dependendo do botão pressionado, enquanto
// implementa debouncing para evitar múltiplas leituras em um único pressionamento de botão, exibindo o valor de cont na matriz de LEDs após 
// cada atualização.
void gpio_irq_handler(uint gpio, uint32_t eventos) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000) { // 200 ms de debouncing
        last_time = current_time; 

        if (gpio == BOTAO_A) {
            cont++;
            if (cont > 9) cont = 0;
        } 
        else if (gpio == BOTAO_B) {
            cont--;
            if (cont < 0) cont = 0;
        } 
        
        desenhaNumero(cont); // Atualiza a matriz com o número modificado
    }
}