# Simon Says Game

Um jogo de "Simon Says" desenvolvido em C++ usando a biblioteca de computação gráfica **ABCg**. Este projeto recria o clássico jogo de memória onde o jogador deve repetir sequências de cores mostradas pelo sistema.

## Visão Geral

Neste jogo, o jogador precisa observar e memorizar uma sequência de cores que são reproduzidas na tela. A cada rodada, a sequência aumenta em um item, e o jogador deve repetir corretamente para continuar no jogo. A dificuldade aumenta conforme a sequência cresce!

## Funcionalidades

- Interface gráfica colorida que destaca cada cor da sequência.
- Animações para cada cor exibida.
- Aumento automático de dificuldade com o avanço das rodadas.
- Verificação de sequências corretas para dar feedback ao jogador.

## Detalhes do código

A nossa janela personalizada da aplicação altera sobrescreve seguintes as funções padrões das janelas da ABCg:

- onCreate()
- onPaint()
- onUpdate()
- onResize()
- onDestroy()
- onEvent()

Também inclui nossas próprias funções:

- playerTurn()
- setupModel()
- endGame()

A função onCreate() é chamada ao se inicializar a janela, ela foi sobrescrita de forma a configurar os shaders para renderização, definir a cor de fundo, inicializar o gerador de números aleatórios, gerar uma sequência de cores aleatórias em um vetor (a partir de um inteiro de 0 a 3 gerado aleatoriamente, sendo cada uma das quatro cores dos botões representadas por um desses quatro inteiros).

A função onPaint() é chamada uma vez a cada quadro, esse função chama a setupModel() que é responsavel por desenhar circulos com quatro quadrantes de cores que são passadas como argumentos de suas chamadas. Antes dessa função ser chamada a função onUpdate() é chamada também, que por sua vez é responsável pelas animações dos botões piscando.

A função setupModel() pode criar tanto o círculo de quadrantes coloridos, como o círculo preto no meio do círculo colorido (que é desenhado para que o formato do círculo colorido do fundo fique mais parecido com o jogo real).

A animação de piscar dos botões é feita ao se chamar a função setupModel() com argumentos diferentes para o cículo colorido, ou melhor passando-se cores mais claras para os botões que piscam.

A função onEvent() lida com o input do jogador pelo mouse.

O input do jogador é comparado com as entradas do vetor que armazena a sequência aleatória de cores, na função playerTurn(). Que por sua vez chama a função endGame() passando em seu argumento um valor que indica se o jogador foi vencedor ou não, encerrando o jogo com uma mensagem apropriada.

## Tecnologias Utilizadas

- Linguagem: C++
- Biblioteca gráfica: **ABCg** (para renderização de gráficos)
- Ferramentas adicionais: **CMake** (para compilação e gerenciamento de dependências)

