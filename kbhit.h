#pragma once
#include <sys/ioctl.h>
#include <termios.h>

extern "C" bool kbhit() //repl.it runs on linux so you must create the function kbhit() (which is only avaliable on windows) to detect if a key is pressed
{
  termios term;
  tcgetattr(0, &term);

  termios term2 = term;
  term2.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &term2);
  int byteswaiting;
  ioctl(0, FIONREAD, &byteswaiting);
  tcsetattr(0, TCSANOW, &term);
  return byteswaiting > 0;
}