#include "gui/guioption.h"

#include <sstream>

const std::string GUIOPTION::null;

GUIOPTION::GUIOPTION() :
	current_valid(false),
	min(0),
	max(1),
	percentage(true)
{
	current_value = values.begin();
	set_val.call.bind<GUIOPTION, &GUIOPTION::SetCurrentValue>(this);
	prev_val.call.bind<GUIOPTION, &GUIOPTION::Decrement>(this);
	next_val.call.bind<GUIOPTION, &GUIOPTION::Increment>(this);
}

GUIOPTION::GUIOPTION(const GUIOPTION & other)
{
	*this = other;
	set_val.call.bind<GUIOPTION, &GUIOPTION::SetCurrentValue>(this);
	prev_val.call.bind<GUIOPTION, &GUIOPTION::Decrement>(this);
	next_val.call.bind<GUIOPTION, &GUIOPTION::Increment>(this);
}

GUIOPTION & GUIOPTION::operator=(const GUIOPTION & other)
{
	current_valid = other.current_valid;
	values = other.values;
	current_value = values.begin(); // fixme
	non_value_data = other.non_value_data;
	description = other.description;
	text = other.text;
	type = other.type;
	min = other.min;
	max = other.max;
	percentage = other.percentage;
	signal_val = other.signal_val;
	signal_str = other.signal_str;
	return *this;
}

void GUIOPTION::ReplaceValues(const std::list <std::pair<std::string, std::string> > & newvalues)
{
	values = newvalues;
	SetToFirstValue();
}

void GUIOPTION::SetInfo(const std::string & newtext, const std::string & newdesc, const std::string & newtype)
{
	text = newtext;
	description = newdesc;
	type = newtype;
}

void GUIOPTION::Insert(const std::string & stored_value, const std::string & display_value)
{
	values.push_back(std::pair<std::string, std::string>(stored_value, display_value));
}

void GUIOPTION::SetMinMaxPercentage(float newmin, float newmax, bool newpercentage)
{
	min = newmin;
	max = newmax;
	percentage = newpercentage;
}

void GUIOPTION::SetCurrentValue(const std::string & storedvaluename)
{
	if (values.empty())
	{
		non_value_data = storedvaluename;
		current_valid = false;
		signal_val(storedvaluename);
		signal_str(storedvaluename);
		return;
	}

	current_valid = false;
	if (type == "float")
	{
		float storedvaluefloat(0);
		{
			std::stringstream floatstr;
			floatstr.str(storedvaluename);
			floatstr >> storedvaluefloat;
		}
		for (std::list <std::pair<std::string, std::string> >::iterator i = values.begin(); i != values.end(); ++i)
		{
			float ifloat(0);
			std::stringstream floatstr;
			floatstr.str(i->first);
			floatstr >> ifloat;
			if (ifloat == storedvaluefloat)
			{
				current_valid = true;
				current_value = i;
				break;
			}
		}
	}
	else
	{
		for (std::list <std::pair<std::string, std::string> >::iterator i = values.begin(); i != values.end(); ++i)
		{
			if (i->first == storedvaluename)
			{
				current_valid = true;
				current_value = i;
				break;
			}
		}
	}

	// if value not found set first value
	if (!current_valid)
	{
		current_value = values.begin();
		current_valid = true;
	}

	SignalValue();
}

void GUIOPTION::Increment()
{
	if (values.empty())
	{
		current_value = values.end();
		return;
	}

	if (current_value == values.end())
		current_value = values.begin();
	else
	{
		current_value++;
		if (current_value == values.end())
			current_value = values.begin();
	}

	SignalValue();
}

void GUIOPTION::Decrement()
{
	if (values.empty())
	{
		current_value = values.end();
		return;
	}

	if (current_value == values.begin())
	{
		current_value = values.end();
	}

	current_value--;

	SignalValue();
}

void GUIOPTION::SetToFirstValue()
{
	current_value = values.begin();
	SignalValue();
}

const std::string & GUIOPTION::GetCurrentDisplayValue() const
{
	if (values.empty())
	{
		return non_value_data;
	}
	else if (current_value == values.end() || !current_valid)
	{
		return null;
	}
	else
	{
		return current_value->second;
	}
}

const std::string & GUIOPTION::GetCurrentStorageValue() const
{
	if (values.empty())
	{
		return non_value_data;
	}
	else if (current_value == values.end() || !current_valid)
	{
		return null;
	}
	else
	{
		return current_value->first;
	}
}

const std::list <std::pair<std::string,std::string> > & GUIOPTION::GetValueList() const
{
	return values;
}

void GUIOPTION::SignalValue()
{
	current_valid = !values.empty();
	if (current_valid)
	{
		signal_val(current_value->first);
		signal_str(current_value->second);
	}
}