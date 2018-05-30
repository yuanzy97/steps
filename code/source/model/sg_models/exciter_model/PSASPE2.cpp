#include "header/model/sg_models/exciter_model/PSASPE2.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <iostream>

using namespace std;

static vector<string> MODEL_VARIABLE_TABLE{ "EXCITATION VOLTAGE",      //0
                                            "VOLTAGE REFERENCE",      //1
                                            "PSASPE2ENSATED VOLTAGE",      //2
                                            "STABILIZING SIGNAL",      //3
                                            "STATE@SENSOR",     //4
                                            "STATE@TUNER1",     //5
                                            "STATE@TUNER2",     //5
                                            "STATE@REGULATOR"     //5
                                            };

PSASPE2::PSASPE2()
{
    clear();
}

PSASPE2::~PSASPE2()
{
    ;
}

void PSASPE2::clear()
{
    sensor.set_limiter_type(NO_LIMITER);
    tuner1_lead_lag.set_limiter_type(NO_LIMITER);
    tuner1_pi.set_limiter_type(NO_LIMITER);
    tuner2.set_limiter_type(NO_LIMITER);
    regulator.set_limiter_type(NO_LIMITER);
    set_K2(true);
}
void PSASPE2::copy_from_const_model(const PSASPE2& model)
{
    clear();
    this->set_KR(model.get_KR());
    this->set_TR_in_s(model.get_TR_in_s());
    this->set_K2(model.get_K2());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_T4_in_s(model.get_T4_in_s());
    this->set_KA(model.get_KA());
    this->set_TA_in_s(model.get_TA_in_s());
    this->set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    this->set_Efdmin_in_pu(model.get_Efdmin_in_pu());
    this->set_Vta_in_pu(model.get_Vta_in_pu());
    this->set_Vtb_in_pu(model.get_Vtb_in_pu());
    this->set_Kpt(model.get_Kpt());
    this->set_Kit(model.get_Kit());
    this->set_Ke(model.get_Ke());
}
PSASPE2::PSASPE2(const PSASPE2& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

PSASPE2& PSASPE2::operator=(const PSASPE2& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string PSASPE2::get_model_name() const
{
    return "PSASPE2";
}

double PSASPE2::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double PSASPE2::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void PSASPE2::set_double_data_with_index(size_t index, double value)
{
    if(index==0)
        return;
}

void PSASPE2::set_double_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return;
}

void PSASPE2::set_KR(double K)
{
    sensor.set_K(K);
}

void PSASPE2::set_TR_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void PSASPE2::set_K2(bool K)
{
    this->K2 = K;
}

void PSASPE2::set_T1_in_s(double T)
{
    tuner1_lead_lag.set_T1_in_s(T);
}

void PSASPE2::set_T2_in_s(double T)
{
    tuner1_lead_lag.set_T2_in_s(T);
}

void PSASPE2::set_T3_in_s(double T)
{
    tuner2.set_T1_in_s(T);
}

void PSASPE2::set_T4_in_s(double T)
{
    tuner2.set_T2_in_s(T);
}

void PSASPE2::set_KA(double K)
{
    regulator.set_K(K);
}

void PSASPE2::set_TA_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void PSASPE2::set_Efdmax_in_pu(double emax)
{
    Efdmax = emax;
}

void PSASPE2::set_Efdmin_in_pu(double emin)
{
    Efdmin = emin;
}

void PSASPE2::set_Vta_in_pu(double V)
{
    Vta = V;
}

void PSASPE2::set_Vtb_in_pu(double V)
{
    Vtb = V;
}

void PSASPE2::set_Kpt(double K)
{
    Kpt = K;
}

void PSASPE2::set_Kit(double K)
{
    Kit = K;
}

void PSASPE2::set_Ke(double K)
{
    Ke = K;
}

double PSASPE2::get_KR() const
{
    return sensor.get_K();
}

double PSASPE2::get_TR_in_s() const
{
    return sensor.get_T_in_s();
}

bool PSASPE2::get_K2() const
{
    return K2;
}

double PSASPE2::get_T1_in_s() const
{
    return tuner1_lead_lag.get_T1_in_s();
}

double PSASPE2::get_T2_in_s() const
{
    return tuner1_lead_lag.get_T2_in_s();
}

double PSASPE2::get_T3_in_s() const
{
    return tuner2.get_T1_in_s();
}

double PSASPE2::get_T4_in_s() const
{
    return tuner2.get_T2_in_s();
}

double PSASPE2::get_KA() const
{
    return regulator.get_K();
}

double PSASPE2::get_TA_in_s() const
{
    return regulator.get_T_in_s();
}

double PSASPE2::get_Efdmax_in_pu() const
{
    return Efdmax;
}

double PSASPE2::get_Efdmin_in_pu() const
{
    return Efdmin;
}

double PSASPE2::get_Vta_in_pu() const
{
    return Vta;
}

double PSASPE2::get_Vtb_in_pu() const
{
    return Vtb;
}

double PSASPE2::get_Kpt() const
{
    return Kpt;
}

double PSASPE2::get_Kit() const
{
    return Kit;
}

double PSASPE2::get_Ke() const
{
    return Ke;
}

bool PSASPE2::setup_model_with_steps_string(string data)
{
    return false;
}

bool PSASPE2::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<19)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;


    double kr, tr, k2, t1, t2, t3, t4, ka, ta, efdmax, efdmin, vta, vtb, kpt, kit, ke;

    size_t i=3;
    kr = get_double_data(dyrdata[i],"1.0"); i++;
    tr = get_double_data(dyrdata[i],"0.0"); i++;
    k2 = get_double_data(dyrdata[i],"0.0"); i++;
    t1 = get_double_data(dyrdata[i],"0.0"); i++;
    t2 = get_double_data(dyrdata[i],"0.0"); i++;
    t3 = get_double_data(dyrdata[i],"0.0"); i++;
    t4 = get_double_data(dyrdata[i],"0.0"); i++;
    ka = get_double_data(dyrdata[i],"0.0"); i++;
    ta = get_double_data(dyrdata[i],"0.0"); i++;
    efdmax = get_double_data(dyrdata[i],"0.0"); i++;
    efdmin = get_double_data(dyrdata[i],"0.0"); i++;
    vta = get_double_data(dyrdata[i],"0.0"); i++;
    vtb = get_double_data(dyrdata[i],"0.0"); i++;
    kpt = get_double_data(dyrdata[i],"0.0"); i++;
    kit = get_double_data(dyrdata[i],"0.0"); i++;
    ke = get_double_data(dyrdata[i],"0.0"); i++;

    set_KR(kr);
    set_TR_in_s(tr);
    set_K2(k2);
    set_T1_in_s(tr);
    set_T2_in_s(tr);
    set_T3_in_s(tr);
    set_T4_in_s(tr);
    set_KA(ka);
    set_TA_in_s(ta);
    set_Efdmax_in_pu(efdmax);
    set_Efdmax_in_pu(efdmin);
    set_Vta_in_pu(vta);
    set_Vtb_in_pu(vtb);
    set_Kpt(kpt);
    set_Kit(kit);
    set_Ke(ke);

    is_successful = true;

    return is_successful;
}

