//Accessing libraries
#include <Engine/Keys.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Sprite.h>
/*
	Accessing header files
	Using header files to compile easier and keep codes organised
*/
#include "Constants.h"
#include "Game.h"
#include "GameFont.h"
#include "Vector.h"


/*
	Creating constructor. 
	Mainly use for some logic that may need to be executed.
*/
Pong::Pong()
{
	
}


/*
	Creating destructor.
	Removing any non-managed memory and callbacks
	to reduce memory issues such as memory leaks
*/
Pong::~Pong()
{
	this->inputs->unregisterCallback(callback_id);				// Game Engine
	this->inputs->unregisterCallback(key_callback_id);			// Key Input
	//**this->inputs->unregisterCallback(mouse_callback_id);	// Mouse Input
	// Sprite - Background
	if (game_background)			
	{
		delete game_background;
		game_background = nullptr;
	}
	// Sprite - Left bat/paddle
	if (left_bat)			
	{
		delete left_bat;
		left_bat = nullptr;
	}
	// Sprite - Right bat/paddle
	if (right_bat)			
	{
		delete right_bat;
		right_bat = nullptr;
	}
	// Sprite - Ball
	if (ball)													
	{
		delete ball;
		ball = nullptr;
	}
	// Sprite - Buffs 1 to 5
	if (buff_1)													
	{
		delete buff_1;
		buff_1 = nullptr;
	}
	if (buff_2)
	{
		delete buff_2;
		buff_2 = nullptr;
	}
	if (buff_3)
	{
		delete buff_3;
		buff_3 = nullptr;
	}
	if (buff_4)
	{
		delete buff_4;
		buff_4 = nullptr;
	}
	if (buff_5)
	{
		delete buff_5;
		buff_5 = nullptr;
	}
	// Game fonts
	for (auto& font : GameFont::fonts)							
	{
		delete font;
		font = nullptr;
	}
}
/*
	Initialises the game.
	The game window is created and all assets required to
	run the game are loaded. The keyHandler callback will also
	be set in the initialise function. 
	Return true if the game initialised correctly.
*/
bool Pong::init()
{
	game_width  = WINDOW_WIDTH;		//Assigning game width
	game_height = WINDOW_HEIGHT;	//Assigning game height
	//Check if the game initialised correctly
	if (!initAPI())
	{
		//Stop the program from running
		return false;
	}

	//Getting key input resource
	key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &Pong::keyHandler, this);
	//Giving game basic information
	renderer->setWindowTitle("Pong");
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	renderer->setSpriteMode(ASGE::SpriteSortMode::IMMEDIATE);
	//**toggleFPS();

	//Assigning resources to sprites
	game_background = renderer->createRawSprite();
	game_background->loadTexture(".\\Resources\\Textures\\background.png");
	game_background->width(1024);
	game_background->height(720);

	buff_1 = renderer->createRawSprite();
	buff_1->loadTexture(".\\Resources\\Textures\\positive_speed_buff.png");
	buff_1->width(buff_w);
	buff_1->height(buff_h);

	buff_2 = renderer->createRawSprite();
	buff_2->loadTexture(".\\Resources\\Textures\\positive_speed_buff.png");
	buff_2->width(buff_w);
	buff_2->height(buff_h);

	buff_3 = renderer->createRawSprite();
	buff_3->loadTexture(".\\Resources\\Textures\\negative_speed_buff.png");
	buff_3->width(buff_w);
	buff_3->height(buff_h);

	buff_4 = renderer->createRawSprite();
	buff_4->loadTexture(".\\Resources\\Textures\\negative_speed_buff.png");
	buff_4->width(buff_w);
	buff_4->height(buff_h);

	buff_5 = renderer->createRawSprite();
	buff_5->loadTexture(".\\Resources\\Textures\\ball_speed_buff.png");
	buff_5->width(buff_w);
	buff_5->height(buff_h);

	left_bat = renderer->createRawSprite();
	left_bat->loadTexture(".\\Resources\\Textures\\paddle.png");
	left_bat->width(bat_w);
	left_bat->height(bat_h);
	left_bat->xPos(30);
	left_bat->yPos(296.5);

	right_bat = renderer->createRawSprite();
	right_bat->loadTexture(".\\Resources\\Textures\\paddle.png");
	right_bat->width(bat_w);
	right_bat->height(bat_h);
	right_bat->xPos(976);
	right_bat->yPos(296.5);

	ball = renderer->createRawSprite();
	ball->loadTexture(".\\Resources\\Textures\\ball.png");
	ball->width(ball_w);
	ball->height(ball_h);
	ball->xPos(506);
	ball->yPos(354);

	// enable noob mode
	inputs->use_threads = false;
	
	return true;
}


