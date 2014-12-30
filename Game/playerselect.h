
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"
#include "fighter.h"

class PlayerSelect : public Stage
{
	private:
		ConfigFile* datalists;

		int p1charidx;
		Fighter* p1char;

		int p2charidx;
		Fighter* p2char;

		Arena* gameArena;

		void ConstructPlayer1( Fighter::FighterController Controller, std::string Character );
		void ConstructPlayer2( Fighter::FighterController Controller, std::string Character );

		int GetPlayerWithControls( Fighter::FighterController Controller );

  public:

		PlayerSelect( Fighter::FighterController Player1Controls, Fighter::FighterController Player2Controls );

    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void EventOccurred(Event *e);
    virtual void Update();
    virtual void Render();
		virtual bool IsTransition();
};