bool PSASPE2::setup_model_with_bpa_string(string data)
{
    return false;
}


void PSASPE2::initialize()
{
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

    set_voltage_reference_in_pu(Ecomp);

    sensor.set_output(0.0);
    sensor.initialize();

    if(get_K2()==true)
    {
        tuner1_lead_lag.set_output(0.0);
        tuner1_lead_lag.initialize();
    }
    else
    {
        tuner1_pi.set_output(0.0);
        tuner1_pi.initialize();
    }

    tuner2.set_output(0.0);
    tuner2.initialize();

    regulator.set_output(0.0);
    regulator.initialize();

    double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();
    this->Efd0 = Efd;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    size_t bus = generator->get_generator_bus();
    this->Vt0 = psdb->get_bus_voltage_in_pu(bus);

    set_flag_model_initialized_as_true();
}

void PSASPE2::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    double Ecomp = get_compensated_voltage_in_pu();
    double Vref = get_voltage_reference_in_pu();
    double Vs = get_stabilizing_signal_in_pu();

    double input = Vref-Ecomp+Vs;
    sensor.set_input(input);
    sensor.run(mode);

    input = sensor.get_output();
    if(get_K2()==true)
    {
        tuner1_lead_lag.set_input(input);
        tuner1_lead_lag.run(mode);
        input = tuner1_lead_lag.get_output();
    }
    else
    {
        tuner1_pi.set_input(input);
        tuner1_pi.run(mode);
        input = tuner1_pi.get_output();
    }

    tuner2.set_input(input);
    tuner2.run(mode);

    input = tuner2.get_output();
    regulator.set_input(input);
    regulator.run(mode);

    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double PSASPE2::get_excitation_voltage_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return 0.0;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    size_t bus = generator->get_generator_bus();
    complex<double> Vt = psdb->get_bus_complex_voltage_in_pu(bus);
    complex<double> It = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();

    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();

    double Vta = get_Vta_in_pu(), Vtb = get_Vtb_in_pu();
    double Kv1 = 1.0/(Vt0*Vta), Kv2 = 1.0/(Vt0/Vtb);

    double Kpt = get_Kpt(), Kit = get_Kit(), Ke = get_Ke();
    complex<double> imag_1(0.0, 1.0);

    double scale = abs(Kpt*Vt + imag_1*Kit*It);
    Efdmax = Kv1*scale*Efdmax - Ke*Ifd;
    Efdmin = Kv2*scale*Efdmin - Ke*Ifd;

    double Efd = Efd0 + regulator.get_output();

    if(Efd>Efdmax) Efd = Efdmax;
    if(Efd<Efdmin) Efd = Efdmin;

    return Efd;
}
void PSASPE2::check()
{
    ;
}

