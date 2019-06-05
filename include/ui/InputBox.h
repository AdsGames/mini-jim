#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <allegro.h>
#include <string>

class InputBox {
  public:
    InputBox();
    InputBox(int x, int y, int width, int height, std::string value = "", std::string type = "text");
    virtual ~InputBox();

    void Focus();

    std::string GetValue();
    void Update();
    void Draw(BITMAP *buffer);
    bool Hover();
  private:
    //Text input
    std::string text;
    std::string type;
    unsigned int text_iter;

    int x, y;
    int height, width;
    bool focused;
    static const int MAX_LENGTH = 25;
};

#endif // INPUTBOX_H
