#ifndef _INPUT_H_
#define _INPUT_H_

namespace pi
{
	class Input
	{
	public:
		enum Pin : int
		{
			Pin0,
			Pin1,
			Pin2,
			Pin3,
			Pin4,
			Pin5,
			Pin6,
			Pin7,
			MAX_PIN
		};

	public:
		Input();
		~Input();

		void setPin(int pin, int mode, int pud);

		void beginStep();
		void endStep();
		void capture();

		bool getPin(int index) const;
		bool getPinOn(int index) const;
		bool getPinOff(int index) const;

		inline bool getAny()	const { return m_any; }
		inline int	getState()	const { return m_state; }

	private:
		bool	m_new[Pin::MAX_PIN];
		bool	m_old[Pin::MAX_PIN];
		bool	m_any;
		int		m_state;
	};
}

#endif // !_INPUT_H_
