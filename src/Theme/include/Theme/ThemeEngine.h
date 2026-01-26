// include/Theme/ThemeEngine.h
#pragma once

#include <QColor>
#include <QFont>
#include <QObject>
#include <QPalette>

namespace CodeWizard::Theme {

enum class ThemeType { Light, Dark, CatppuccinLatte, CatppuccinFrappé, CatppuccinMacchiato, CatppuccinMocha };

struct Colors
{
  // Base
  QColor background;
  QColor foreground;
  QColor disabledText;

  // Editor
  QColor editorBackground;
  QColor editorForeground;
  QColor editorSelection;
  QColor editorCurrentLine;

  // Sidebar
  QColor sidebarBackground;
  QColor sidebarTabActive;
  QColor sidebarTabInactive;
  QColor sidebarTabHover;
  QColor sidebarBorder;

  // Status Bar
  QColor statusBarBackground;
  QColor statusBarForeground;
  QColor statusBarError;

  // Widgets
  QColor buttonBackground;
  QColor buttonForeground;
  QColor buttonHover;
  QColor selectionBackground;
  QColor selectionForeground;
  QGradient::Preset lineNumberBackground;
  QColor lineNumberForeground;
};

struct Fonts
{
  QFont uiFont;
  QFont codeFont;
};

class ThemeEngine : public QObject
{
  Q_OBJECT

public:
  static ThemeEngine &instance();

  void setTheme(ThemeType type);
  [[nodiscard]] ThemeType currentTheme() const { return m_currentTheme; }

  // Getters
  [[nodiscard]] const Colors &colors() const { return m_colors; }
  [[nodiscard]] const Fonts &fonts() const { return m_fonts; }
  [[nodiscard]] QPalette palette() const { return m_palette; }

  void updateCatppuccinMocha();
  // Apply theme to application
  void applyToApplication();

signals:
  void themeChanged();

private:
  ThemeEngine();
  void updateLightTheme();
  void updateDarkTheme();
  void updatePalette();
  void updateCatppuccinLatte();
  void updateCatppuccinFrappé();
  void updateCatppuccinMacchiato();

  ThemeType m_currentTheme;
  Colors m_colors;
  Fonts m_fonts;
  QPalette m_palette;
};

}// namespace CodeWizard::Theme