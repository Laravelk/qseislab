#include "main/controller.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Main::Controller controller;

  return a.exec();
}

//#include <iostream>

// int bar(int x, char c, bool b) { return b ? x : static_cast<int>(c); }

// template <typename... Types> int foo(Types... args) { return bar(args...); }

// int main() {

//  std::cout << foo(1, 'a', false) << std::endl;

//  return 0;
//}
