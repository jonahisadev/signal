#include <iostream>
#include "signal.h"

class Counter : public SignalObject
{
private:
    int _value;

public:
    Counter()
    {
        _value = 0;
    }

    int value() const { return _value; }
    void setValue(int value);

    SLOT valueChanged(int nValue)
    {
        _value = nValue;
    }
};

void Counter::setValue(int value)
{
    if (value != _value) {
        _value = value;
        emit("value_changed", value);
    }
}

int main(int argc, char** argv)
{
    Counter a, b;
    a.connect("value_changed", &b, &Counter::valueChanged);

    a.setValue(42);
    std::cout << a.value() << ", " << b.value() << std::endl;

    b.setValue(1209);
    std::cout << a.value() << ", " << b.value() << std::endl;

    return 0;
}