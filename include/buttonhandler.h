#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H


class ButtonHandler
{
protected:
    bool done {false};
public:
    ButtonHandler() = default;
    ~ButtonHandler(){}
    void stop(){done = true;}
};

#endif // BUTTONHANDLER_H
