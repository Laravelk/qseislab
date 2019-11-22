//#include "main/controller.h"

//#include <QApplication>

// int main(int argc, char *argv[]) {
//  QApplication a(argc, argv);

//  Main::Controller controller;

//  return a.exec();
//}

#include <iostream>

class A {
public:
  A(int x) : _x(x) {}

  int getx() const { return _x; }

  int const *fun() const {
    std::cout << "call const" << std::endl;
    return &_x;
  }

  int *fun() {
    std::cout << "call int" << std::endl;
    return &_x;
  }

private:
  int _x;
};

int main() {

  A a(2);

  int const *ptr = a.fun();

  int *ptr2 = a.fun();

  auto ptr3 = static_cast<const A>(a).fun();

  //  auto ptr2 = a2.fun();
  //  *ptr2 = -1;
  //  std::cout << "2: " << a2.getx() << std::endl;
}
