#pragma once
#include "util/config.h"
#include "util/debug.h"

struct fiber_s{
    int slot;
    int turn;
    int command[5];
    int argument[5];
    int ansint[5];
    char input[5][20];
    int input_length[5];
    char answers[5][10];
    int answer_length[5];
    bool fiber_init[5];
    bool done[5];
};

struct shellstate_t{
    int num_key_pressed;
    char input[20][80];
    int input_length[20];
    bool command_set;
    bool result_set;
    bool coro_init;
    bool fiber_init;
    bool fiber_sch_result;
    int command;
    int command_length;
    int line_num;
    bool space_pressed;
    bool enter_pressed;
    bool command_invalid;
    bool screen_roll;
    fiber_s fiber_sch;    
};

struct renderstate_t{
    int num_key_pressed;
    char input[20][80];
    int input_length[20];
    bool command_set;
    bool result_set;
    int command;
    int line_num;
    bool command_invalid;
    bool screen_roll;
    bool fiber_sch_result;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

int stoi(char* str,int length);

void cpystr(char* from,char* to,int length);

