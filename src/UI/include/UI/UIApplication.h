#pragma once
#include "MainWindow.h"

namespace CodeWizard::UI {

class UIApplication
{
public:
  int run(int argc, char** argv);
private:
  MainWindow* m_window;
};

}// namespace CodeWizard::UI
