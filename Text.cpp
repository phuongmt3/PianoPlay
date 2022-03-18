#include "Game.h"

Text::Text()
{
    if( gFont == NULL )
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
}