/*
    Processes any key inputs
 	This function is added as a callback to handle the game's 
	keyyboard input. For this assignment, calls to this function 
	are thread safe, so it is free to alter the game's state
*/
void Pong::keyHandler(ASGE::SharedEventData data)
{
	//Assigns the event data relating to key input.
	auto key = static_cast<const ASGE::KeyEvent*>(data.get());
	//Giving singleplayer and multiplayer mode movements on key press (up, down, W and S)
	if (key->key == ASGE::KEYS::KEY_UP)
	{
		if (!in_menu && !escape_pressed)
		{
			auto right_bat_y = right_bat->yPos();
			if (right_bat_y > 0)
			{
				right_bat_y -= initial_bat_speed + right_bat_buff_speed;
				right_bat->yPos(right_bat_y);
			}
		}
	}

	if (key->key == ASGE::KEYS::KEY_DOWN)
	{
		if (!in_menu && !escape_pressed)
		{
			auto right_bat_y = right_bat->yPos();
			if (right_bat_y < game_height - bat_h)
			{
				right_bat_y += initial_bat_speed + right_bat_buff_speed;
				right_bat->yPos(right_bat_y);
			}
		}
	}

	if (key->key == ASGE::KEYS::KEY_W)
	{
		if (!in_menu && multiplayer)
		{
			auto left_bat_y = left_bat->yPos();
			if (left_bat_y > 0)
			{
				left_bat_y -= initial_bat_speed + left_bat_buff_speed;
				left_bat->yPos(left_bat_y);
			}
		}
	}

	if (key->key == ASGE::KEYS::KEY_S)
	{
		if (!in_menu && multiplayer)
		{
			auto left_bat_y = left_bat->yPos();
			if (left_bat_y < game_height - bat_h)
			{
				left_bat_y += initial_bat_speed + left_bat_buff_speed;
				left_bat->yPos(left_bat_y);
			}
		}
	}
	//Giving user the free selection of option choices on menu and in both game modes
	if (key->key == ASGE::KEYS::KEY_UP && key->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (in_menu || escape_pressed)
		{
			menu_option -= 1;			
		}
		if (menu_option < 1)
		{
			menu_option = 0;
		}
	}

	if (key->key == ASGE::KEYS::KEY_DOWN && key->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (in_menu || escape_pressed)
		{
			menu_option += 1;
		}
		
		if (in_menu)
		{
			if (menu_option > 2)
			{
				menu_option = 2;
			}
			
		}
		else
		{
			(menu_option > 1);

			menu_option = 1;
		}
	}
	//Trigger events depending on option selected when pressing enter key 
	if (key->key == ASGE::KEYS::KEY_ENTER && key->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (in_menu)
		{
			if (menu_option == 2)
			{
				signalExit();		//Exit the game
			}

			else if (menu_option == 1)
			{
				in_menu = false;		//Setting game mode state
				multiplayer = true;		//Entering multiplayer mode
				menu_option = 0;
				newstart = true;
			}
			else if (menu_option == 0)
			{
				single_player = true;		//Entering singleplayer mode
				in_menu = false;
				menu_option = 0;
				newstart = true;
			}
		}

		else if (!in_menu && escape_pressed)		//When the game is paused in either of player mode
		{
			if (menu_option == 0)
			{
				if (!player_lost) 
				{
					ball_speed_x = temp_ball_speed_x;		//When the game is pasued, everything is set to 0 or no movement
					ball_speed_y = temp_ball_speed_y;		//Setting the game state back to before the pause
					start_time = clock();					//Essentially, continuing the game when this option is selected
					last_buff_time = 0;
					temp_ball_speed_x = 0;
					temp_ball_speed_y = 0;
					escape_pressed = false;
					player_won = false;
				}
				else
				{
					escape_pressed = false;					//Setting the gaame to new game state as player
					player1_score = 0;						//selected continue after a game is finished
					player2_score = 0;
					menu_option = 0;
					newstart = true;
					player_lost = false;
				}
			}
			else
			{
				in_menu = true;								//Trigger when player selected exit option
				multiplayer = false;						//Setting game state to default values
				single_player = false;						//Going back to menu screen
				menu_option = 0;
				player1_score = 0;
				player2_score = 0;
				escape_pressed = false;
				player_won = false;
			}
		}
	}
	//This event is mainly used to pause the game
	if (key->key == ASGE::KEYS::KEY_ESCAPE && key->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (!in_menu)
		{
			if (!escape_pressed)
			{
				escape_pressed = true;						//Saving the sprite current values such as speeds
				temp_ball_speed_x = ball_speed_x;			//Setting sprites value to 0 or no movement
				temp_ball_speed_y = ball_speed_y;			//Essentially paused the game
				ball_speed_x = 0;
				ball_speed_y = 0;
			}
			else
			{
				escape_pressed = false;						//If escape key were pressed again
				ball_speed_x = temp_ball_speed_x;			//Give the saved value back and continue the game
				ball_speed_y = temp_ball_speed_y;
				temp_ball_speed_x = 0;
				temp_ball_speed_y = 0;
				start_time = clock();
				last_buff_time = 0;
				
			}
		}
		else
		{
			signalExit();									//Exit the game when escape is pressed on menu screen
		}
	}
	

}


