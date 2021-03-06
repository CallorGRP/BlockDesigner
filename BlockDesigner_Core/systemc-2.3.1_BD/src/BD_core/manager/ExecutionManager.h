//-----------------------------------------------------------------------------
// Design								: Block Designer Execution Manager 
// Author								: Bryan Choi 
// Email								: bryan.choi@twoblocktech.com 
// File		     					: ExecutionManager.h
// Date	       					: 2015/12/29
// Reference            :
// ----------------------------------------------------------------------------
// Copyright (c) 2015-2016 TwoBlockTechinologies Co.
// ----------------------------------------------------------------------------
// Description	: This class provide Execution contol API
// ----------------------------------------------------------------------------

#ifndef EXECUTIONMANAGER_H 
#define EXECUTIONMANAGER_H 

#define NOTHING 0
#define RUN   	1
#define STEP  	2
#define STOP  	3
#define CLOSE   4

#include "TopManagerBase.h"
#include <pthread.h>

/*
 * namespace	: BDapi 
 * design	    : Block Designer API 
 * description	: support analyzing ESL platform based on systemc
 */
namespace BDapi
{
	/*
	 * class		    : ExecutionManager 
	 * design	      : Control Execution of simulation 
	 * description	: Give Execution Control flag to Simulation thread, Simulation Handler thread 
	 */
	class ExecutionManager : public TopManagerBase
	{
		public:
			void PutOperationControl(GUI_COMMAND Command);
			void GetOperationControl(GUI_COMMAND Command);

			static void SetExecutionFlag(unsigned int Flag);
			static unsigned int GetExecutionFlag();	

			static void SetStepValue(unsigned int Value);
			static unsigned int GetStepValue();

			static ExecutionManager* GetInstance();
			static void DeleteInstance();

		protected:
			ExecutionManager();
			virtual ~ExecutionManager();

		private:
			static unsigned int dw_ExecutionControlFlag;
			static unsigned int dw_StepValue;
			
			static ExecutionManager *_ExecutionManager;
			// mutex for singleton pattern 
			static pthread_mutex_t ExecutionManagerInstanceMutex;   
	};
} // namespace BDapi 

#endif 


