#include "labs/coroutine.h"


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){
    if(shellstate.enter_pressed){
        if(shellstate.command_set && !shellstate.command_invalid){
            if(shellstate.command==5){
                if(!shellstate.coro_init){
                    coroutine_reset(f_coro);
                    shellstate.coro_init = true;
                }
                
                int line_curr = shellstate.line_num;
                int& i = f_locals.i;
                int& count= f_locals.ans;
                h_begin(f_coro);
                int line_prev = line_curr-1;
                if(line_prev<0){
                    line_prev += 19;
                }

                int length = shellstate.input_length[line_prev]-shellstate.command_length;
                char argument[length];
                for(int i=0;i < shellstate.input_length[line_prev]-shellstate.command_length;i++){
                    argument[i] = shellstate.input[line_prev][i+shellstate.command_length];
                }

                int num = stoi(argument,length);
                count = 0;
                if(num>1){
                    int j;
                    int flag=0;
                    for(i=num;i>1;i--){
                        flag = 0;
                        for(j=2;j<=i/2;j++){
                            if(i%j == 0){
                                flag = 1;
                                break;
                            }
                        }
                        if(flag==0){
                            count ++;
                        }
                        h_yield(f_coro);
                    }
                }

                char answer[100];
                int alength = 0;
                if(count==0){
                    answer[alength] = 0 + 48;
                    alength++;
                }

                while(count>0){
                    answer[alength] = count%10 + 48;
                    count = count/10;
                    alength++;
                }
                shellstate.input_length[line_curr] = alength;
                for(int i=0;i<alength;i++){
                    shellstate.input[line_curr][i] = answer[alength-1-i];
                }
                shellstate.result_set = true;
                shellstate.command_set = false;
                shellstate.command_invalid = false;
                shellstate.command_length = 10;

                shellstate.enter_pressed = false;
                shellstate.space_pressed = false;
                shellstate.coro_init = false;
                h_end(f_coro);
            }

        }
    }

}


