#include "UI/UIApplication.h"
#include "UI/MainWindow.h"
#include "Theme/ThemeEngine.h"
#include <QApplication>

namespace CodeWizard::UI {

int UIApplication::run(int argc, char** argv)
{
  QApplication app(argc, argv);
  auto& theme = Theme::ThemeEngine::instance();
  theme.setTheme(Theme::ThemeType::CatppuccinMocha); // or load from config
  theme.applyToApplication();
  m_window = new MainWindow();
  m_window->show();
  return app.exec();
}

}// namespace CodeWizard::UI