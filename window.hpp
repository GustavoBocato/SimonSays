#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include "abcgOpenGL.hpp"
#include <SDL_events.h>
#include <random>
#include <string>

class Window : public abcg::OpenGLWindow {
protected:
    // metodos implementados em window.cpp
    void onCreate() override;
    void onPaint() override;
    void onUpdate() override;
    void onPaintUI() override;
    void onResize(glm::ivec2 const &size) override;
    void onEvent(SDL_Event const &event) override;
    void onDestroy() override;

private:
    // utilizado no onResize
    glm::ivec2 m_viewportSize{};

    /*
    recursos alocados pelo OpenGL -> shaders que definem o comportamento da
    renderização

    */

    GLuint m_vao{};
    GLuint m_vboVertices{};
    GLuint m_vboColors{};
    GLuint m_program{};

    abcg::Timer m_timer;
    abcg::Timer m_turnTimer;
    abcg::Timer m_playerChooseTimer;

    int m_turnCount = 1;
    int m_totalTurns = 50;
    int m_showIndex = 0;
    int const m_sides = 180;

    float const m_colorAtualizationRate = 1.0/1;
    std::default_random_engine m_randomEngine;


    enum class EndCondition { Win, Lose };
    enum class GameState { Running, Stoped };
    GameState m_gameState; // guarda em qual estado o jogo esta

    std::string endText;
    // vira true quando player tem que dar input
    bool m_isPlayerTurn{true};
    int m_playerChoice = -1;


    // definindo cores padrao
    inline static const glm::vec4 m_Red{1.00f, 0.00f, 0.00f, 1.00f};
    inline static const glm::vec4 m_CRed{0.60f, 0.00f, 0.00f, 1.00f};

    inline static const glm::vec4 m_Green{0.00f, 0.85f, 0.22f, 1.00f};
    inline static const glm::vec4 m_CGreen{0.00f, 0.50f, 0.22f, 1.00f};
    
    inline static const glm::vec4 m_Blue{0.00f, 0.00f, 1.00f, 1.00f};
    inline static const glm::vec4 m_CBlue{0.00f, 0.00f, 0.60f, 1.00f};
    
    inline static const glm::vec4 m_Yellow{1.00f, 1.00f, 0.00f, 1.00f};
    inline static const glm::vec4 m_CYellow{0.60f, 0.60f, 0.00f, 1.00f};

    inline static const glm::vec4 m_Black{0.00f, 0.00f, 0.00f, 1.00f};

    // cria um array de cores, inicialmente vazio (cabe 50 cores)
    // vamos preencher aleatoriamente com cores, ao inicio do jogo
    std::vector<std::string> m_colorSequence;
    void setupModel(int sides, float radius ,glm::vec4 red, glm::vec4 green, glm::vec4 blue,
                    glm::vec4 yellow);
    void playerTurn(std::vector<int> &gameChoices, int turnCount);
    int findQuadrant();
    void endGame(EndCondition condition);
};

#endif
