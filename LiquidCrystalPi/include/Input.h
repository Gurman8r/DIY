#ifndef _INPUT_H_
#define _INPUT_H_

namespace pi
{
	class Input
	{
	public:
		enum Button : int
		{
			Pin0,
			Pin1,
			Pin2,
			Pin3,
			Pin4,
			Pin5,
			Pin6,
			Pin7,
			MAX_BUTTON
		};

	public:
		Input();
		~Input();

		//void setAxis(const std::string& name, int pin, int mode, int pud);
		void setPin(int pin, int mode, int pud = 0);

		void beginStep();
		void endStep();

		bool getButton(int index) const;
		bool getButtonDown(int index) const;
		bool getButtonUp(int index) const;

		inline bool getAny() const
		{
			return m_any;
		}

	private:
		bool m_new[Button::MAX_BUTTON];
		bool m_old[Button::MAX_BUTTON];
		bool m_any;
	};
}

#endif // !_INPUT_H_
