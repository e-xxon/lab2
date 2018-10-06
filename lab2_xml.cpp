#include <iostream>
#include <stack>
#include <glib.h>
#include <libxml++/libxml++.h>
#include <iomanip>

int k = 0;
struct city
{
    Glib::ustring district, address,amount ;
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
        v_address.address =  second_currency;
        v_address.amount = "-1";
		create.amount = first_currency;
		create.address = second_currency;
    }

    virtual ~CParser() {}

    void print_result_District()
    {  
        std::cout << "District " <<  v_district.district << "   " << "Amount of surveillance cameras = " << k << std::endl;
    }

	void print_result_Address()
	{	
        int amount_address = std::stod(v_address.amount.substr(0, v_address.amount.length()));
			std::cout <<  "Address " << v_address.address << "   " << "Amount of surveillance cameras = " << amount_address << std::endl;
	}



protected:
    virtual void on_start_document() override
    {

    }

    virtual void on_end_document() override
    {

    }

    virtual void on_start_element(const Glib::ustring& name,
                                  const AttributeList& properties) override
    {
        tags.push(name);

        if (name == "addresses_camera_installation")
        {	
            current_info.district.clear();
            current_info.address.clear();
            current_info.amount.clear();
        }
    }

    virtual void on_end_element(const Glib::ustring& name) override
    {	last = current_info;
       tags.pop();
		if ((name == "dataset" )&& ( create.amount == create.address)  )
			{
			k+= std::stod(last.amount.substr(0,current_info.amount.length()));
			std::cout <<"District " << last.district << "  " <<"Amount of surveillance cameras = "<< k << std::endl;		
			}
        if (name == "addresses_camera_installation")
        {	 if ( create.amount == create.address )
			{	if (v_district.district =="-1")
				{
					v_district = current_info;
				}
           		else if (current_info.district == v_district.district)
				{ 	
                	
					k += std::stod(v_district.amount.substr(0,current_info.amount.length()));
					v_district = current_info;
				}

					else  
					{
						k += std::stod(v_district.amount.substr(0,current_info.amount.length()));
						create.district = v_district.district;
               			v_district = current_info;
							
						std::cout << "District " << create.district << "  " <<"Amount of surveillance cameras = " << k << std::endl;
						k = 0;
					}
			}
			 else if (current_info.district == v_district.district)
            {
                v_district = current_info;
				k += std::stod(current_info.amount.substr(0,current_info.amount.length()));
            }
            else if (current_info.address == v_address.address)
            {
                v_address = current_info;
            }

        }
    }

    virtual void on_characters(const Glib::ustring& chars) override
    {
        if (tags.top() == "district")
        {
            current_info.district = chars;
        }
        else if (tags.top() == "address")
        {
            current_info.address = chars;
        }
        else if (tags.top() == "amount")
        {
            current_info.amount = chars;
        }


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
	else if(district == "-1")
	{
		CParser parser(district, address);
    	parser.parse_file(filename);
		parser.print_result_Address();
	}
    return 0;
}
