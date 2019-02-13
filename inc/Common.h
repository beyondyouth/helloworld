#ifndef COMMON_H_
#define COMMON_H_

enum L_state
{
	LINK_FAILED,
	LINK_ACCEPT,
	LINK_CONNECT,
	LINK_SUCCESS,
	LINK_ABORT
};

enum G_state
{
	GAME_START,
	GAME_SELECT,
	GAME_LINK,
	GAME_FIGHT,
	GAME_OVER,
	GAME_EXIT
};

enum G_signal
{
	SIGN_NO,
	SIGN_PAUSE,
	SIGN_EXIT
};

#endif /* COMMON_H_ */