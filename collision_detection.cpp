#include "stdafx.h"

void bottom(RECT& ball_rc, const RECT parent_rc)
{
	ball_rc.bottom = 2 * parent_rc.bottom - ball_rc.bottom;
	ball_rc.top = ball_rc.bottom - ball_diameter;
	direction.y = -1;
}

void left(RECT& ball_rc, const RECT parent_rc)
{
	ball_rc.left = 2 * parent_rc.left - ball_rc.left;
	ball_rc.right = ball_rc.left + ball_diameter;
	direction.x = 1;
}

void top(RECT& ball_rc, const RECT parent_rc)
{
	ball_rc.top = 2 * parent_rc.top - ball_rc.top;
	ball_rc.bottom = ball_rc.top + ball_diameter;
	direction.y = 1;
}

RECT collision_detection(const HWND h_parent, RECT ball_rc, const RECT paddle_rc)
{
	bool is_paddle = false;
	RECT parent_rc;
	GetClientRect(h_parent, &parent_rc);
	if (ball_rc.right > paddle_rc.left && direction.x == 1 && (direction.y == 1 && ball_rc.bottom - paddle_rc.top - gap > ball_rc.right - paddle_rc.left && ball_rc.top -
		paddle_rc.bottom + gap < ball_rc.right - paddle_rc.left
		|| direction.y == -1 && paddle_rc.bottom - ball_rc.top - gap > ball_rc.right - paddle_rc.left && paddle_rc.top -
		ball_rc.bottom + gap < ball_rc.right - paddle_rc.left))
	{
		parent_rc.right = paddle_rc.left;
		is_paddle = true;
	}

	if (ball_rc.bottom > parent_rc.bottom)
	{
		bottom(ball_rc, parent_rc);
	}
	else if (ball_rc.top < parent_rc.top)
	{
		top(ball_rc, parent_rc);
	}
	if (ball_rc.right > parent_rc.right)
	{
		ball_rc.right = 2 * parent_rc.right - ball_rc.right;
		ball_rc.left = ball_rc.right - ball_diameter;
		direction.x = -1;
		if (is_paddle)
		{
			win_count++;
		}
		else
		{
			loose_count++;
		}
		update_bitmap(h_parent);
	}
	if (ball_rc.left < parent_rc.left)
	{
		left(ball_rc, parent_rc);
	}
	return ball_rc;
}


