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

```cpp
void Window::onCreate() {
    auto const path{abcg::Application::getAssetsPath()};
    m_program =
      abcg::createOpenGLProgram({{.source = path + "UnlitVertexColor.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = path + "UnlitVertexColor.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
    abcg::glClearColor(0, 0, 0, 1);
    abcg::glClear(GL_COLOR_BUFFER_BIT);
    auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
    m_randomEngine.seed(seed);

    m_gameState = GameState::Running;
    int i = 0;
    m_colorSequence.reserve(m_totalTurns);

    m_turnTimer.restart();
    while (i < m_totalTurns){
        std::uniform_int_distribution<int> distribuicao(0, 1000);
        int numero = distribuicao(m_randomEngine) % 4;
        switch (numero) {
            case 0:
                m_colorSequence.push_back("red");
                break;
            case 1:
                m_colorSequence.push_back("green");
                break;
            case 2:
                m_colorSequence.push_back("blue");
                break;
            default:
                m_colorSequence.push_back("yellow");
        }
        i++;
    }
}
```

A função onPaint() é chamada a cada quadro, sendo ela responsável por pintar os círculos que são desenhados a partir de um Vertex Array Object (VAO) (que é um um array que tem acesso a coordenadas e cores de uma sequência de pontos) armazenado na variável global chamada de m_vao. Essa função chama a função glDrawArrays() que desenha triângulos conectando dois a dois os pontos da circumferência do círculo com o ponto central do círculo, esses triângulos ficam com a base apontando para fora do círculo e são coloridos a partir das cores associadas aos pontos da circumferência.

```cpp
void Window::onPaint() {
    abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

    abcg::glUseProgram(m_program);
    abcg::glBindVertexArray(m_vao);
    
    // desenha as cores
    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, m_sides + 2);
    
    // desenha circulo preto no meio
    setupModel(m_sides, 0.43,m_Black, m_Black, m_Black, m_Black); 
    abcg::glBindVertexArray(m_vao);
    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, m_sides + 2);

    abcg::glBindVertexArray(0);
    abcg::glUseProgram(0);
}
```

A função responsável por criar esse VAO usado em onPaint() é o setupModel(), que cria uma sequência de 182 coordenadas e 182 cores, as coordenadas são os pontos da circumferência do círculo e as cores são designadas para as coordenadas a partir do quadrante em que a coordenada se encontra que é determinado pelo ângulo da coordenada no círculo (por exemplo se o ângulo for entre 0 e π/2 então a coordenada está no primeiro quadrante).  

```cpp
void Window::setupModel(int sides,float radius, glm::vec4 red, glm::vec4 green,
                        glm::vec4 blue, glm::vec4 yellow) {

    // Generate a new VBO and get the associated ID
    abcg::glGenBuffers(1, &m_vboVertices);
    // Bind VBO in order to use it
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);

    std::vector<glm::vec2> positions{{0, 0}};
    auto const step{M_PI * 2 / sides};
    for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
        positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }
    positions.push_back(positions.at(1));

    std::vector<glm::vec4> colors{m_Black};
    auto const stepColor{M_PI * 2 / sides};
    for (auto const angle : iter::range(0.0, M_PI * 2, stepColor)) {
        if (angle <= M_PI / 2 && angle >= 0.0f) {
          colors.emplace_back(red);
        } else if (angle > M_PI / 2 && angle <= M_PI) {
          colors.emplace_back(green);
        } else if (angle > M_PI && angle <= 3.0 * M_PI / 2) {
          colors.emplace_back(blue);
        } else {
          colors.emplace_back(yellow);
        }
    }
    colors.push_back(colors.at(1));
    // Upload data to VBO
    abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
    // Unbinding the VBO is allowed (data can be released now)
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glGenBuffers(1, &m_vboColors);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4),
                     colors.data(), GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
    auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

    // Create VAO
    abcg::glGenVertexArrays(1, &m_vao);

    // Bind vertex attributes to current VAO
    abcg::glBindVertexArray(m_vao);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glEnableVertexAttribArray(colorAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    abcg::glBindVertexArray(0);
}
```

A animação de piscar dos botões é feita ao se chamar a função setupModel() dentro da função onUpdate(), com argumentos diferentes para o círculo colorido, ou melhor passando-se cores mais claras para os botões que piscam. A sequência de botões que piscam é definida pela sequência de cores armazenadas no vetor m_colorSequence.

```cpp
// chamada toda vez antes de onpaint pela ABCg
void Window::onUpdate(){

    if (m_gameState == GameState::Stoped){
        setupModel(m_sides, 1.0,m_CRed, m_CGreen, m_CBlue, m_CYellow); 
    } else {
        if (m_timer.elapsed() < m_colorAtualizationRate)
          return;

        m_timer.restart();

        std::vector<int> gameChoices;
        
      if (m_turnTimer.elapsed() >= m_turnCount * m_colorAtualizationRate){
            fmt::print("entrou no turno do player\n");
            playerTurn(gameChoices, m_turnCount);
            m_turnCount ++;
            m_showIndex = 0;
            m_turnTimer.restart();
        }
        
        if (m_colorSequence[m_showIndex] == "red"){
            gameChoices.push_back(1);
            fmt::print("push 1\n");
            setupModel(m_sides, 1.0, m_Red, m_CGreen, m_CBlue, m_CYellow);
        } else if (m_colorSequence[m_showIndex] == "green") {
            gameChoices.push_back(2);
            fmt::print("push 2\n");
            setupModel(m_sides,1.0, m_CRed, m_Green, m_CBlue, m_CYellow);
        } else if (m_colorSequence[m_showIndex] == "blue") {
            gameChoices.push_back(3);
            fmt::print("push 3\n");
            setupModel(m_sides,1.0, m_CRed, m_CGreen, m_Blue, m_CYellow);
        } else {
            gameChoices.push_back(4);
            fmt::print("push 4\n");
            setupModel(m_sides,1.0, m_CRed, m_CGreen, m_CBlue, m_Yellow);
        }
        m_showIndex++;


      if(m_turnCount > m_totalTurns) {
            endGame(EndCondition::Win);
        }
    }
}
```

A função onEvent() lida com o input do jogador pelo mouse. O input do jogador é comparado com as entradas do vetor m_colorSequence que armazena a sequência aleatória de cores, na função playerTurn(). Que por sua vez chama a função endGame() passando em seu argumento um valor que indica se o jogador foi vencedor ou não, encerrando o jogo com uma mensagem apropriada. (*)

(*): Essas funcionaidade ainda não está pronta.

## Tecnologias Utilizadas

- Linguagem: C++
- Biblioteca gráfica: **ABCg** (para renderização de gráficos)
- Ferramentas adicionais: **CMake** (para compilação e gerenciamento de dependências)

## Integrantes da equipe 3

- Pedro de Luca Occulate Serra; RA: 21044215
- Gabriela Sampaio Magiri; RA: 11201922191
- Gustavo da Silva Bocato; RA: 11201921878