void PSASPE2::report()
{
    ostringstream sstream;
    sstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(sstream);
}

void PSASPE2::save()
{
    ;
}

string PSASPE2::get_standard_model_string() const
{
    ostringstream sstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();
    double KR = get_KR();
    double TR = get_TR_in_s();
    bool K2 = get_K2();
    double T1 = get_T1_in_s();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double T4 = get_T4_in_s();
    double KA = get_KA();
    double TA = get_TA_in_s();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();
    double Vta = get_Vta_in_pu();
    double Vtb = get_Vtb_in_pu();
    double Kpt = get_Kpt();
    double Kit = get_Kit();
    double Ke = get_Ke();

    sstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<KR<<", "
      <<setw(8)<<setprecision(6)<<TR<<", "
      <<setw(8)<<setprecision(6)<<K2<<", "
      <<setw(8)<<setprecision(6)<<T1<<", "
      <<setw(8)<<setprecision(6)<<T2<<", "
      <<setw(8)<<setprecision(6)<<T3<<", "
      <<setw(8)<<setprecision(6)<<T4<<", "
      <<setw(8)<<setprecision(6)<<KA<<", "
      <<setw(8)<<setprecision(6)<<TA<<", "
      <<setw(8)<<setprecision(6)<<Efdmax<<", "
      <<setw(8)<<setprecision(6)<<Efdmin<<", "
      <<setw(8)<<setprecision(6)<<Vta<<", "
      <<setw(8)<<setprecision(6)<<Vtb<<", "
      <<setw(8)<<setprecision(6)<<Kpt<<", "
      <<setw(8)<<setprecision(6)<<Kit<<", "
      <<setw(8)<<setprecision(6)<<Ke<<"  /";
    return sstream.str();
}


size_t PSASPE2::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}

string PSASPE2::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}

double PSASPE2::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}

double PSASPE2::get_variable_with_name(string var_name)
{
    if(var_name == "EXCITATION VOLTAGE")
        return get_excitation_voltage_in_pu();

    if(var_name == "VOLTAGE REFERENCE")
        return get_voltage_reference_in_pu();

    if(var_name == "PSASPE2ENSATED VOLTAGE")
        return get_compensated_voltage_in_pu();

    if(var_name == "STABILIZING SIGNAL")
        return get_stabilizing_signal_in_pu();

    if(var_name == "STATE@SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@TUNER1")
    {
        if(get_K2()==true)
            return tuner1_lead_lag.get_state();
        else
            return tuner1_pi.get_state();
    }

    if(var_name == "STATE@TUNER2")
        return tuner2.get_state();

    if(var_name == "STATE@REGULATOR")
        return regulator.get_state();

    return 0.0;
}


string PSASPE2::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPE2::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPE2::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}