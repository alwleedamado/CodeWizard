#include <Runtime/Application.h>

int main(int argc, char **argv)
{
  auto app = CodeWizard::Runtime::Application::instance();
  return app.run(argc, argv);
}
