# Projeto Final

## Descrição
Este projeto consiste na implementação de um sistema de detecção de vazamento de gás com uma válvula para impedir o fluxo de gás GLP (gás encontrado predominantemente nos botijões de cozinha), além de emitir diversos alertar sonoros e visuais para as pessoas no ambiente. O projeto simula sensores e atuadores para identificar e agir perante necessidade.

## Funcionalidades
- **LED verde**: Indica que a passagem de gás GLP está ocorrendo.
- **LED vermelho**: Indica que a passagem de gás GLP está interrompida.
- **Display**: Mostra mensagens dependendo do que o sensor capta no ar.
- **Matriz de LED's**: Pisca em cor vermelha em grande intensidade luminosa para alertar pessoas próximas de possível perigo com vazamento de gás.
- **Buzzer**: Emite sinais sonoros semelhantes a de um alarme para alertar sobre possível perigo com vazamento de gás.

## Estrutura do Código
O código apresenta diversas funções, das quais vale a pena citar:

- `setup_inicial()`: Inicializa todos os pinos e periféricos necessários para o projeto.
- `sirene()`: Aciona o buzzer para alternar entre frequências de grave e agudo, simulando uma espécie de alarme.
- `ssd1306_draw_string()`: Imprime frases no display, dependendo da situação em que o ambiente se encontra.
- `acender_leds()`: Acende os LED's na matriz na cor vermelha e máxima intensidade.
- `limpar_todos_leds()`: Apaga todos os LED's na matriz.

## Estrutura dos arquivos
```
project/
│
├── display_Files/
│   ├── font.h
│   ├── ssd1306.c
│   ├── ssd1306.h
│
├── ledMatriz_Files/
│   ├── led_matriz.c
│   ├── led_matriz.h
├── EmbarcaTech_projetoFinal.c
├── CMakeLists.txt
├── pio_matriz.pio
└── README.md
```
## Desenvolvedor 
Guilherme Miller Gama Cardoso

## Link com o vídeo explicativo