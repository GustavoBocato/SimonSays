#include "window.hpp"
#include <iostream>
#include <iterator>
#include <ostream>

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

    int i = 0;
    m_colorSequence.reserve(m_totalTurns);

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

// chamada toda vez antes de onpaint pela ABCg
void Window::onUpdate(){

    if (m_timer.elapsed() < m_colorAtualizationRate)
      return;
    m_timer.restart();

    if (m_colorSequence[m_showIndex] == "red"){
            setupModel(m_sides, 1.0, m_Red, m_CGreen, m_CBlue, m_CYellow);
    } else if (m_colorSequence[m_showIndex] == "green"){
            setupModel(m_sides,1.0, m_CRed, m_Green, m_CBlue, m_CYellow);
    } else if (m_colorSequence[m_showIndex] == "blue") {
            setupModel(m_sides,1.0, m_CRed, m_CGreen, m_Blue, m_CYellow);
    } else {
            setupModel(m_sides,1.0, m_CRed, m_CGreen, m_CBlue, m_Yellow);
    }
    if (m_turnTimer.elapsed() >= m_turnCount * m_colorAtualizationRate){
        playerTurn();
        m_turnCount ++;
        std::cout << "turno: " << m_turnCount << std::endl;
        m_turnTimer.restart();
    }
    m_showIndex++;
}

void Window::playerTurn(){
    std::cout << " da pra fazer o turno assim" << std::endl;
}

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
void Window::onPaintUI() {}

void Window::onResize(glm::ivec2 const &size) {
    m_viewportSize = size;
    abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
    abcg::glDeleteProgram(m_program);
    abcg::glDeleteBuffers(1, &m_vboVertices);
    abcg::glDeleteBuffers(1, &m_vboColors);
    abcg::glDeleteVertexArrays(1, &m_vao);
}
