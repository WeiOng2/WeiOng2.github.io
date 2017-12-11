//Accessing libraries and header file
#pragma once
#include <string>
#include <Engine/OGLGame.h>
#include "Vector.h"

//Construct game font
struct GameFont;

/*
	Pong. An OpenGL Game based on ASGE.
*/

class Pong:
	public ASGE::OGLGame
{
public:
	Pong();
	~Pong();
	virtual bool init() override;

private:
	void keyHandler(ASGE::SharedEventData data);
	bool newstart = true;

	// Inherited via OGLGame
	virtual void update(const ASGE::GameTime &) override;
	virtual void render(const ASGE::GameTime &) override;

	int  callback_id = -1;             // Input Callback ID. The callback ID assigned by the game engine.
	int  key_callback_id = -1;	       // Key Input Callback ID. 
	//**int  mouse_callback_id = -1;   // Mouse Input Callback ID. 	
	bool exit = false;                 // Exit boolean. If true the game loop will exit.

	//Declaring variables for game mode and game states
	bool in_menu = true, single_player = false, multiplayer = false;
	bool player_lost = false, player_won = false;
	bool escape_pressed = false;
	//Variables to store current ball speed when game is being paused
	float temp_ball_speed_x, temp_ball_speed_y;

	int menu_option = 0;							//Menu option
	int player1_score = 0, player2_score = 0;		//AI and player scores

	double ball_h = 24, ball_w = 24, bat_h = 136, bat_w = 20, buff_h = 45, buff_w = 45;		//Assigning sprite's height and width

	double initial_bat_speed = 10.0f;		//Assigning initial bat movement speed
	float ai_bat_speed;						//AI's bat movement speed

	float buff_speed = 0;					//The current buffed speed of bat
	float right_bat_buff_speed = 0;			//The amount of speed buffing the rght bat
	float left_bat_buff_speed = 0;			//The amount of speed buffing the left bat

	float speed_x = 6;						//Initial x ball speed
	float speed_y = 6;						//Initial y ball speed
	const float pi = 3.141592654;			//Constant value of Pi

	float ball_speed_x = 0.0f;				//Movement of ball x
	float ball_speed_y = 0.0f;				//Movement of ball y

	//calculating the direction or angle of ball
	float ball_direction(int degree)
	{
		float direction = 0;
		direction = (degree * pi) / 180;
		return direction;
	}
	/*
		Creating 8 segments for the bat - Every segment is 1/8 of bat's height
		The interaction check for right bat is between
			Ball right side is hitting bat left side
			Depending on which segment (from 1/8, 2/8, 3/8... , 8/8)
			the deflect angle are different for each segment and the speeds are also different
			Segment 1 -115 degree | Segment 2 -135 degree | Segment 3 -150 degree | Segment 4 -175 degree
			Segment 5  175 degree | Segment 6  150 degree | Segment 7  135 degree | Segment 8  115 degree
			Angles/direction are calculated using cos@, sin@ and radian value

				**Determine the first segment by checking the ball center y axis (ball y axis + ball height / 2)
				is lower than bat y axis and higher than bat y axis + bat 1/8 height
				essentially between bat top and bat 1/8th height 
				(ball center > bat y axis and ball center < bat y axis + bat height / 8)

				**Determine the second segment by checking the ball center y axis
				is lower than bat y axis + bat 1/8 height and higher than bat y axis + bat 2/8 height
				essentially between bat 1/8th height and bat 2/8th height 
				(ball center > bat y axis + bat height / 8 and ball center < bat y axis + 2 * (bat height / 8))
				.
				.
				.
				**Determine the last segment by checking the ball center y axis
				is lower than bat y axis  + bat 7/8 height and higher than bat y axis + height
				essentially between bat 7/8th height and bat's bottom
				(ball center > bat y axis + bat 7 * (bat height / 8) and ball center < bat y axis + bat height)
	*/
	void check_right_bat(float ball_x, float ball_y, float x, float y, float ball_height, float ball_width, float bat_height, float bat_width)
	{
		if (ball_x + ball_width > x && ball_y + ball_height/2 > y && ball_y + ball_height/2 < y + bat_height/8)
		{
			ball_speed_x = speed_x * cos(ball_direction(-115)) * 2.5;
			ball_speed_y = speed_y * sin(ball_direction(-115));
		}
		else if (ball_x + ball_width > x && ball_y + ball_height/2 > y + bat_height / 8 && ball_y + ball_height/2 < y + 2 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(-135)) * 2;
			ball_speed_y = speed_y * sin(ball_direction(-135));
		}
		else if (ball_x + ball_width > x && ball_y + ball_height/2 > y + 2 *(bat_height / 8) && ball_y + ball_height/2 < y + 3 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(-150)) * 1.5;
			ball_speed_y = speed_y * sin(ball_direction(-150));
		}
		else if (ball_x + ball_width > x && ball_y + ball_height/2 > y + 3 * (bat_height / 8) && ball_y + ball_height/2 < y + 4 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(-175)) * 1.25;
			ball_speed_y = speed_y * sin(ball_direction(-175));
		}
		else if (ball_x + ball_width > x && ball_y + ball_height / 2 > y + 4 * (bat_height / 8) && ball_y + ball_height / 2 < y + 5 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(175)) * 1.25;
			ball_speed_y = speed_y * sin(ball_direction(175));
		}
		else if (ball_x + ball_width > x && ball_y + ball_height / 2 > y + 5 * (bat_height / 8) && ball_y + ball_height / 2 < y + 6 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(150)) * 1.5;
			ball_speed_y = speed_y * sin(ball_direction(150));
		}
		else if (ball_x + ball_width > x && ball_y + ball_height / 2 > y + 6 * (bat_height / 8) && ball_y + ball_height / 2 < y + 7 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(135)) * 2;
			ball_speed_y = speed_y * sin(ball_direction(135));
		}
		else if (ball_x + ball_width > x && ball_y + ball_height / 2 > y + 7 * (bat_height / 8) && ball_y < y + bat_height)
		{
			ball_speed_x = speed_x * cos(ball_direction(115)) * 2.5;
			ball_speed_y = speed_y * sin(ball_direction(115));
		}
		
	}
	//Same check as right bat and angle are toward the opposite direction so +- 180 degrees
	void check_left_bat(float ball_x, float ball_y, float x, float y, float ball_height, float ball_width, float bat_height, float bat_width)
	{
		if (ball_x < x + bat_width && ball_y + ball_height > y && ball_y + ball_height / 2 < y + bat_height / 8)
		{
			ball_speed_x = speed_x * cos(ball_direction(-65)) * 2.5;
			ball_speed_y = speed_y * sin(ball_direction(-65));
		}
		else if (ball_x < x + bat_width && ball_y + ball_height / 2 > y + bat_height / 8 && ball_y + ball_height / 2 < y + 2 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(-45)) * 2;
			ball_speed_y = speed_y * sin(ball_direction(-45));
		}
		else if (ball_x < x + bat_width && ball_y + ball_height / 2 > y + 2 * (bat_height / 8) && ball_y + ball_height / 2 < y + 3 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(-30)) * 1.5;
			ball_speed_y = speed_y * sin(ball_direction(-30));
		}
		else if (ball_x < x + bat_width && ball_y + ball_height / 2 > y + 3 * (bat_height / 8) && ball_y + ball_height / 2 < y + 4 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(-5)) * 1.25;
			ball_speed_y = speed_y * sin(ball_direction(-5));
		}
		else if (ball_x < x + bat_width && ball_y + ball_height / 2 > y + 4 * (bat_height / 8) && ball_y + ball_height / 2 < y + 5 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(5)) * 1.25;
			ball_speed_y = speed_y * sin(ball_direction(5));
		}
		else if (ball_x < x + bat_width && ball_y + ball_height / 2 > y + 5 * (bat_height / 8) && ball_y + ball_height / 2 < y + 6 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(30)) * 1.5;
			ball_speed_y = speed_y * sin(ball_direction(30));
		}
		else if (ball_x < x + bat_width && ball_y + ball_height / 2 > y + 6 * (bat_height / 8) && ball_y + ball_height / 2 < y + 7 * (bat_height / 8))
		{
			ball_speed_x = speed_x * cos(ball_direction(45)) * 2;
			ball_speed_y = speed_y * sin(ball_direction(45));
		}
		else if (ball_x < x + bat_width && ball_y + ball_height / 2 > y + 7 * (bat_height / 8) && ball_y < y + bat_height)
		{
			ball_speed_x = speed_x * cos(ball_direction(65)) * 2.5;
			ball_speed_y = speed_y * sin(ball_direction(65));
		}

	}
	//generating random angle for initial ball spawn when game state anew with constrains
	float random_angle(float a, float b) 
	{
		float random = ((float)rand()) / (float)RAND_MAX;
		float max_min = b - a;
		float r = random * max_min;
		return a + r;
	}	
	//Using timer to determine when to render buffs
	clock_t start_time = clock();
	
	bool buff_time = false;				//Initate buff state
	double last_buff_time = 0;			//To ensure buff time isn't overlapping
	
	void spawn_buff();					//Creating function to use in game.cpp
	bool new_buff = false;				//To check buff state
	bool active_buff_1 = false;			//To control buff 1-5 rendering state
	bool active_buff_2 = false;
	bool active_buff_3 = false;
	bool active_buff_4 = false;
	bool active_buff_5 = false;
	//Assigning buff's value
	double buff_increment = 3, buff_decrement = -2;
	//Generating buff, buff state start at 5 seconds and every 15 seconds a chance of new buff will be generated
	void random_buff()
	{
		double current_time = ((clock() - start_time) / CLOCKS_PER_SEC);
		for (double next_buff_time = 5; next_buff_time < current_time + 1; next_buff_time += 15)
		{
			if (next_buff_time > last_buff_time && current_time == next_buff_time)
			{
				buff_time = true;
				new_buff = true;
				last_buff_time = current_time;
			}
		}
	}
	/*
		Checking if buff is taken by checking if
		ball left side is touching buff right side or
		ball right side is touching buff left side or
		ball top side is touching buff bottom side or
		ball bottom side is touching buff top side
		Essentially creating a bounding box for the buff

		When buff is interacting with ball, check which direction of the ball is
		if ball is going left, the buff will apply to right player (player 2)
		if ball is going right, the buff will apply to left player (player 1)
		AI will not gain buff effect and still take the buff, however the ball will
		
		Buff 1 and 2 is giving extra speed to pat, so pat move up and down faster
		Buff 3 and 4 is giving negative speed to pat, so pat move up and down slower
		Buff 5 is giving ball extra speed so ball moves faster
		Buff effect is increment, can be stacked but with a cap
		so a player can not gain over or decrease over a set amount of speed, so is the ball
		Buff effects are remove upon entering new set of game (when someone scored)
	*/
	void check_buff(Vector buff1, Vector buff2, Vector buff3, Vector buff4, Vector buff5, Vector buffsize, float ballx, float bally, float ballh, float ballw)
	{
		if (ballx + ballw > buff1.x && ballx < buff1.x + buffsize.x && bally + ballh > buff1.y && bally < buff1.y + buffsize.y)
		{
			active_buff_1 = false;
			if (ball_speed_x > 0 && left_bat_buff_speed < 30)
			{
				left_bat_buff_speed += buff_increment;
			}
			else if (ball_speed_x < 0 && left_bat_buff_speed < 30)
			{
				right_bat_buff_speed += buff_increment;
			}
		}
		if (ballx + ballw > buff2.x && ballx < buff2.x + buffsize.x && bally + ballh > buff2.y && bally < buff2.y + buffsize.y)
		{
			active_buff_2 = false;
			if (ball_speed_x > 0 && left_bat_buff_speed < 30)
			{
				left_bat_buff_speed += buff_increment;
			}
			else if (ball_speed_x < 0 && left_bat_buff_speed < 30)
			{
				right_bat_buff_speed += buff_increment;
			}
		}
		if (ballx + ballw > buff3.x && ballx < buff3.x + buffsize.x && bally + ballh > buff3.y && bally < buff3.y + buffsize.y)
		{
			active_buff_3 = false;
			if (ball_speed_x > 0 && left_bat_buff_speed > -20)
			{
				left_bat_buff_speed += buff_decrement;
			}
			else if (ball_speed_x < 0 && left_bat_buff_speed > -20)
			{
				right_bat_buff_speed += buff_decrement;
			}
		}
		if (ballx + ballw > buff4.x && ballx < buff4.x + buffsize.x && bally + ballh > buff4.y && bally < buff4.y + buffsize.y)
		{
			active_buff_4 = false;
			if (ball_speed_x > 0 && left_bat_buff_speed > -20)
			{
				left_bat_buff_speed += buff_decrement;
			}
			else if (ball_speed_x < 0 && left_bat_buff_speed > -20)
			{
				right_bat_buff_speed += buff_decrement;
			}
		}
		if (ballx + ballw > buff5.x && ballx < buff5.x + buffsize.x && bally + ballh > buff5.y && bally < buff5.y + buffsize.y)
		{
			active_buff_5 = false;
			if (buff_speed < 20)
			{
				buff_speed += 2;
				speed_x = buff_speed;
				speed_y = buff_speed;
			}
		}
	}

	/*
		AI bat are set to 8 segments similarly to normal bat reflection on ball
		AI bat are assigned with value to move up or down depending on ball's position
		esentially following the ball
		
		There are 5 diffeent levels and accordingly to the player's score
		Every 10 scores on player, AI game will level up

		Level 1 - AI bat will try to hit the ball to its center
				- Which is between Segment 4 and Segment 5
		Level 2 - AI bat will try to hit the ball between 
				- Segment 2 and Segment 3 if ball is above half of screen height
				- Segment 6 and Segment 7 if ball is below half of screen height
		Level 3 - AI bat will try to hit the ball on
				- Segment 1 if ball is above half of screen height
				- Segment 8 if ball is below half of screen height
		
		Level 4 and 5 will be similar to 3 except AI bat movement speed

		AI bat movement speed is increased on every level
		so that when it chases the ball it will travel faster toward the ball and hit on correct segment
		Speed are set reasonbly so that even hardest level can be beaten especially with buff mechanism
	*/
	void check_ai(double bat_y, double bat_h, float ball_y, double ball_h)
	{
		float ball_center = ball_y + ball_h / 2;
		if (player2_score < 10)
		{
			if (ball_center < bat_y + 5 * (bat_h / 8) && ball_center > bat_y + 3 * (bat_h / 8))
			{
				ai_bat_speed = 0;
			}
			else if (ball_center < bat_y + 3 * (bat_h / 8))
			{
				ai_bat_speed = -3.0f;
			}
			else if (ball_center > bat_y + 5*(bat_h / 8))
			{
				ai_bat_speed = 3.0f;
			}

		}
		else if (player2_score > 9)
		{
			if (ball_y + ball_h / 2 < game_height / 2)
			{
				if (ball_center < bat_y + 3 * (bat_h / 8) && ball_center > bat_y + bat_h / 8)
				{
					ai_bat_speed = 0;
				}
				else if (ball_center < bat_y + bat_h / 8)
				{
					ai_bat_speed = -3.0;
				}
				else if (ball_center > bat_y + 3 * (bat_h / 8))
				{
					ai_bat_speed = 3.0;
				}
			}
			else
			{
				if (ball_center > bat_y + 5 * (bat_h / 8) && ball_center < bat_y + 7*(bat_h / 8))
				{
					ai_bat_speed = 0;
				}
				else if (ball_center < bat_y + 5*(bat_h / 8))
				{
					ai_bat_speed = -3.5;
				}
				else if (ball_center > bat_y + 7 * (bat_h / 8))
				{
					ai_bat_speed = 3.5;
				}
			}
			
		}
		else if (player2_score > 19)
		{
			if (ball_y + ball_h / 2 < game_height / 2)
			{
				if (ball_center < bat_y + (bat_h / 8) && ball_center > bat_y)
				{
					ai_bat_speed = 0;
				}
				else if (ball_center > bat_y + bat_h / 8)
				{
					ai_bat_speed = 4.0;
				}
			}
			else
			{
				if (ball_center > bat_y + 7 * (bat_h / 8) && ball_center < bat_y + bat_h)
				{
					ai_bat_speed = 0;
				}
				else if (ball_center < bat_y + 7 * (bat_h / 8))
				{
					ai_bat_speed = -4.0;
				}
			}
		}
		else if (player2_score > 29)
		{
			if (ball_y + ball_h / 2 < game_height / 2)
			{
				if (ball_center < bat_y + (bat_h / 8) && ball_center > bat_y)
				{
					ai_bat_speed = 0;
				}
				else if (ball_center > bat_y + bat_h / 8)
				{
					ai_bat_speed = 4.5;
				}
			}
			else
			{
				if (ball_center > bat_y + 7 * (bat_h / 8) && ball_center < bat_y + bat_h)
				{
					ai_bat_speed = 0;
				}
				else if (ball_center < bat_y + 7 * (bat_h / 8))
				{
					ai_bat_speed = -4.5;
				}
			}
		}
		else if (player2_score > 39)
		{
			if (ball_y + ball_h / 2 < game_height / 2)
			{
				if (ball_center < bat_y + (bat_h / 8) && ball_center > bat_y)
				{
					ai_bat_speed = 0;
				}
				else if (ball_center > bat_y + bat_h / 8)
				{
					ai_bat_speed = 5;
				}
			}
			else
			{
				if (ball_center > bat_y + 7 * (bat_h / 8) && ball_center < bat_y + bat_h)
				{
					ai_bat_speed = 0;
				}
				else if (ball_center < bat_y + 7 * (bat_h / 8))
				{
					ai_bat_speed = -5;
				}
			}
		}
		else if (player2_score > 49)
		{
			player_won = true;
		}
		
	}
	//Creating all sprite Object
	ASGE::Sprite* left_bat = nullptr;   
	ASGE::Sprite* right_bat = nullptr;
	ASGE::Sprite* ball = nullptr;
	ASGE::Sprite* game_background = nullptr;

	ASGE::Sprite* buff_1 = nullptr;
	ASGE::Sprite* buff_2 = nullptr;
	ASGE::Sprite* buff_3 = nullptr;
	ASGE::Sprite* buff_4 = nullptr;
	ASGE::Sprite* buff_5 = nullptr;
};

