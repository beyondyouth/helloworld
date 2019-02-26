#ifndef COMMON_H_
#define COMMON_H_


typedef enum
{
    UP = 0,					//上
    LEFT,					//左
    DOWN,					//下
    RIGHT					//右
}dir;

typedef enum
{
	LINK_REQ,				//等待接收请求状态
	LINK_RSP,				//等待接收响应状态
	LINK_WAIT,				//等待按键确认状态
	LINK_OK,				//协商成功
	LINK_ERR
}L_state;

typedef enum
{
	GAME_START,				//游戏开始
	GAME_SELECT,			//配置对手
	GAME_FIGHT,				//对战状态
	GAME_PAUSE,				//暂停状态
	GAME_OVER,				//胜负已分
	GAME_EXIT				//退出
}G_state;


extern L_state _link_state;
extern G_state _game_state;

#endif /* COMMON_H_ */