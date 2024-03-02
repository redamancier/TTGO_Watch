#ifndef __HAND_USER_H
#define __HAND_USER_H

#include "hand_user.h"
#include "hand.h"
#include "hand_port.h"

void hand_init_again(void);
/*
hand_init_again();
while(xxx)
{
    hand_handler();
    delay_ms(5);
}

*/
void hand_init(hand_t* hand); 
/*
simple use:

hand_t hand_structure;
hand_init(&hand_structure);
*/
void hand_handler(void); // 5ms调用 
// key -> key handler  -> off key 

unsigned char hand_return_action_usefel();

feature_class hand_return_cur_action();

gravity_class hand_return_cur_gravity();

// typedef enum
// {
// 	none = 0,
// 	palm = 1, //翻腕
// 	applause = 2,//鼓掌
// 	put = 3//抬手
// }feature_class;

#endif