
#include "playerselect.h"
#include "arena.h"
#include "menu.h"

PlayerSelect::PlayerSelect( Fighter::FighterController Player1Controls, Fighter::FighterController Player2Controls )
{
	datalists = new ConfigFile( "resources/data.txt" );

	p1charidx = 0;
	p1char = nullptr;
	if( Player1Controls != Fighter::FighterController::NoControls )
	{
		ConstructPlayer1( Player1Controls, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
	}

	p2charidx = 0;
	p2char = nullptr;
	if( Player2Controls != Fighter::FighterController::NoControls )
	{
		ConstructPlayer2( Player2Controls, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
	}

}

void PlayerSelect::Begin()
{
}

void PlayerSelect::Pause()
{
}

void PlayerSelect::Resume()
{
}

void PlayerSelect::Finish()
{
}

void PlayerSelect::EventOccurred(Event *e)
{
	Fighter::FighterController source = Fighter::FighterController::NoControls;
	bool sourceisjump = false;

	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Keyboard.Jump", ALLEGRO_KEY_LSHIFT ) )
		{
			source = Fighter::FighterController::LocalKeyboardP1;
			sourceisjump = true;
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Keyboard.Kick", ALLEGRO_KEY_LCTRL ) )
		{
			source = Fighter::FighterController::LocalKeyboardP1;
			sourceisjump = false;
		}

		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Jump", ALLEGRO_KEY_RSHIFT ) )
		{
			source = Fighter::FighterController::LocalKeyboardP2;
			sourceisjump = true;
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Kick", ALLEGRO_KEY_RCTRL ) )
		{
			source = Fighter::FighterController::LocalKeyboardP2;
			sourceisjump = false;
		}
	}

	if( e->Type == EVENT_JOYSTICK_BUTTON_DOWN )
	{
		Menu* menustage = (Menu*)FRAMEWORK->ProgramStages->Previous();

		// Setup joystick controls
		if( menustage->Player1Joystick != e->Data.Joystick.ID && menustage->Player2Joystick != e->Data.Joystick.ID )
		{
			if( menustage->Player1Joystick == -1 )
			{
				menustage->Player1Joystick = e->Data.Joystick.ID;
			} else if ( menustage->Player2Joystick == -1 ) {
				menustage->Player2Joystick = e->Data.Joystick.ID;
			}
		}

		if( e->Data.Joystick.ID == menustage->Player1Joystick )
		{
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Joystick.Jump", 0 ) )
			{
				source = Fighter::FighterController::LocalJoystickP1;
				sourceisjump = true;
			}
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Joystick.Kick", 1 ) )
			{
				source = Fighter::FighterController::LocalJoystickP1;
				sourceisjump = false;
			}
		}
		if( e->Data.Joystick.ID == menustage->Player2Joystick )
		{
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Joystick.Jump", 0 ) )
			{
				source = Fighter::FighterController::LocalJoystickP2;
				sourceisjump = true;
			}
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Joystick.Kick", 1 ) )
			{
				source = Fighter::FighterController::LocalJoystickP2;
				sourceisjump = false;
			}
		}
	}


	if( source != Fighter::FighterController::NoControls )
	{
		int activeplayer = GetPlayerWithControls( source );
		switch( activeplayer )
		{
			case 1:
				if( sourceisjump )
				{
					p1charidx = (p1charidx + 1) % datalists->GetArraySize( "Characters" );
					ConstructPlayer1( source, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
				} else {
					p1char->Fighter_SetState( Fighter::FighterStates::Victor );
				}
				break;

			case 2:
				if( sourceisjump )
				{
					p2charidx = (p2charidx + 1) % datalists->GetArraySize( "Characters" );
					ConstructPlayer2( source, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
				} else {
					p2char->Fighter_SetState( Fighter::FighterStates::Victor );
				}
				break;

			case 0:
				if( p1char == nullptr )
				{
					ConstructPlayer1( source, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
				} else if ( p2char == nullptr ) {
					ConstructPlayer2( source, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
				}
				break;
		}

	}

}

void PlayerSelect::Update()
{
	Arena* ingame;

	if( p1char != 0 )
	{
		p1char->Fighter_Update( true );
	}
	if( p2char != 0 )
	{
		p2char->Fighter_Update( true );
	}

	if( p1char != nullptr && p2char != nullptr )
	{
		if( p1char->Fighter_GetState() == Fighter::FighterStates::Victor && p2char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Both players have selected their fighters
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
			return;
		}
	}

	if( p1char != nullptr && p2char == nullptr )
	{
		if( p1char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Start up single player game
			ConstructPlayer2( Fighter::FighterController::CPU_Easy, *datalists->GetQuickStringValue( "Characters", rand() % datalists->GetArraySize( "Characters" ), "" ) );
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
			return;
		}
	}

	if( p1char == nullptr && p2char != nullptr )
	{
		if( p2char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Start up single player game
			ConstructPlayer1( Fighter::FighterController::CPU_Easy, *datalists->GetQuickStringValue( "Characters", rand() % datalists->GetArraySize( "Characters" ), "" ) );
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
			return;
		}
	}

}

void PlayerSelect::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	if( p1char != 0 )
	{
		p1char->Fighter_Render( 0, 0 );
	}

	if( p2char != 0 )
	{
		p2char->Fighter_Render( 0, 0 );
	}
}

bool PlayerSelect::IsTransition()
{
	return false;
}

void PlayerSelect::ConstructPlayer1( Fighter::FighterController Controller, std::string Character )
{
	p1char = new Fighter( Controller, Character.c_str(), 0, false );
	p1char->Fighter_SetPosition( DISPLAY->GetWidth() / 4, 0 );
	p1char->Fighter_SetState( Fighter::Idle );
	p1char->Fighter_SetFacing( false );
}

void PlayerSelect::ConstructPlayer2( Fighter::FighterController Controller, std::string Character )
{
	p2char = new Fighter( Controller, Character.c_str(), 0, true );
	p2char->Fighter_SetPosition( (DISPLAY->GetWidth() / 4) * 3, 0 );
	p2char->Fighter_SetState( Fighter::Idle );
	p2char->Fighter_SetFacing( true );
}

int PlayerSelect::GetPlayerWithControls( Fighter::FighterController Controller )
{
	if( p1char != nullptr )
	{
		if( p1char->Controller == Controller )
		{
			return 1;
		}
	}
	if( p2char != nullptr )
	{
		if( p2char->Controller == Controller )
		{
			return 2;
		}
	}
	return 0;
}
