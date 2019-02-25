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
	LINK_REQ,
	LINK_RSP,
	LINK_WAIT,
	LINK_OK,
	LINK_ERR
}L_state;

typedef enum
{
	GAME_START,
	GAME_SELECT,
	GAME_FIGHT,
	GAME_OVER,
	GAME_EXIT
}G_state;


extern L_state _link_state;
extern G_state _game_state;

#endif /* COMMON_H_ */