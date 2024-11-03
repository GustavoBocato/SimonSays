#include "window.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setOpenGLSettings({.samples = 2, .doubleBuffering = false});
    window.setWindowSettings(
        {.width = 600, .height = 600, .title = "Simon Says"});

    app.run(window);
  } catch (std::exception const &e) {
    fmt::print(stderr, "{}\n", e.what());
    return -1;
  }
  return 0;
}