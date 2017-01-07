
#ifndef _INC_PERIPHERAL
#define _INC_PERIPHERAL

#define PAGE_SIZE		4096
#define BLOCK_SIZE		4096

class BCM238X_Peripheral
{
	private:
		volatile uint32_t	*	map;

	protected:
		BCM238X_Peripheral(int fd, uint32_t baseAddress);
		~BCM238X_Peripheral();
		
		virtual void			checkPrivileges() = 0;
		volatile uint32_t *		getMemoryMap();
};

class GPIOMemory_Peripheral : public BCM238X_Peripheral
{
	protected:
		GPIOMemory_Peripheral(int fd, uint32_t baseAddress);
		virtual ~GPIOMemory_Peripheral() {}
		
		virtual void			checkPrivileges();
};

class ProtectedMemory_Peripheral : public BCM238X_Peripheral
{
	protected:
		ProtectedMemory_Peripheral(int fd, uint32_t baseAddress);
		virtual ~ProtectedMemory_Peripheral() {}
		
		virtual void			checkPrivileges();
};

#endif