/**
	Updates the scene
	Prepares the renderer subsystem before drawing the
	current frame. Once the current frame is has finished
	the buffers are swapped accordingly and the image shown.
	Giving movement and changes frame by frame to the game
*/
void Pong::update(const ASGE::GameTime &)
{
	auto ball_x = ball->xPos();										//Getting sprite's information temporarily
	auto ball_y = ball->yPos();
	auto right_bat_x = right_bat->xPos();
	auto right_bat_y = right_bat->yPos();
	auto left_bat_x = left_bat->xPos();
	auto left_bat_y = left_bat->yPos();
	//Assigning current sprite information to vectors
	Vector buff_1_position(buff_1->xPos(), buff_1->yPos());
	Vector buff_2_position(buff_2->xPos(), buff_2->yPos());
	Vector buff_3_position(buff_3->xPos(), buff_3->yPos());
	Vector buff_4_position(buff_4->xPos(), buff_4->yPos());
	Vector buff_5_position(buff_5->xPos(), buff_5->yPos());
	Vector buff_size(buff_1->height(), buff_1->width());
	//Reseting the game state back to initial
	if (newstart && !in_menu)
	{
		//Setting sprites back to initial positions
		ball_x = 506;
		ball_y = 354;
		right_bat->xPos(976);
		right_bat->yPos(296.5);
		left_bat->xPos(30);
		left_bat->yPos(296.5);
		left_bat_y = left_bat->yPos();
		//Setting sprites information back to initial values
		left_bat_buff_speed = 0;									
		right_bat_buff_speed = 0;
		buff_speed = 0;
		initial_bat_speed = 10;
		//Getting rid of the (buffing) sprites that isn't in initial state originally
		active_buff_1 = false;										
		active_buff_2 = false;										
		active_buff_3 = false;
		active_buff_4 = false;
		active_buff_5 = false;

		start_time = clock();
		last_buff_time = 0;
		//Giving ball a random speed to start off with
		//Ball direction will always start by going toward loosing player
		float angle = random_angle(-pi / 16, pi / 16);
		if (ball_speed_x < 0)
		{
			ball_speed_x = -speed_x * cos(angle);
		}
		else
		{
			ball_speed_x = speed_x * cos(angle);
		}
		ball_speed_y = speed_y * sin(angle);
		//Check if game state is finished or not
		//Set values to 0 if game is finished, this way game is seen as paused when reset to new state
		if (player_lost || player_won)
		{
			ball_speed_x = 0;
			ball_speed_y = 0;
			ai_bat_speed = 0;
			initial_bat_speed = 0;
		}
		newstart = false;
	}
	//Assigning movement values of the ball
	ball_x += ball_speed_x;
	ball_y += ball_speed_y;
	//Checking bat's and ball's interaction
	check_right_bat(ball_x, ball_y, right_bat_x, right_bat_y, ball_h, ball_w, bat_h, bat_w);
	check_left_bat(ball_x, ball_y, left_bat_x, left_bat_y, ball_w, ball_h, bat_h, bat_w);
	//Bouncing ball off the top and bottom screen
	if (ball_y < 0 || ball_y + ball_h > game_height)
	{
		ball_speed_y *= -1;
	}
	//Assigning scores to winning players and also set game state to finish if certain scores are reached
	//Reset the game anew everytime new score is made
	if (ball_x > game_width - 20)
	{
		player1_score++;	
		if (player1_score == 10)
		{
			player_lost = true;
			escape_pressed = true;
			newstart = true;
		}
		else
		{
			newstart = true;
		}
	}
	else if (ball_x + ball_w < 20)
	{
		player2_score++;
		if (player2_score == 10 && multiplayer)
		{
			player_lost = true;
			escape_pressed = true;
			newstart = true;
		}
		else
		{
			newstart = true;
		}
		
	}
	//Giving values to AI for singleplayer mode 
	if (single_player)
	{
		if (left_bat_y + bat_h > game_height)
		{
			left_bat_y = game_height - bat_h;
		}
		else if (left_bat_y < 0)
		{
			left_bat_y = 0;
		}
		//Checking AI state and pulling values for AI movement
		check_ai(left_bat_y, bat_h, ball_y, ball_h);
		//For pausing state purpose
		if (escape_pressed)
		{
			ai_bat_speed = 0;
		}
		//Assigning values to AI to move
		left_bat_y += ai_bat_speed;
		left_bat->yPos(left_bat_y);
	}
	//Generating buffs sprites to the screen by calling a function to check
	if (!escape_pressed && !player_lost && !in_menu)
	{
		random_buff();
	}
	//Checks ball's and buffs' interaction
	check_buff(buff_1_position, buff_2_position, buff_3_position, buff_4_position, buff_5_position, buff_size, ball_x, ball_y, ball_h, ball_w);
	//Moving the ball
	ball->xPos(ball_x);
	ball->yPos(ball_y);
}



