#include "UI/UIApplication.h"
#include "UI/MainWindow.h"
#include "Theme/ThemeEngine.h"
#include <QApplication>
#include <QTimer>
namespace CodeWizard::UI {

int UIApplication::run(int argc, char** argv)
{
  QApplication app(argc, argv);
  auto& theme = Theme::ThemeEngine::instance();
  theme.setTheme(Theme::ThemeType::CatppuccinMocha); // or load from config
  theme.applyToApplication();
  Editor::TreeSitterManager::instance().initialize(":/queries");
  m_window = new MainWindow();
  m_window->show();
  QTimer::singleShot(0, []() {
    EventBus<IncChangeEvent>::poll();
    EventBus<CursorMovedEvent>::poll();
    EventBus<SelectionEvent>::poll();
  });
  return app.exec();
}

}// namespace CodeWizard::UI