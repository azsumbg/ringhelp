#include "pch.h"
#include "ringhelp.h"

// EXCEPTION class **************************

dll::EXCEPTION::EXCEPTION(int what) :_what{ what } {};

const wchar_t* dll::EXCEPTION::eWhat()const
{
	switch (_what)
	{
	case EX_INDEX:
		return L"Bad index passed to a class method !";

	case EX_PARAM:
		return L"Bad parameter passed to a class method !";

	case EX_PTR:
		return L"Bad pointer passed to a class method !";

	case EX_INIT:
		return L"Error creating BAG container !";
	}

	return L"Unknown error occurred in a class method !";
}

/////////////////////////////////////////////

// RANDIT class *****************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}

dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	if (max < min)return 0;

	std::uniform_int_distribution distrib(min, max);

	return distrib(*twister);
}
float dll::RANDIT::operator()(float min, float max)
{
	if (max < min)return 0.0f;

	std::uniform_real_distribution<float> distrib(min, max);

	return distrib(*twister);
}

////////////////////////////////////////////

// PROTON class ***************************

dll::PROTON::PROTON()
{
	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	unit_rect.left = start.x;
	unit_rect.right = end.x;
	unit_rect.top = start.y;
	unit_rect.bottom = end.y;
}
dll::PROTON::PROTON(float _sx, float _sy)
{
	start.x = _sx;
	start.y = _sy;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	unit_rect.left = start.x;
	unit_rect.right = end.x;
	unit_rect.top = start.y;
	unit_rect.bottom = end.y;
}
dll::PROTON::PROTON(float _sx, float _sy, float _first_width, float _first_height)
{
	start.x = _sx;
	start.y = _sy;

	_width = _first_width;
	_height = _first_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	unit_rect.left = start.x;
	unit_rect.right = end.x;
	unit_rect.top = start.y;
	unit_rect.bottom = end.y;
}

void dll::PROTON::set_edges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	unit_rect.left = start.x;
	unit_rect.right = end.x;
	unit_rect.top = start.y;
	unit_rect.bottom = end.y;
}
void dll::PROTON::new_dims(float _new_width, float _new_height)
{
	_width = _new_width;
	_height = _new_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	unit_rect.left = start.x;
	unit_rect.right = end.x;
	unit_rect.top = start.y;
	unit_rect.bottom = end.y;
}

void dll::PROTON::new_width(float _new_width)
{
	_width = _new_width;
	
	x_rad = _width / 2.0f;
	
	end.x = start.x + _width;
	
	center.x = start.x + x_rad;

	unit_rect.left = start.x;
	unit_rect.right = end.x;
	unit_rect.top = start.y;
	unit_rect.bottom = end.y;
}
void dll::PROTON::new_height(float _new_height)
{
	_height = _new_height;

	y_rad = _height / 2.0f;

	end.y = start.y + _height;

	center.y = start.y + y_rad;

	unit_rect.left = start.x;
	unit_rect.right = end.x;
	unit_rect.top = start.y;
	unit_rect.bottom = end.y;
}

