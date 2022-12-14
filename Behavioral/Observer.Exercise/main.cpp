#include "stock.hpp"

using namespace std;

int main()
{
    Stock misys("Misys", 340.0);
    Stock ibm("IBM", 245.0);
    Stock tpsa("TPSA", 95.0);

    // rejestracja inwestorow zainteresowanych powiadomieniami o zmianach kursu spolek
    Investor solorz("Solorz");

    ibm.price_changed.connect([&solorz](const std::string& s, double p) { solorz.update(s, p); });
    auto conn = tpsa.price_changed.connect([&solorz](const std::string& s, double p) { solorz.update(s, p); });

    // zmian kursow
    misys.set_price(360.0);
    ibm.set_price(210.0);
    tpsa.set_price(45.0);

    conn.disconnect();

    misys.set_price(380.0);
    ibm.set_price(230.0);
    tpsa.set_price(15.0);
}
