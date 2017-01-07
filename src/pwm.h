
#ifndef _INC_PWM
#define _INC_PWM

#include "peripheral.h"

/*
** PWM Channel Identifiers
*/
#define PWM_CHANNEL_0				0
#define PWM_CHANNEL_1				1

/*
** PWM Control register flags...
*/
#define PWM_CTRL_ENABLE				0x00000001
#define PWM_CTRL_DISABLE			0x00000000

#define PWM_CTRL_MODE_PWM			0x00000000
#define PWM_CTRL_MODE_SER			0x00000002

#define PWM_CTRL_REPEAT_INT			0x00000000
#define PWM_CTRL_REPEAT_RPT			0x00000004

#define PWM_CTRL_SILENCE_OFF		0x00000000
#define PWM_CTRL_SILENCE_ON			0x00000008

#define PWM_CTRL_POLARITY_HIGH		0x00000000
#define PWM_CTRL_POLARITY_LOW		0x00000010

#define PWM_CTRL_FIFO_DISABLE		0x00000000
#define PWM_CTRL_FIFO_ENABLE		0x00000020

#define PWM_CTRL_FIFO_CLEAR			0x00000040

#define PWM_CTRL_ALGORITHM_PWM		0x00000000
#define PWM_CTRL_ALGORITHM_MS		0x00000080

/*
** PWM Status flags...
*/
#define PWM_STATUS_FIFO_FULL		0x00000001
#define PWM_STATUS_FIFO_EMPTY		0x00000002

#define PWM_ERROR_FIFO_WRITE		0x00000004
#define PWM_ERROR_FIFO_READ			0x00000008
#define PWM_ERROR_CH0_GAP			0x00000010
#define PWM_ERROR_CH1_GAP			0x00000020
#define PWM_ERROR_BUS				0x00000100

#define PWM_STATUS_CH0_TX			0x00000200
#define PWM_STATUS_CH1_TX			0x00000200


class PWM : public ProtectedMemory_Peripheral
{
	private:
		CLOCK	*		pClock;
	
	protected:
		CLOCK *			getClock();
		
		void			setControl(int channel, uint32_t flags);
		
	public:
		PWM(int memFd, uint32_t baseAddress);
		
		void			setClock(CLOCK * pClock);
		
		void			disable();
		void			disable(int channel);
		
		void			enable(int channel);
		
		void			setup(int channel, uint32_t flags, uint32_t range);
		void			setup(int channel, uint32_t flags, unsigned long clkFrequency, uint32_t range);
		
		uint32_t		getStatus(int channel);
		
		bool			isFifoFull();
		bool			isFifoEmpty();
		bool			isError(int channel);
		bool			isActive(int channel);
		
		void			setRange(int channel, uint32_t range);
		void			setData(int channel, uint32_t data);
		
		uint32_t		getPWM_CTRL();
		void			setPWM_CTRL(uint32_t value);
		
		uint32_t		getPWM_STATUS();
		void			setPWM_STATUS(uint32_t value);

		uint32_t		getPWM_DMAC();
		void			setPWM_DMAC(uint32_t value);
		
		uint32_t		getPWM_RANGE0();
		void			setPWM_RANGE0(uint32_t value);

		uint32_t		getPWM_DATA0();
		void			setPWM_DATA0(uint32_t);
		
		uint32_t		getPWM_FIFO();
		void			setPWM_FIFO(uint32_t value);
		
		uint32_t		getPWM_RANGE1();
		void			setPWM_RANGE1(uint32_t value);
		
		uint32_t		getPWM_DATA1();
		void			setPWM_DATA1(uint32_t value);
};

#endif