void dll::PROTON::set_path(float _ex, float _ey)
{
	hor_dir = false;
	ver_dir = false;

	move_sx = start.x;
	move_sy = start.y;

	move_ex = _ex;
	move_ey = _ey;

	if (move_ex == move_sx || (move_ex > move_sx && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_ey == move_sy || (move_ey > move_sy && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - start.x * slope;
}

float dll::PROTON::get_width()const
{
	return _width;
}
float dll::PROTON::get_height()const
{
	return _height;
}

float dll::PROTON::get_target_x()const
{
	return move_ex;
}
float dll::PROTON::get_target_y()const
{
	return move_ey;
}

D2D1_RECT_F dll::PROTON::get_rect()const
{
	return unit_rect;
}

////////////////////////////////////////////

// FIELD class *****************************

dll::FIELD::FIELD(fields _what, float s_x, float s_y) :PROTON(s_x, s_y)
{
	type = _what;

	switch (type)
	{
	case fields::flat_ground:
		new_dims(100.0f, 100.0f);
		break;

	case fields::left_slope:
		new_dims(100.0f, 100.0f);
		break;

	case fields::right_slope:
		new_dims(100.0f, 100.0f);
		break;

	case fields::background:
		new_dims(1000.0f, 750.0f);
		max_frames = 23;
		break;

	case fields::intro:
		new_dims(1000.0f, 800.0f);
		max_frames = 92;
		break;

	case fields::portal:
		new_dims(100.0f, 100.0f);
		max_frames = 23;
		break;
	}

	if (type == fields::portal)frame_delay = 3;
	else frame_delay = 1;
}

bool dll::FIELD::move(float gear, dirs to_where)
{
	float my_speed = _speed + gear / 5.0f;

	switch (to_where)
	{
	case dirs::left:
		start.x -= my_speed;
		set_edges();
		if (end.x >= -scr_width)return true;

	case dirs::right:
		start.x += my_speed;
		set_edges();
		if (start.x <= 2.0f * scr_width)return true;
	}

	return false;
}

int dll::FIELD::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		if (type == fields::portal)frame_delay = 3;
		else frame_delay = 1;
		++frame;
		if (frame > max_frames)frame = 0;
	}
	
	return frame;
}

void dll::FIELD::Release()
{
	delete this;
}

dll::FIELD* dll::FIELD::create(fields what, float sx, float sy)
{
	FIELD* ret = new FIELD(what, sx, sy);
	
	return ret;
}

///////////////////////////////////////////

// HERO class ****************************

dll::HERO::HERO(float s_x, float s_y) :PROTON(s_x, s_y, 24.0f, 35.0f){}

void dll::HERO::climb(float ground_speed, D2D1_RECT_F hill, fields hill_type)
{
	if (action != actions::climb_up && action != actions::climb_down)
	{
		if (dir == dirs::right)
		{
			if (hill_type == fields::left_slope)
			{
				action = actions::climb_up;
				set_path(hill.right, hill.top);
			}
			else if(hill_type == fields::right_slope)
			{
				action = actions::climb_down;
				set_path(hill.right, hill.bottom);
			}
		}
		else if (dir == dirs::left)
		{
			if (hill_type == fields::left_slope)
			{
				action = actions::climb_down;
				set_path(hill.left, hill.bottom);
			}
			else if (hill_type == fields::right_slope)
			{
				action = actions::climb_up;
				set_path(hill.left, hill.top);
			}
		}
	}

	if (action == actions::climb_up)
	{
		if (dir == dirs::right)
		{
			float x_modifier = start.x + ground_speed;
			start.y = x_modifier * slope + intercept;
			set_edges();
			if (end.y <= hill.top)
			{
				end.y = hill.top;
				start.y = end.y - _height;
				set_edges();
				action = actions::stop;
			}
		}
		else if (dir == dirs::left)
		{
			float x_modifier = start.x - ground_speed;
			start.y = x_modifier * slope + intercept;
			set_edges();
			if (end.y <= hill.top)
			{
				end.y = hill.top;
				start.y = end.y - _height;
				set_edges();
				action = actions::stop;
			}
		}
	}
	else if (action == actions::climb_down)
	{
		if (dir == dirs::right)
		{
			float x_modifier = start.x + ground_speed;
			start.y = x_modifier * slope + intercept;
			set_edges();
			if (end.y >= hill.bottom)
			{
				end.y = hill.bottom;
				start.y = end.y - _height;
				set_edges();
				action = actions::stop;
			}
		}
		else if (dir == dirs::left)
		{
			float x_modifier = start.x - ground_speed;
			start.y = x_modifier * slope + intercept;
			set_edges();
			if (end.y >= hill.bottom)
			{
				end.y = hill.bottom;
				start.y = end.y - _height;
				set_edges();
				action = actions::stop;
			}
		}
	}
}

void dll::HERO::jump(BAG<D2D1_RECT_F>& grounds)
{
	if (!in_jump)
	{
		in_jump = true;
		jump_up = true;

		set_path(start.x, start.y - max_jump);
	}
	else
	{
		if (jump_up)
		{
			start.y -= _speed;
			set_edges();
			if (end.y <= move_ey)
			{
				jump_up = false;
				set_path(start.x, move_sy);
			}
		}
		else
		{
			start.y += _speed;
			set_edges();
			if (!grounds.empty())
			{
				sort(grounds, unit_rect);
				if (intersect(unit_rect, grounds[0]))
				{
					in_jump = false;
					end.y = move_ey;
					start.y = end.y - _height;
					set_edges();
					action = actions::stop;
				}
			}
			else if (end.y >= move_ey)
			{
				in_jump = false;
				end.y = move_ey;
				start.y = end.y - _height;
				set_edges();
				action = actions::stop;
			}
		}
	}
}

int dll::HERO::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}

	return frame;
}

