/*
* MIT License
*
* Copyright (c) 2021-2024 bitten2up
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

////////////////////////////////////////////////////////////
// Battle                                                 //
////////////////////////////////////////////////////////////
// Code for battle system                                 //
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// STD And SDL
////////////////////////////////////////////////////////////
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

//////////////////
// bitten headers
//////////////////

#include "sdl_bittendef.h"
#include "r_render.h"
#include "i_event.h"
#include "e_entity.h"
#include "bit_game.h"

// TODO: the rest of this

// root menu
static void b_battleMain()
{
  // TODO: make a menu framework, this will do for now though
  
  
  if (bitgame.menu.menupos.x == 0 && bitgame.player.inputs.right == 1)
  {
    bitgame.menu.menupos.x = 1;
  }
  else if (bitgame.menu.menupos.x == 1 && bitgame.player.inputs.left == 1)
  {
    bitgame.menu.menupos.x = 0;
  }

  // menu switching test
  else if (bitgame.player.inputs.x == 1 && bitgame.menu.menupos.x == 0 && bitgame.menu.menupos.y == 0)
  {
	  bitgame.menu.curmenu = battleAttack;

	// we don't want this input to repeat
	  bitgame.player.inputs.x = 0;
  }

  // bitgame.menu.menupos probally could just be a bitmask so then we can use switch statements..
  if (bitgame.menu.menupos.x == 0 && bitgame.menu.menupos.y == 0) // we are at the top left, so fill in that box
  	r_textbox("Attack", SCREENWIDTH/4, (SCREENHEIGHT/4)*3);
  else
  	r_text("Attack", SCREENWIDTH/4, (SCREENHEIGHT/4)*3);

  if (bitgame.menu.menupos.x == 1 && bitgame.menu.menupos.y == 0) // we are at the top right, so fill in that box
  	r_textbox("Items", (SCREENWIDTH/4)*3, (SCREENHEIGHT/4)*3);
  else
  	r_text("Items", (SCREENWIDTH/4)*3, (SCREENHEIGHT/4)*3);

  r_sprite(&bitgame.player.entitySprite);
  r_sprite(&bitgame.enemy.entitySprite);
}

void b_battleAttack()
{
  // TODO: make a menu framework, this will do for now though
  
  
  if (bitgame.menu.menupos.x == 0 && bitgame.player.inputs.right == 1)
  {
    bitgame.menu.menupos.x = 1;
  }
  else if (bitgame.menu.menupos.x == 1 && bitgame.player.inputs.left == 1)
  {
    bitgame.menu.menupos.x = 0;
  }
  r_textbox("Attack", (SCREENWIDTH/8), (SCREENHEIGHT)/3);
  // bitgame.menu.menupos probally could just be a bitmask so then we can use switch statements..
  if (bitgame.menu.menupos.x == 0 && bitgame.menu.menupos.y == 0) // we are at the top left, so fill in that box
  	r_textbox("Jabbbbbba", SCREENWIDTH/4, (SCREENHEIGHT/4)*3);
  else
  	r_text("Jabbbbbba", SCREENWIDTH/4, (SCREENHEIGHT/4)*3);

  if (bitgame.menu.menupos.x == 1 && bitgame.menu.menupos.y == 0) // we are at the top right, so fill in that box
  	r_textbox("sleep", (SCREENWIDTH/4)*3, (SCREENHEIGHT/4)*3);
  else
  	r_text("sleep", (SCREENWIDTH/4)*3, (SCREENHEIGHT/4)*3);

  r_sprite(&bitgame.player.entitySprite);
  r_sprite(&bitgame.enemy.entitySprite);
}

void b_battle() {
  r_textbox("bitten", SCREENWIDTH/8, SCREENHEIGHT/4);
  switch (bitgame.menu.curmenu)
  {
	case battleMain:
  		b_battleMain();
		break;
	case battleAttack:
	    b_battleAttack();
	  break;
	default:
		r_text("INVALID MENU", SCREENWIDTH/4, (SCREENHEIGHT/4)*3);
		break;
  }

}
