#include "header/model/sg_models/exciter_model/SEXS.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
SEXS::SEXS()
{
    clear();
}

SEXS::~SEXS()
{
    ;
}

void SEXS::clear()
{
    prepare_model_variable_table();

    exciter.set_limiter_type(WINDUP_LIMITER);
}

void SEXS::copy_from_const_model(const SEXS& model)
{
    clear();

    //this->set_power_system_database(model.get_default_power_system_database());
    //this->set_device_id(model.get_device_id());

    this->set_TA_in_s(model.get_TA_in_s());
    this->set_TB_in_s(model.get_TB_in_s());
    this->set_K(model.get_K());
    this->set_TE_in_s(model.get_TE_in_s());
    this->set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    this->set_Efdmin_in_pu(model.get_Efdmin_in_pu());

}
SEXS::SEXS(const SEXS& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

SEXS& SEXS::operator=(const SEXS& model)
{
    if(this == &model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string SEXS::get_model_name() const
{
    return "SEXS";
}

double SEXS::get_model_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double SEXS::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void SEXS::set_model_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void SEXS::set_model_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void SEXS::set_TA_in_s(double T)
{
    phase_tuner.set_T1_in_s(T);
}

void SEXS::set_TB_in_s(double T)
{
    phase_tuner.set_T2_in_s(T);
}

void SEXS::set_K(double K)
{
    exciter.set_K(K);
}

void SEXS::set_TE_in_s(double T)
{
    exciter.set_T_in_s(T);
}

void SEXS::set_Efdmax_in_pu(double efd)
{
    exciter.set_upper_limit(efd);
}

void SEXS::set_Efdmin_in_pu(double efd)
{
    exciter.set_lower_limit(efd);
}

double SEXS::get_TA_in_s() const
{
    return phase_tuner.get_T1_in_s();
}

double SEXS::get_TB_in_s() const
{
    return phase_tuner.get_T2_in_s();
}

double SEXS::get_K() const
{
    return exciter.get_K();
}

double SEXS::get_TE_in_s() const
{
    return exciter.get_T_in_s();
}

double SEXS::get_Efdmax_in_pu() const
{
    return exciter.get_upper_limit();
}

double SEXS::get_Efdmin_in_pu() const
{
    return exciter.get_lower_limit();
}

bool SEXS::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool SEXS::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<8)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;


    double ta_over_tb, ta, tb, k, te, emax, emin;

    size_t i=3;
    ta_over_tb = get_double_data(dyrdata[i],"0.0"); i++;
    tb = get_double_data(dyrdata[i],"0.0"); i++;
    k = get_double_data(dyrdata[i],"0.0"); i++;
    te = get_double_data(dyrdata[i],"0.0"); i++;
    emin = get_double_data(dyrdata[i],"0.0"); i++;
    emax = get_double_data(dyrdata[i],"0.0"); i++;
    ta = ta_over_tb*tb;

    set_TA_in_s(ta);
    set_TB_in_s(tb);
    set_K(k);
    set_TE_in_s(te);
    set_Efdmin_in_pu(emin);
    set_Efdmax_in_pu(emax);

    is_successful = true;

    return is_successful;
}

bool SEXS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void SEXS::initialize()
{
    ostringstream osstream;
    if(is_model_initialized())
        return;

    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return;
    if(not gen_model->is_model_initialized())
        gen_model->initialize();

    double Ecomp = get_compensated_voltage_in_pu();

    double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();

    if(Efd>get_Efdmax_in_pu())
    {
        osstream<<"Initialization error. Efd of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"Efd is "<<Efd<<", and Efdmax is "<<get_Efdmax_in_pu()<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    if(Efd<get_Efdmin_in_pu())
    {
        osstream<<"Initialization error. Efd of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
          <<"Efd is "<<Efd<<", and Efdmin is "<<get_Efdmin_in_pu()<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    exciter.set_output(Efd);
    exciter.initialize();

    double input = exciter.get_input();
    phase_tuner.set_output(input);
    phase_tuner.initialize();

    input = phase_tuner.get_input();

    double Vref = input+Ecomp;
    set_voltage_reference_in_pu(Vref);

    set_flag_model_initialized_as_true();
}

void SEXS::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    double Ecomp = get_compensated_voltage_in_pu();
    double Vref = get_voltage_reference_in_pu();
    double Vs = get_stabilizing_signal_in_pu();

    double input = Vref-Ecomp+Vs;

    phase_tuner.set_input(input);
    phase_tuner.run(mode);

    input = phase_tuner.get_output();
    exciter.set_input(input);
    exciter.run(mode);

    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double SEXS::get_excitation_voltage_in_pu() const
{
    return exciter.get_output();
}
void SEXS::check()
{
    ;
}

void SEXS::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}

void SEXS::save()
{
    ;
}

string SEXS::get_standard_model_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();
    double TA = get_TA_in_s();
    double TB = get_TB_in_s();
    double K = get_K();
    double TE = get_TE_in_s();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<TA/TB<<", "
      <<setw(8)<<setprecision(6)<<TB<<", "
      <<setw(8)<<setprecision(6)<<K<<", "
      <<setw(8)<<setprecision(6)<<TE<<", "
      <<setw(8)<<setprecision(6)<<Efdmax<<", "
      <<setw(8)<<setprecision(6)<<Efdmin<<"  /";
    return osstream.str();
}

void SEXS::prepare_model_variable_table()
{
    size_t i=0;
    add_model_variable_name_and_index_pair("STATE@PHASE TUNER", i); i++;
    add_model_variable_name_and_index_pair("STATE@EXCITER", i); i++;
}

double SEXS::get_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@PHASE TUNER")
        return phase_tuner.get_state();

    if(var_name == "STATE@EXCITER")
        return exciter.get_state();

    return 0.0;
}


string SEXS::get_dynamic_data_in_psse_format() const
{
    return "";
}

string SEXS::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string SEXS::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
