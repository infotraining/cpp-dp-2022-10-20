#include "state.hpp"

int main()
{
    // Setup context in a state
    Context c;

    // Issue requests, which toggles state
    c.request();
    c.request();
    c.request();
    c.request();
}
