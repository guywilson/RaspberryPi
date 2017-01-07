
#ifndef _INC_SWTIMER
#define _INC_SWTIMER

#define NUM_TIMERS				5

class SWTimer
{
	private:
		static void				_shortDelay(unsigned long delay);
		
	public:
		static void				usDelay(unsigned long delay);
		static void				msDelay(unsigned long delay);
		static void				ssDelay(unsigned long delay);
		
		static int				start();
		static unsigned long	end(int timerId);
};

#endif