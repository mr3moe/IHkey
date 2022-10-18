#ifndef __IHKEY_ANTIVM__
#define __IHKEY_ANTIVM__

#include "include.h"




namespace AntiVM 
{
	// checking for System resources
	void CheckForCPUCores();
	void CheckForRamSize();
	void CheckForHHDSize();
	void UsernamesChecker();
	void ComputerNameChecker();

	// check for Directories and files and registry
	void DirectoryChecker();
	void registryChecker();
	void CurrentProcChecker();
	VOID vbox_reg_key_value();
	VOID vbox_reg_keys();
	

	//checking for Running Proccess and Services and Moduls
	void CheckForRunningModule();
	void CheckForBlackListPorc();
	void VirtualDevices();
	
	//others
	VOID ScreenResolution();


}
#endif //!__IHKEY_ANTIVM__