/*
	Renders the scene
	Renders all the game objects to the current frame.
	Once the current frame is has finished the buffers are 
	swapped accordingly and the image shown.
*/
void Pong::render(const ASGE::GameTime &)
{
	//Render the background
	renderer->renderSprite(*game_background);
	//Render texts for menu screen
	if (in_menu)
	{
		renderer->renderText("Welcome to Pong!", 420, 270, 1.2, ASGE::COLOURS::DARKBLUE);
		renderer->renderText(menu_option == 0 ? ">  1 Player" : "   1 Player", 450, 370, 1.0, ASGE::COLOURS::DARKORANGE);
		renderer->renderText(menu_option == 1 ? ">  2 Players" : "   2 Players", 450, 400, 1.0, ASGE::COLOURS::DARKORANGE);
		renderer->renderText(menu_option == 2 ? ">  Exit" : "   Exit", 450, 430, 1.0, ASGE::COLOURS::DARKORANGE);

	}
	//Render texts, resources and sprites for multiplayer mode depending on game state
	else if (multiplayer)
	{
		std::string score1_str = "P1 SCORE: " + std::to_string(player1_score);
		std::string score2_str = "P2 SCORE: " + std::to_string(player2_score);
		renderer->renderText(score1_str.c_str(), 20, 20, 1.0, ASGE::COLOURS::DARKRED);
		renderer->renderText(score2_str.c_str(), 880, 20, 1.0, ASGE::COLOURS::DARKRED);
		//Rendering resources for pausing state and finish state
		if (escape_pressed)
		{
			if (!player_lost)
			{
				renderer->renderText("PAUSED!", 480, 300, 1.2, ASGE::COLOURS::DARKORANGE);
				renderer->renderText(menu_option == 0 ? ">  Continue" : "   Continue", 450, 400, 1.0, ASGE::COLOURS::DARKORANGE);
				renderer->renderText(menu_option == 1 ? ">  Exit" : "   Exit", 450, 430, 1.0, ASGE::COLOURS::DARKORANGE);
			}
			else
			{
				renderer->renderText("Game Over!", 460, 300, 1.2, ASGE::COLOURS::DARKORANGE);
				renderer->renderText(menu_option == 0 ? ">  New Game" : "   New Game", 450, 400, 1.0, ASGE::COLOURS::DARKORANGE);
				renderer->renderText(menu_option == 1 ? ">  Exit" : "   Exit", 450, 430, 1.0, ASGE::COLOURS::DARKORANGE);

				std::string score1_str = "AI SCORED: " + std::to_string(player1_score);
				std::string score2_str = "YOU SCORED: " + std::to_string(player2_score);
				renderer->renderText(score1_str.c_str(), 360, 360, 1.0, ASGE::COLOURS::DARKRED);
				renderer->renderText(score2_str.c_str(), 540, 360, 1.0, ASGE::COLOURS::DARKRED);
			}
			
		}
		//Renders buff sprites and calling a function to see if valid
		if (buff_time && !escape_pressed)
		{
			spawn_buff();
		}
		//Render ball and bats
		renderer->renderSprite(*left_bat);
		renderer->renderSprite(*right_bat);
		renderer->renderSprite(*ball);

	}
	//Render texts, resources and sprites for singleplayer mode depending on game state
	else if (single_player)
	{
		
		std::string score1_str = "P1 SCORE: " + std::to_string(player1_score);
		std::string score2_str = "P2 SCORE: " + std::to_string(player2_score);
		renderer->renderText(score1_str.c_str(), 40, 20, 1.0, ASGE::COLOURS::DARKORANGE);
		renderer->renderText(score2_str.c_str(), 860, 20, 1.0, ASGE::COLOURS::DARKORANGE);
		// Rendering resources for pausing state and finish state
		if (escape_pressed)
		{
			if (!player_lost)
			{
				if (!player_won)
				{
					renderer->renderText("PAUSED!", 480, 300, 1.2, ASGE::COLOURS::DARKORANGE);
				}
				renderer->renderText(menu_option == 0 ? ">  Continue" : "   Continue", 450, 400, 1.0, ASGE::COLOURS::DARKORANGE);
				renderer->renderText(menu_option == 1 ? ">  Exit" : "   Exit", 450, 430, 1.0, ASGE::COLOURS::DARKORANGE);
			}
			else
			{
				renderer->renderText("You Lost!", 460, 300, 1.2, ASGE::COLOURS::DARKORANGE);
				renderer->renderText(menu_option == 0 ? ">  New Game" : "   New Game", 450, 400, 1.0, ASGE::COLOURS::DARKORANGE);
				renderer->renderText(menu_option == 1 ? ">  Exit" : "   Exit", 450, 430, 1.0, ASGE::COLOURS::DARKORANGE);

				std::string score1_str = "AI SCORED: " + std::to_string(player1_score);
				std::string score2_str = "YOU SCORED: " + std::to_string(player2_score);
				renderer->renderText(score1_str.c_str(), 360, 360, 1.0, ASGE::COLOURS::DARKRED);
				renderer->renderText(score2_str.c_str(), 540, 360, 1.0, ASGE::COLOURS::DARKRED);
			}
		}
		//Render text depending on AI levels accordingly to player's score
		if (player2_score < 10)
		{
			renderer->renderText("Level 1", 490, 20, 1.0, ASGE::COLOURS::RED);
		}
		else if (player2_score > 9 && player2_score < 20)
		{
			renderer->renderText("Level 2", 490, 20, 1.0, ASGE::COLOURS::RED);
		}
		else if (player2_score > 19 && player2_score < 30)
		{
			renderer->renderText("Level 3", 490, 20, 1.0, ASGE::COLOURS::RED);
		}
		else if (player2_score > 29 && player2_score < 40)
		{
			renderer->renderText("Level 4", 490, 20, 1.0, ASGE::COLOURS::RED);
		}
		else if (player2_score > 39 && player2_score < 50)
		{
			renderer->renderText("Level 5", 490, 20, 1.0, ASGE::COLOURS::RED);
		}
		else if (player2_score > 49 && player_won)
		{
			renderer->renderText("     Congratulations! \n You have beat the game!", 320, 300, 1.5, ASGE::COLOURS::DARKORANGE);
			escape_pressed = true;
			newstart = true;
		}
		//Renders buff sprites and calling a function to see if valid
		if (buff_time && !escape_pressed)
		{
			spawn_buff();
		}
		//Render ball and bats
		renderer->renderSprite(*left_bat);
		renderer->renderSprite(*right_bat);
		renderer->renderSprite(*ball);
	}
}
/*
	Function to check if buff is valid
	and assign values to sprites
	Renders the sprites once checked and given values to
	The sprites generated position are randomised
	The reason this function has to be here instead of header is
	to not repeat the buff sprites rendering setting for both
	single and multiplayer modes and have them both to call this instead
*/
void Pong::spawn_buff()
{
	//Setting variable for this function only
	int buff_x, buff_y;
	//Randomise buff's poisition with constrain
	buff_x = rand() % (game_width - 45 - 50) + (20 + 50);
	buff_y = rand() % (game_height - 45 - 20) + (20);
	//Assigning random poisitons to the sprites
	//Randomising which buff sprite to render (3 types of buff)
	//If same buff is chosen, it will not respawn and skip a buff
	//If all 5 buff sprites are in game, no buff sprites will be spawned or respawn until gone from game
	//Everytime a buff is spawned, chance of a buff spawn decrease 1/5 since 1 less possible buff can be spawn
	//unless it is taken out from the game
	while (new_buff)
	{
		int random_buff = rand() % 499 + 1;
		if (random_buff > 0 && random_buff < 100 && !active_buff_1)
		{
			active_buff_1 = true;
			buff_1->xPos(buff_x);
			buff_1->yPos(buff_y);
		}
		else if (random_buff > 99 && random_buff < 200 && !active_buff_2)
		{
			active_buff_2 = true;
			buff_2->xPos(buff_x);
			buff_2->yPos(buff_y);
		}
		else if (random_buff > 199 && random_buff < 300 && !active_buff_3)
		{
			active_buff_3 = true;
			buff_3->xPos(buff_x);
			buff_3->yPos(buff_y);
		}
		else if (random_buff > 299 && random_buff < 400 && !active_buff_4)
		{
			active_buff_4 = true;
			buff_4->xPos(buff_x);
			buff_4->yPos(buff_y);
		}
		else if (random_buff > 399 && random_buff < 500 && !active_buff_5)
		{
			active_buff_5 = true;
			buff_5->xPos(buff_x);
			buff_5->yPos(buff_y);
		}
		new_buff = false;
	}
	//Rendering the buff sprites and managing buff sprites thats taken out of the game
	if (active_buff_1)
	{
		renderer->renderSprite(*buff_1);
	}
	else
	{
		buff_1->xPos(game_width);
		buff_1->yPos(game_height);
		renderer->renderSprite(*buff_1);
	}
	
	if (active_buff_2)
	{
		renderer->renderSprite(*buff_2);
	}
	else
	{
		buff_2->xPos(game_width);
		buff_2->yPos(game_height);
		renderer->renderSprite(*buff_2);
	}
	if (active_buff_3)
	{
		renderer->renderSprite(*buff_3);
	}
	else
	{
		buff_3->xPos(game_width);
		buff_3->yPos(game_height);
		renderer->renderSprite(*buff_3);
	}
	if (active_buff_4)
	{
		renderer->renderSprite(*buff_4);
	}
	else
	{
		buff_4->xPos(game_width);
		buff_4->yPos(game_height);
		renderer->renderSprite(*buff_4);
	}
	if (active_buff_5)
	{
		renderer->renderSprite(*buff_5);
	}
	else
	{
		buff_5->xPos(game_width);
		buff_5->yPos(game_height);
		renderer->renderSprite(*buff_5);
	}

}