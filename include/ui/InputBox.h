#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <allegro.h>
#include <string>

class InputBox {
  public:
    InputBox();
    InputBox(int x, int y, int width, int height, std::string value = "");
    virtual ~InputBox();

    void Focus();
    void Unfocus();

    std::string GetValue();
    void Update();
    void Draw(BITMAP *buffer);
  private:
    //Text input
    std::string text;
    unsigned int text_iter;

    int x, y;
    int height, width;
    bool focused;
    static const int MAX_LENGTH = 25;
};

#endif // INPUTBOX_H
