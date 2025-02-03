# Interrupções e Matriz de LEDs 💡

## Descrição do Projeto :memo:

Este repositório foi desenvolvido para demonstrar a utilização do microcontrolador RP2040 e a placa BitDogLab, com o intuito de aplicar conceitos de interrupções e controle de hardware em sistemas embarcados. O projeto foi criado como parte de uma tarefa prática na EmbarcaTech, na residência de Sistemas Embarcados.

### Objetivo da Tarefa 🎯

A tarefa consiste na construção de um programa para controlar os seguintes componentes:

1. **Matriz 5x5 de LEDs WS2812** conectada à GPIO 7.
2. **LED RGB** com os pinos conectados às GPIOs (11, 12 e 13).
3. **Botão A** conectado à GPIO 5.
4. **Botão B** conectado à GPIO 6.

As ** funcionalidades** implementadas são:

- O **LED vermelho** pisca 5 vezes por segundo.
- O **botão A** incrementa o número exibido na matriz de LEDs quando pressionado.
- O **botão B** decrementa o número exibido na matriz de LEDs quando pressionado.
- Os **LEDs WS2812** são utilizados para exibir números de 0 a 9 na matriz, utilizando um estilo digital ou criativo, desde que o número seja claramente identificável.

### Estrutura do Projeto 📂

O projeto contém a seguinte estrutura:

- **Arquivos principais** do projeto para configuração e execução no ambiente Pico W.
- **Arquivos ligados ao `wokwi`**: Este projeto contém os arquivos `diagram.json` e `wokwi.toml` para a simulação do projeto no Wokwi.
  - Arquivos `.h`:
    - Cabeçalhos para as funções nos arquivos `.c`.
- **Arquivo `tarefaWLS.pio`**: Contém o código PIO para controlar os LEDs WS2812 e outras funções pio.

## Demonstração em Vídeo :video_camera:

Confira o vídeo com a execução desta tarefa na placa BitDogLab:

[Assista ao vídeo](https://youtube.com/shorts/0zFoVwcjHgA?feature=share)

## Como Executar :keyboard:

Para executar este projeto, siga os passos abaixo:

1. Clone este repositório para sua máquina local:
   ```bash
   git clone https://github.com/AlicaAraujo/tarefaWLS.git
2. Abrir o projeto no VS Code, importando-o pela extensão Raspberry Pi Pico;
3. Conectar a placa BitDogLab em modo BOOTSEL;
4. Compilar e executar o projeto.
