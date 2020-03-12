#include "labs/fiber.h"

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, preempt_t timer,addr_t& f_stack, addr_t f_array, uint32_t f_arraysize, dev_lapic_t lapic){
  if(shellstate.enter_pressed){
        if(shellstate.command_set && !shellstate.command_invalid){
            if(shellstate.command==6){
              int n,count;
              if(!shellstate.fiber_init){
                int line_prev = shellstate.line_num -1;
                if(line_prev<0){
                    line_prev += 19;
                }
                int length = shellstate.input_length[line_prev]-shellstate.command_length;
                char argument[length];
                for(int i=0;i < shellstate.input_length[line_prev]-shellstate.command_length;i++){
                    argument[i] = shellstate.input[line_prev][i+shellstate.command_length];
                }

                int num = stoi(argument,length);
                if(num>1){
                  n = num;
                  count = 0;
                  stack_init4(f_stack,&f_array, f_arraysize,fnprime,&main_stack,&f_stack,&n,&count);
                  shellstate.fiber_init = true;
                }                                      
              }
              stack_saverestore(main_stack,f_stack);
              if(n<2){
                int line_curr = shellstate.line_num;
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
                shellstate.fiber_init = false;
              }
            }

        }
    }
}

void fnprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count){
  addr_t& main_stack = *pmain_stack;
  addr_t& f_stack    = *pf_stack;
  int& pi          = *i;
  int& pcount       = *count;

  int j;
  int flag=0;
  for(pi;pi>1;pi--){
      flag = 0;
      for(j=2;j<=pi/2;j++){
          if(pi%j == 0){
              flag = 1;
              break;
          }
      }
      if(flag==0){
          pcount ++;
      }
      pi = pi;
      pcount = pcount;
      stack_saverestore(f_stack,main_stack);
  }

  for(;;){
    stack_saverestore(f_stack,main_stack);
  }
}


