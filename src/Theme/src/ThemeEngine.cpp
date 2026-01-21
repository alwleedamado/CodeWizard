// src/Theme/ThemeEngine.cpp
#include "Theme/ThemeEngine.h"
#include <QApplication>
#include <QFontDatabase>

namespace CodeWizard::Theme {

ThemeEngine &ThemeEngine::instance()
{
  static ThemeEngine inst;
  return inst;
}

void ThemeEngine::setTheme(ThemeType type)
{
  m_currentTheme = type;

  switch (type) {
  case ThemeType::Light:
    updateLightTheme();
    break;
  case ThemeType::Dark:
    updateDarkTheme();
    break;
  case ThemeType::CatppuccinLatte:
    updateCatppuccinLatte();
    break;
  case ThemeType::CatppuccinFrappé:
    updateCatppuccinFrappé();
    break;
  case ThemeType::CatppuccinMacchiato:
    updateCatppuccinMacchiato();
    break;
  case ThemeType::CatppuccinMocha:
    updateCatppuccinMocha();
    break;
  }

  updatePalette();
  applyToApplication();
  emit themeChanged();
}

// Catppuccin Mocha (Darkest)
void ThemeEngine::updateCatppuccinMocha()
{
  m_colors = { .background = QColor(16, 16, 24),// base
    .foreground = QColor(202, 202, 226),// text
    .disabledText = QColor(118, 118, 138),// subtext0

    .editorBackground = QColor(16, 16, 24),
    .editorForeground = QColor(202, 202, 226),
    .editorSelection = QColor(103, 137, 255),// blue
    .editorCurrentLine = QColor(26, 26, 36),// surface0

    .sidebarBackground = QColor(16, 16, 24),
    .sidebarTabActive = QColor(26, 26, 36),
    .sidebarTabInactive = QColor(36, 36, 46),
    .sidebarTabHover = QColor(46, 46, 56),
    .sidebarBorder = QColor(66, 66, 76),

    .statusBarBackground = QColor(26, 26, 36),
    .statusBarForeground = QColor(202, 202, 226),
    .statusBarError = QColor(243, 139, 168),// red

    .buttonBackground = QColor(36, 36, 46),
    .buttonForeground = QColor(202, 202, 226),
    .buttonHover = QColor(46, 46, 56) };
}

void ThemeEngine::applyToApplication()
{
  QApplication::setPalette(m_palette);
  QApplication::setFont(m_fonts.uiFont);
}

ThemeEngine::ThemeEngine()
{
  // Set default fonts
  m_fonts.uiFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
  m_fonts.uiFont.setPointSize(10);

  m_fonts.codeFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  m_fonts.codeFont.setPointSize(11);
  setTheme(ThemeType::Light);
}
void ThemeEngine::updateLightTheme()
{
  m_colors = { // Base
    .background = QColor(240, 240, 240),
    .foreground = QColor(0, 0, 0),
    .disabledText = QColor(128, 128, 128),

    // Editor
    .editorBackground = QColor(255, 255, 255),
    .editorForeground = QColor(0, 0, 0),
    .editorSelection = QColor(180, 210, 255),
    .editorCurrentLine = QColor(245, 245, 245),

    // Sidebar
    .sidebarBackground = QColor(255, 255, 255),
    .sidebarTabActive = QColor(255, 255, 255),
    .sidebarTabInactive = QColor(232, 232, 232),
    .sidebarTabHover = QColor(220, 220, 220),
    .sidebarBorder = QColor(192, 192, 192),

    // Status Bar
    .statusBarBackground = QColor(224, 224, 224),
    .statusBarForeground = QColor(51, 51, 51),
    .statusBarError = QColor(211, 47, 47),

    // Widgets
    .buttonBackground = QColor(240, 240, 240),
    .buttonForeground = QColor(0, 0, 0),
    .buttonHover = QColor(220, 220, 220)
  };
}
void ThemeEngine::updateDarkTheme()
{
  m_colors = { // Base
    .background = QColor(40, 40, 40),
    .foreground = QColor(220, 220, 220),
    .disabledText = QColor(160, 160, 160),

    // Editor
    .editorBackground = QColor(30, 30, 30),
    .editorForeground = QColor(220, 220, 220),
    .editorSelection = QColor(60, 80, 120),
    .editorCurrentLine = QColor(45, 45, 45),

    // Sidebar
    .sidebarBackground = QColor(35, 35, 35),
    .sidebarTabActive = QColor(45, 45, 45),
    .sidebarTabInactive = QColor(50, 50, 50),
    .sidebarTabHover = QColor(60, 60, 60),
    .sidebarBorder = QColor(80, 80, 80),

    // Status Bar
    .statusBarBackground = QColor(50, 50, 50),
    .statusBarForeground = QColor(200, 200, 200),
    .statusBarError = QColor(239, 83, 80),

    // Widgets
    .buttonBackground = QColor(60, 60, 60),
    .buttonForeground = QColor(220, 220, 220),
    .buttonHover = QColor(80, 80, 80)
  };
}

void ThemeEngine::updatePalette()
{
  m_palette = QPalette();
  m_palette.setColor(QPalette::Window, m_colors.background);
  m_palette.setColor(QPalette::WindowText, m_colors.foreground);
  m_palette.setColor(QPalette::Base, m_colors.editorBackground);
  m_palette.setColor(QPalette::AlternateBase, m_colors.editorCurrentLine);
  m_palette.setColor(QPalette::ToolTipBase, m_colors.editorBackground);
  m_palette.setColor(QPalette::ToolTipText, m_colors.foreground);
  m_palette.setColor(QPalette::Text, m_colors.foreground);
  m_palette.setColor(QPalette::Button, m_colors.buttonBackground);
  m_palette.setColor(QPalette::ButtonText, m_colors.buttonForeground);
  m_palette.setColor(QPalette::BrightText, Qt::red);
  m_palette.setColor(QPalette::Link, QColor(42, 130, 218));
  m_palette.setColor(QPalette::Highlight, m_colors.editorSelection);
  m_palette.setColor(QPalette::HighlightedText, m_colors.foreground);
}

// Catppuccin Latte (Light)
void ThemeEngine::updateCatppuccinLatte()
{
  m_colors = { .background = QColor(239, 241, 245),// base
    .foreground = QColor(76, 79, 105),// text
    .disabledText = QColor(138, 142, 163),// subtext0

    .editorBackground = QColor(239, 241, 245),
    .editorForeground = QColor(76, 79, 105),
    .editorSelection = QColor(186, 213, 255),// blue surface
    .editorCurrentLine = QColor(229, 231, 235),// surface0

    .sidebarBackground = QColor(239, 241, 245),
    .sidebarTabActive = QColor(255, 255, 255),
    .sidebarTabInactive = QColor(229, 231, 235),
    .sidebarTabHover = QColor(219, 221, 225),
    .sidebarBorder = QColor(209, 211, 215),

    .statusBarBackground = QColor(229, 231, 235),
    .statusBarForeground = QColor(76, 79, 105),
    .statusBarError = QColor(221, 83, 71),// red

    .buttonBackground = QColor(229, 231, 235),
    .buttonForeground = QColor(76, 79, 105),
    .buttonHover = QColor(219, 221, 225) };
}

// Catppuccin Frappé (Dark)
void ThemeEngine::updateCatppuccinFrappé()
{
  m_colors = { .background = QColor(30, 30, 46),// base
    .foreground = QColor(202, 202, 226),// text
    .disabledText = QColor(138, 142, 163),// subtext0

    .editorBackground = QColor(30, 30, 46),
    .editorForeground = QColor(202, 202, 226),
    .editorSelection = QColor(103, 137, 255),// blue
    .editorCurrentLine = QColor(41, 41, 59),// surface0

    .sidebarBackground = QColor(30, 30, 46),
    .sidebarTabActive = QColor(41, 41, 59),
    .sidebarTabInactive = QColor(51, 51, 69),
    .sidebarTabHover = QColor(61, 61, 79),
    .sidebarBorder = QColor(81, 81, 99),

    .statusBarBackground = QColor(41, 41, 59),
    .statusBarForeground = QColor(202, 202, 226),
    .statusBarError = QColor(243, 139, 168),// red

    .buttonBackground = QColor(51, 51, 69),
    .buttonForeground = QColor(202, 202, 226),
    .buttonHover = QColor(61, 61, 79) };
}
// Catppuccin Macchiato (Darker)
void ThemeEngine::updateCatppuccinMacchiato()
{
  m_colors = { .background = QColor(24, 24, 37),// base
    .foreground = QColor(202, 202, 226),// text
    .disabledText = QColor(128, 128, 148),// subtext0

    .editorBackground = QColor(24, 24, 37),
    .editorForeground = QColor(202, 202, 226),
    .editorSelection = QColor(103, 137, 255),// blue
    .editorCurrentLine = QColor(34, 34, 49),// surface0

    .sidebarBackground = QColor(24, 24, 37),
    .sidebarTabActive = QColor(34, 34, 49),
    .sidebarTabInactive = QColor(44, 44, 59),
    .sidebarTabHover = QColor(54, 54, 69),
    .sidebarBorder = QColor(74, 74, 89),

    .statusBarBackground = QColor(34, 34, 49),
    .statusBarForeground = QColor(202, 202, 226),
    .statusBarError = QColor(243, 139, 168),// red

    .buttonBackground = QColor(44, 44, 59),
    .buttonForeground = QColor(202, 202, 226),
    .buttonHover = QColor(54, 54, 69) };
}

}// namespace CodeWizard::Theme