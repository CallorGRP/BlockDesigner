#include "console.h"

BDDI* CONSOLE::GetBDDI()
{
	return bddi;
}

char* CONSOLE::GetModuleName()
{
	return (char*)"CONSOLE";
}

void CONSOLE::BDInit()
{
	HCLK.set_port_name("HCLK");
	HRESETn.set_port_name("HRESETn");

	AHB_SS = new BD_AHBPort_SS((char*)"SS_S0");

	/////// for test of BDDI ///////
	hw_reg = 0;
	w_reg = 0;
	dw_reg = 0;
	lw_reg = 0;
	b_reg = false;
	h_reg = 0;
	f_reg = 0;
	df_reg = 0;
	memset(a_reg, 0, sizeof(a_reg));

	hw_par = 0;
	w_par = 0;
	dw_par = 0;
	lw_par = 0;
	b_par = false;
	dw_paru = 0;
	f_par = 0;
	df_par = 0;
	memset(a_par, 0, sizeof(a_par));
	////////////////////////////////

	// for BDDI
	bddi = new CONSOLE_BDDI(this);
}

extern "C" sc_module* CreateInstance(const char *ModuleInstanceName)
{
	return new CONSOLE(ModuleInstanceName);
}
