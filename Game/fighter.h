
#pragma once

#include <string>
#include "../Framework/Primitives/vector2.h"
#include "../Framework/Display/animation.h"



class Fighter
{
	public:
		enum FighterStates
		{
			Idle ,
			Jump,
			Kick,
			BackJump,
			Super,
			Ultra,
			Victor,
			Loser
		};

	private:
		SpriteSheet* spriteSheet;
		Animation* animIdle;
		Animation* animJumpTakeOff;
		Animation* animJumpFloat;
		Animation* animJumpLand;
		Animation* animKick;
		Animation* animSuper;
		Animation* animKnockDown;
		Animation* animKOLand;
		Animation* animKO;
		Animation* animWin;

		float currentScale;
		FighterStates currentState;
		int currentStateTime;
		Animation* currentAnimation;
		Vector2* currentPosition;
		bool currentFaceLeft;

	public:
		void Initialise( std::string Config );

		void CharSelect_RenderProfileIcon( int ScreenX, int ScreenY );
		void CharSelect_RenderName( int ScreenX, int ScreenY );

		void Fighter_Update();
		void Fighter_SetState( int NewState );

		Vector2* Fighter_GetPosition();
		void Fighter_SetPosition( float X, float Y );
		void Fighter_SetPosition( Vector2* NewPosition );
		void Fighter_Render( int ScreenOffsetX, int ScreenOffsetY );

		bool Fighter_IsFacingLeft();
		void Fighter_SetFacing( bool FacingLeft );

		void Fighter_JumpPressed();
		void Fighter_KickPressed();
		void Fighter_SuperPressed();

};
