

#ifndef _INC_GPIO
#define _INC_GPIO

#include "peripheral.h"
#include "types.h"

typedef enum 
{
	fnInput 	= 0x00,
	fnOutput	= 0x01,
	fnAlt0 		= 0x04,
	fnAlt1 		= 0x05,
	fnAlt2 		= 0x06,
	fnAlt3 		= 0x07,
	fnAlt4 		= 0x03,
	fnAlt5 		= 0x02
}
PinFunction;

typedef enum 
{
	updnOff 	= 0x00,
	updnDown	= 0x01,
	updnUp 		= 0x02
}
PullUpDn;

typedef enum
{
	evSyncRisingEdge,
	evSyncFallingEdge,
	evSyncBoth,
	evAsyncRisingEdge,
	evAsyncFallingEdge,
	evAsyncBoth,
	evLevelHigh,
	evLevelLow
}
PinEvent;

class GPIO : public GPIOMemory_Peripheral
{
	private:
		PullUpDn			pullUpDownState[54];
			
		int					_getTwinRegisterNum(int pinNum);
		uint32_t			_getTwinRegisterMask(int pinNum);
		
	public:
		GPIO(int memFd, uint32_t baseAddress);
		
		PinFunction			getPinFunction(int pinNum);
		void				setPinFunction(int pinNum, PinFunction function);
		
		PullUpDn			getPinPullUpDown(int pinNum);
		void				setPinPullUpDown(int pinNum, PullUpDn pullUpDown);
		
		byte				getPinLevel(int pinNum);
		
		byte				getPinEventStatus(int pinNum);
		void				clearPinEventStatus(int pinNum);
		
		void				setPinEventDetect(int pinNum, PinEvent event);
		void				clearPinEventDetect(int pinNum, PinEvent event);
		
		void				setPin(int pinNum);
		void				clrPin(int pinNum);
		
		uint32_t			getGPFSELN(int n);
		void				setGPFSELN(int n, uint32_t val);
			
		void				setGPSETN(int n, uint32_t val);
			
		void				setGPCLRN(int n, uint32_t val);
			
		uint32_t			getGPLEVN(int n);
			
		uint32_t			getGPEDSN(int n);
		void				setGPEDSN(int n, uint32_t val);
			
		uint32_t			getGPRENN(int n);
		void				setGPRENN(int n, uint32_t val);
				
		uint32_t			getGPFENN(int n);
		void				setGPFENN(int n, uint32_t val);
				
		uint32_t			getGPHENN(int n);
		void				setGPHENN(int n, uint32_t val);
	
		uint32_t			getGPLENN(int n);
		void				setGPLENN(int n, uint32_t val);
	
		uint32_t			getGPARENN(int n);
		void				setGPARENN(int n, uint32_t val);
				
		uint32_t			getGPAFENN(int n);
		void				setGPAFENN(int n, uint32_t val);
				
		uint32_t			getGPPUD();
		void				setGPPUD(uint32_t val);
	
		uint32_t			getGPPUDCLKN(int n);
		void				setGPPUDCLKN(int n, uint32_t val);
};

#endif