void dll::HERO::Release()
{
	delete this;
}

dll::HERO* dll::HERO::create(float sx, float sy)
{
	HERO* ret = new HERO(sx, sy);

	return ret;
}

//////////////////////////////////////////













// FUNCTIONS **********************************************

float dll::distance(D2D1_POINT_2F first, D2D1_POINT_2F second)
{
	float a = (float)(pow(abs(second.x - first.x), 2));
	float b = (float)(pow(abs(second.y - first.y), 2));

	return (float)(sqrt(a + b));
}

bool dll::intersect(D2D1_RECT_F first, D2D1_RECT_F second)
{
	if (!(first.left >= second.right || first.right <= second.left ||
		first.top >= second.bottom || first.bottom <= second.top))return true;

	return false;
}
bool dll::intersect(D2D1_POINT_2F first_center, D2D1_POINT_2F second_center, float first_xrad, float second_xrad,
	float first_yrad, float second_yrad)
{
	if (abs(second_center.x - first_center.x) <= first_xrad + second_xrad
		&& abs(second_center.y - first_center.y) <= first_yrad + second_yrad)return true;

	return false;
}

void dll::sort(BAG<D2D1_POINT_2F>& bag, D2D1_POINT_2F ref)
{
	if (bag.size() < 2)return;
	else
	{
		bool ok = false;
		
		while (!ok)
		{
			ok = true;

			for (size_t count = 0; count < bag.size() - 1; ++count)
			{
				if (distance(bag[count], ref) > distance(bag[count + 1], ref))
				{
					ok = false;

					D2D1_POINT_2F temp = bag[count];
					bag[count] = bag[count + 1];
					bag[count + 1] = temp;
				}
			}
		}
	}
}
void dll::sort(BAG<D2D1_RECT_F>& bag, D2D1_RECT_F ref)
{
	if (bag.size() < 2)return;
	else
	{
		bool ok{ false };

		while (!ok)
		{
			ok = true;

			for (size_t count = 0; count < bag.size() - 1; ++count)
			{
				D2D1_POINT_2F first_center{ bag[count].left + (bag[count].right - bag[count].left) / 2.0f,
				bag[count].top + (bag[count].bottom - bag[count].top) / 2.0f };

				D2D1_POINT_2F second_center{ bag[count + 1].left + (bag[count + 1].right - bag[count + 1].left) / 2.0f,
				bag[count + 1].top + (bag[count + 1].bottom - bag[count + 1].top) / 2.0f };

				D2D1_POINT_2F ref_center{ ref.left + (ref.right - ref.left) / 2.0f, ref.top + (ref.bottom - ref.top) / 2.0f };

				if (distance(first_center, ref_center) > distance(second_center, ref_center))
				{
					D2D1_RECT_F temp = bag[count];
					bag[count] = bag[count + 1];
					bag[count + 1] = temp;
					ok = false;
				}
			}
		}
	}
}