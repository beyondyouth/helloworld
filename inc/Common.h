#ifndef COMMON_H_
#define COMMON_H_

typedef enum
{
    UP = 0,
    LEFT,
    DOWN,
    RIGHT
}dir;

typedef enum
{
	GAME_START,
	GAME_SELECT,
	GAME_FIGHT,
	GAME_OVER,
	GAME_EXIT
}G_state;


extern G_state _game_state;

#endif /* COMMON_H_ */