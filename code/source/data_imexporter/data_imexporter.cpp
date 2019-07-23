#include "header/data_imexporter/data_imexporter.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"


DATA_IMEXPORTER::DATA_IMEXPORTER()
{
    set_base_frequency_in_Hz(50.0);
    set_export_zero_impedance_line_logic(true);
    set_export_zero_impedance_line_logic(true);
}

DATA_IMEXPORTER::~DATA_IMEXPORTER()
{
    ;
}

void DATA_IMEXPORTER::set_base_frequency_in_Hz(double fbase)
{
    base_frequency_in_Hz = fbase;
}

double DATA_IMEXPORTER::get_base_frequency_in_Hz() const
{
    return base_frequency_in_Hz;
}


void DATA_IMEXPORTER::set_export_zero_impedance_line_logic(bool logic)
{
    export_zero_impedance_line = logic;
}

bool DATA_IMEXPORTER::get_export_zero_impedance_line_logic() const
{
    return export_zero_impedance_line;
}

void DATA_IMEXPORTER::set_export_out_of_service_bus_logic(bool logic)
{
    export_out_of_service_bus = logic;
}

bool DATA_IMEXPORTER::get_export_out_of_service_bus_logic() const
{
    return export_out_of_service_bus;
}

void DATA_IMEXPORTER::export_shadowed_bus_pair(string file) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. Shadowed bus pair file "<<file<<" cannot be opened for exporting shadowed bus data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    map<size_t, vector<size_t> > bus_pairs;

    bus_pairs.clear();

    vector<BUS*> buses = psdb.get_all_buses();
    size_t n = buses.size();
    for(size_t i=0; i!=n; ++i)
    {
        BUS* bus = buses[i];
        size_t bus_number = bus->get_bus_number();
        size_t equiv_bus_number = bus->get_equivalent_bus_number();
        if(equiv_bus_number!=0 and bus_number!=equiv_bus_number)
        {
            map<size_t, vector<size_t> >::iterator iter = bus_pairs.begin();
            iter = bus_pairs.find(equiv_bus_number);
            if(iter!=bus_pairs.end())
            {
                (iter->second).push_back(bus_number);
            }
            else
            {
                vector<size_t> new_pair;
                new_pair.push_back(bus_number);
                bus_pairs.insert(pair<size_t, vector<size_t> >(equiv_bus_number, new_pair));
            }
        }
    }
    for(map<size_t, vector<size_t> >::const_iterator iter = bus_pairs.begin(); iter!=bus_pairs.end(); ++iter)
    {
        size_t equiv_bus_number = iter->first;
        vector<size_t> bus_pair = iter->second;
        size_t n = bus_pair.size();
        if(n==0)
            continue;
        ostringstream oss;
        oss<<equiv_bus_number<<":";
        for(size_t i=0; i!=(n-1); ++i)
            oss<<bus_pair[i]<<",";
        oss<<bus_pair[n-1]<<"\n";
        ofs<<oss.str();
    }
    ofs.close();
}


bool DATA_IMEXPORTER::is_valid() const
{
    return true;
}

void DATA_IMEXPORTER::check()
{
    ;
}

void DATA_IMEXPORTER::clear()
{
    ;
}
