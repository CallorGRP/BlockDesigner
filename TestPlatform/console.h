//-------------------------------------------------------------
// Design	: AHB Lite Console
// Author	: Harold Ko
// Email	: harold.ko@twoblocktech.com
// File		: console.h
// Date		: 2016. 1. 6
//-------------------------------------------------------------
// Copyright (C) 2015 TwoBlock Technologies Co.
//-------------------------------------------------------------
// Description	: Console for Test System.v.1.0.1
//-------------------------------------------------------------

#ifndef	__CONSOLE_H__
#define	__CONSOLE_H__

#define	ENDIAN_BIG	0

#include "systemc.h"
#include "type.h"
#include <stdio.h>

SC_MODULE(CONSOLE)	{
	/********** [port] **********/
	sc_in<bool>		HCLK;
	sc_in<bool>		HRESETn;
	
	sc_in<bool>		HSEL;
	sc_in<bool>		HREADY;
	sc_in<UINT32>		HADDR;
	sc_in<UINT32>		HBURST;
	sc_in<UINT32>		HPROT;
	sc_in<UINT32>		HTRANS;
	sc_in<bool>		HWRITE;
	sc_in<UINT32>		HSIZE;
	sc_in<UINT32>		HWDATA;

	sc_out<bool>		HREADYOUT;	// Always High.
	sc_out<UINT32>		HRDATA;
	sc_out<bool>		HRESP;		// Always Low.

	/********** [local variable] **********/
	// Registers to Store Address Phase Signals
	sc_signal<bool>		REG_ADDR_PHASE_HSEL;
	sc_signal<bool>		REG_ADDR_PHASE_HWRITE;
	sc_signal<UINT32>	REG_ADDR_PHASE_HTRANS;
	sc_signal<UINT32>	REG_ADDR_PHASE_HADDR;
	sc_signal<UINT32>	REG_ADDR_PHASE_HSIZE;

	sc_signal<bool>		NEXT_ADDR_PHASE_HSEL;
	sc_signal<bool>		NEXT_ADDR_PHASE_HWRITE;
	sc_signal<UINT32>	NEXT_ADDR_PHASE_HTRANS;
	sc_signal<UINT32>	NEXT_ADDR_PHASE_HADDR;
	sc_signal<UINT32>	NEXT_ADDR_PHASE_HSIZE;

	sc_signal<UINT32>	dw_WE;

	sc_signal<UINT8>	REG_DATA;
	sc_signal<bool>		REG_FLAG;

	sc_signal<UINT8>	NEXT_DATA;
	sc_signal<bool>		NEXT_FLAG;

	/********** [member function] **********/
	UINT32 ByteEnable(UINT32 addr, UINT32 size)	{
		UINT32 dw_AddrTmp;
		UINT32 dw_SizeTmp;
		UINT32 dw_EnableState;

		dw_AddrTmp = addr & 0x3;		// address offset.
		dw_SizeTmp = (size & 0x7) << 2;		// transfer size.
		dw_EnableState = dw_SizeTmp | dw_AddrTmp;

		switch(dw_EnableState)	{
#if	ENDIAN_BIG
			case 0x8 :	return 0xF;	// word.	// case 5b'010_00 -> 4b'1111
			case 0x4 :	return 0xC;	// halfword.	// case 5b'001_00 -> 4b'1100
			case 0x6 :	return 0x3;	// halfword.	// case 5b'001_10 -> 4b'0011
			case 0x0 :	return 0x8;	// byte.	// case 5b'000_00 -> 4b'1000
			case 0x1 :	return 0x4;	// byte.	// case 5b'000_01 -> 4b'0100
			case 0x2 :	return 0x2;	// byte.	// case 5b'000_10 -> 4b'0010
			case 0x3 :	return 0x1;	// byte.	// case 5b'000_11 -> 4b'0001
#else	// little-endian (default)
			case 0x8 :	return 0xF;	// word.	// case 5b'010_00 -> 4b'1111
			case 0x4 :	return 0x3;	// halfword.	// case 5b'001_00 -> 4b'0011
			case 0x6 :	return 0xC;	// halfword.	// case 5b'001_10 -> 4b'1100
			case 0x0 :	return 0x1;	// byte.	// case 5b'000_00 -> 4b'0001
			case 0x1 :	return 0x2;	// byte.	// case 5b'000_01 -> 4b'0010
			case 0x2 :	return 0x4;	// byte.	// case 5b'000_10 -> 4b'0100
			case 0x3 :	return 0x8;	// byte.	// case 5b'000_11 -> 4b'1000
#endif
			default :	return 0x0;
		}
	}

	/********** [process function] **********/
	void do_assign_addr_phase()	{
		if(HREADY && HSEL)	{
			NEXT_ADDR_PHASE_HSEL = HSEL;
			NEXT_ADDR_PHASE_HWRITE = HWRITE;
			NEXT_ADDR_PHASE_HTRANS = HTRANS;
			NEXT_ADDR_PHASE_HADDR = HADDR;
			NEXT_ADDR_PHASE_HSIZE = HSIZE;
		}
		else	{
			NEXT_ADDR_PHASE_HSEL = 0;
			NEXT_ADDR_PHASE_HWRITE = 0;
			NEXT_ADDR_PHASE_HTRANS = 0x0;
			NEXT_ADDR_PHASE_HADDR = 0x0;
			NEXT_ADDR_PHASE_HSIZE = 0x0;
		}
	}
	
	void do_pos_hclk_neg_hresetn()	{
		while(1)	{
			if(!HRESETn)	{
				REG_ADDR_PHASE_HSEL = 0;
				REG_ADDR_PHASE_HWRITE = 0;
				REG_ADDR_PHASE_HTRANS = 0x0;
				REG_ADDR_PHASE_HADDR = 0x0;
				REG_ADDR_PHASE_HSIZE = 0x0;
				
				HREADYOUT = 1;
				HRESP = 0;
				HRDATA = 0x0;
				
				REG_FLAG = 0;
				REG_DATA = 0x0;
			}
			else	{
				REG_ADDR_PHASE_HSEL = NEXT_ADDR_PHASE_HSEL;
				REG_ADDR_PHASE_HWRITE = NEXT_ADDR_PHASE_HWRITE;
				REG_ADDR_PHASE_HTRANS = NEXT_ADDR_PHASE_HTRANS;
				REG_ADDR_PHASE_HADDR = NEXT_ADDR_PHASE_HADDR;
				REG_ADDR_PHASE_HSIZE = NEXT_ADDR_PHASE_HSIZE;

				HREADYOUT = 1;
				HRESP = 0;
				HRDATA = 0x0;

				REG_FLAG = NEXT_FLAG;
				REG_DATA = NEXT_DATA;
			}
			wait();
		}
	}

	void do_assign_we()	{
		dw_WE = ByteEnable((UINT32)REG_ADDR_PHASE_HADDR, (UINT32)REG_ADDR_PHASE_HSIZE);
	}

	void do_transfer_console_data()	{
		if(REG_ADDR_PHASE_HSEL && REG_ADDR_PHASE_HWRITE && (REG_ADDR_PHASE_HTRANS & 0x2))	{
			if(dw_WE & 0x1)	{
				NEXT_FLAG = 1;
				NEXT_DATA = (UINT8)(HWDATA & 0xFF);
			}
			else	{
				NEXT_FLAG = 0;
				NEXT_DATA = 0x0;
			}
		}
		else	{
			NEXT_FLAG = 0;
			NEXT_DATA = 0x0;
		}
	}

	void do_print_console()	{
		if(REG_FLAG)	{
			if(REG_DATA == 0x0D)	{
				printf("Simulation Stop by CM0DS!\n");
				sc_stop();
			}
			else	printf("\033[33m%c\033[0m", (UINT8)REG_DATA);
		}
	}

	SC_CTOR(CONSOLE)	{
		SC_METHOD(do_assign_addr_phase);
		sensitive << HREADY;
		sensitive << HSEL;
		sensitive << HWRITE;
		sensitive << HTRANS;
		sensitive << HADDR;
		sensitive << HSIZE;
		sensitive << HWDATA;

		SC_THREAD(do_pos_hclk_neg_hresetn);
		sensitive << HCLK.pos();
		sensitive << HRESETn.neg();

		SC_METHOD(do_assign_we);
		sensitive << REG_ADDR_PHASE_HADDR;
		sensitive << REG_ADDR_PHASE_HSIZE;
		
		SC_METHOD(do_transfer_console_data);
		sensitive << REG_ADDR_PHASE_HSEL;
		sensitive << REG_ADDR_PHASE_HWRITE;
		sensitive << REG_ADDR_PHASE_HTRANS;
		sensitive << dw_WE;
		sensitive << HWDATA;

		SC_METHOD(do_print_console);
		sensitive << REG_FLAG;
		sensitive << REG_DATA;
	}
};

#endif	// __CONSOLE_H__
