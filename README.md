# Simon Says Game

Um jogo de "Simon Says" desenvolvido em C++ usando a biblioteca de computação gráfica **ABCg**. Este projeto recria o clássico jogo de memória onde o jogador deve repetir sequências de cores mostradas pelo sistema.

## Visão Geral

Neste jogo, o jogador precisa observar e memorizar uma sequência de cores que são reproduzidas na tela. A cada rodada, a sequência aumenta em um item, e o jogador deve repetir corretamente para continuar no jogo. A dificuldade aumenta conforme a sequência cresce!

## Funcionalidades

- Interface gráfica colorida que destaca cada cor da sequência.
- Animações para cada cor exibida.
- Aumento automático de dificuldade com o avanço das rodadas. (*)
- Verificação de sequências corretas para dar feedback ao jogador. (*)

(*): Essas funcionalidades ainda não estão prontas.

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

A função onCreate() é chamada ao se inicializar a janela, ela foi sobrescrita de forma a configurar os shaders para renderização, definir a cor de fundo, inicializar o gerador de números aleatórios, gerar uma sequência de cores aleatórias em um vetor (a partir de um inteiro de 0 a 3 gerado aleatoriamente, sendo cada uma das quatro cores dos botões representadas por um desses quatro inteiros) chamado de m_colorSequence.

A função onPaint() é chamada a cada quadro, sendo ela responsável por pintar os círculos que são desenhados a partir de um Vertex Array Object (VAO) (que é um um array que tem acesso a coordenadas e cores de uma sequência de pontos) armazenado na variável global chamada de m_vao. Essa função chama a função glDrawArrays() que desenha triangulos conectando dois a dois os pontos da circumfêrencia do círculo com o ponto central do círculo, esses triangulos ficam com a base apontando para fora do círculo e são coloridos a partir das cores associadas aos pontos da circumferência.

A função responsável por criar esse VAO usado em onPaint() é o setupModel(), que cria essa sequência de 182 coordenadas e 182 cores, as coordenadas são os pontos da circumferencia do círculo e as cores são designadas as coordenadas a partir do quadrante em que a coordenada se encontra que é determinado pelo angulo da coordenada no círculo (por exemplo se o angulo for entre 0 e π/2 então a coordenada está no primeiro quadrante).  

A animação de piscar dos botões é feita ao se chamar a função setupModel() com argumentos diferentes para o cículo colorido, ou melhor passando-se cores mais claras para os botões que piscam. A sequência de botões que piscam é definida pela sequência de cores armazenadas no vetor m_colorSequence.

A função onEvent() lida com o input do jogador pelo mouse. O input do jogador é comparado com as entradas do vetor que armazena a sequência aleatória de cores, na função playerTurn(). Que por sua vez chama a função endGame() passando em seu argumento um valor que indica se o jogador foi vencedor ou não, encerrando o jogo com uma mensagem apropriada. 

## Tecnologias Utilizadas

- Linguagem: C++
- Biblioteca gráfica: **ABCg** (para renderização de gráficos)
- Ferramentas adicionais: **CMake** (para compilação e gerenciamento de dependências)

