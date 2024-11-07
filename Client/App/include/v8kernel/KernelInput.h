#pragma once

// NOTE: all code for this class was in the header

namespace RBX
{
	class KernelInput
	{
	private:
		float lastGoal;
		float currentGoal;
		float increment;
		bool latchToZero;
	private:
		const int steps();
	public:
		KernelInput() : lastGoal(0), currentGoal(0), latchToZero(false), increment(0) {}

		void setGoal(float goal)
		{
			if (this->latchToZero)
			{
				this->latchToZero = false;
				this->currentGoal = (this->currentGoal + goal) * 0.5f;
			}

			this->currentGoal += this->increment;
		}

		void setDelta(float);

		void get(float _lastGoal, float& _currentGoal, float& _increment)
		{
			// is the first parameter not being a reference intentional? was it used for something else?
			_lastGoal = this->lastGoal;
			_currentGoal = this->currentGoal;
			_increment = this->increment;
		}
	};
}