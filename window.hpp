#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include "abcgOpenGL.hpp"
#include <array>
#include <random>
#include <string>

class Window : public abcg::OpenGLWindow {
protected:
  // metodos implementados em window.cpp
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
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
  std::default_random_engine m_randomEngine;

  // variaveis que a classe tem
  // vamos pensar melhor nelas, juntos

  enum class GameState { Play, Running, GameOver };
  GameState m_gameState; // guarda em qual estado o jogo esta

  // vira true quando player tem que dar input
  bool m_isPlayerTurn{true};

  // definindo cores padrao
  inline static const glm::vec4 m_Red{1.00f, 0.00f, 0.00f, 1.00f};
  inline static const glm::vec4 m_CRed{1.00f, 0.40f, 0.40f, 1.00f};
  inline static const glm::vec4 m_Green{0.00f, 1.00f, 0.00f, 1.00f};
  inline static const glm::vec4 m_CGreen{0.4f, 1.00f, 0.40f, 1.00f};
  inline static const glm::vec4 m_Blue{0.00f, 0.00f, 1.00f, 1.00f};
  inline static const glm::vec4 m_CBlue{0.40f, 0.40f, 1.00f, 1.00f};
  inline static const glm::vec4 m_Yellow{1.00f, 1.00f, 0.00f, 1.00f};
  inline static const glm::vec4 m_CYellow{1.00f, 1.00f, 0.60f, 1.00f};
  inline static const glm::vec4 m_Black{0.00f, 0.00f, 0.00f, 1.00f};

  // cria um array de cores, inicialmente vazio (cabe 50 cores)
  // vamos preencher aleatoriamente com cores, ao inicio do jogo
  std::vector<std::string> m_colorSequence{};
  void setupModel(int sides, glm::vec4 red, glm::vec4 green, glm::vec4 blue,
                  glm::vec4 yellow);
};

#endif
