#include <iostream>
#include <stack>
#include <glib.h>
#include <libxml++/libxml++.h>
#include <iomanip>

int k = 0;
struct city
{
    Glib::ustring district, address, amount;
};

class CParser : public xmlpp::SaxParser
{
private:
    city v_district, v_address, current_info, create, last;
    std::stack<Glib::ustring> tags;

public:
    CParser(Glib::ustring& first_currency, Glib::ustring& second_currency)
    {
        v_district.district = first_currency;
        v_district.amount = "-1";
        v_address.address = second_currency;
        v_address.amount = "-1";
        create.amount = first_currency;
        create.address = second_currency;
    }

    virtual ~CParser()
    {
    }

    void print_result_District()
    {
        std::cout << "District " << v_district.district << "   "
                  << "Amount of surveillance cameras = " << k << std::endl;
    }

    void print_result_Address()
    {
        int amount_address = std::stod(v_address.amount.substr(0, v_address.amount.length()));
        std::cout << "Address " << v_address.address << "   "
                  << "Amount of surveillance cameras = " << amount_address << std::endl;
    }
};


int main(int argc, char** argv)
{
    Glib::ustring filename("city.xml");
    Glib::ustring district("-1"), address("-1");

    std::locale::global(std::locale(""));

    for (int i = 1; i < argc - 1; ++i)
    {
        Glib::ustring arg = argv[i];
        if (arg == "--file")
            filename = argv[++i];
        else if (arg == "--district")
            district = argv[++i];
        else if (arg == "--address")
            address = argv[++i];
    }

    if (address == district)
    {
        CParser parser(district, address);
        parser.parse_file(filename);
    }
    else if (address == "-1")
    {
        CParser parser(district, address);
        parser.parse_file(filename);
        parser.print_result_District();
    }
    else if (district == "-1")
    {
        CParser parser(district, address);
        parser.parse_file(filename);
        parser.print_result_Address();
    }
    return 0;
}
