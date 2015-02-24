/**
 * A data object that is 'ready' when value is -1, then switches to 0 to be
 * activated. When active you it will advance until a 'max' value (in seconds)
 * and then will reset to 'ready'.
 * This is for tatatatata and tululululu stuff.
 */
// TODO make this class meaningful and not gibberish
#ifndef COOLDOWN_H
#define COOLDOWN_H

namespace bk
{

struct CoolDown
{
	CoolDown() : current(-1) {}

	void reset()
	{
		current = -1;
	}

	void lock()
	{
		current = 0;
	}

	void set_maximum(float f)
	{
		maximum = f;
	}

	/**
	 * @return bool true if ready
	 */
	bool update(float f)
	{
		if (current == -1) return true;
		if (current >= 0)
		{
			current += f;
			if (current >= maximum)
			{
				current = -1;
				return true;
			}
		}
		return false;
	}
	bool is_ready() const { return current == -1; }

private:
	float current;
	float maximum;

};

}

#endif
