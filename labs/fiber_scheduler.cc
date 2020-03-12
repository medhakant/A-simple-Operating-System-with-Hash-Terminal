#include "labs/fiber_scheduler.h"
void snprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count,bool* done);
void slprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count,bool* done);
void ssprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count,bool* done);
//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//
void shell_step_fiber_scheduler(shellstate_t& shellstate, addr_t main_stack,preempt_t timer,addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size,dev_lapic_t lapic){
    int turn = shellstate.fiber_sch.turn;
    int i = 0;
    arrays_size = 500;
    while(shellstate.fiber_sch.done[turn] && i<5){
        turn = (turn+1)%5;
        i++;
    }

    if(shellstate.fiber_sch.done[turn]){
        return;
    }else{
        if(!shellstate.fiber_sch.fiber_init[turn]){
            shellstate.fiber_sch.ansint[turn] = 0;
            if(shellstate.fiber_sch.command[turn]==7){
                stack_init5(stackptrs[2*turn],&arrays[(arrays_size/stackptrs_size)*turn*2],arrays_size,snprime,&stackptrs[2*turn+1],&stackptrs[2*turn],&shellstate.fiber_sch.argument[turn],&shellstate.fiber_sch.ansint[turn],&shellstate.fiber_sch.done[turn]);
                shellstate.fiber_sch.fiber_init[turn] = true; 
            }else if(shellstate.fiber_sch.command[turn]==8){
                stack_init5(stackptrs[2*turn],&arrays[(arrays_size/stackptrs_size)*turn*2],arrays_size,slprime,&stackptrs[2*turn+1],&stackptrs[2*turn],&shellstate.fiber_sch.argument[turn],&shellstate.fiber_sch.ansint[turn],&shellstate.fiber_sch.done[turn]);
                shellstate.fiber_sch.fiber_init[turn] = true; 
            }else if(shellstate.fiber_sch.command[turn]==9){
                stack_init5(stackptrs[2*turn],&arrays[(arrays_size/stackptrs_size)*turn*2],arrays_size,ssprime,&stackptrs[2*turn+1],&stackptrs[2*turn],&shellstate.fiber_sch.argument[turn],&shellstate.fiber_sch.ansint[turn],&shellstate.fiber_sch.done[turn]);
                shellstate.fiber_sch.fiber_init[turn] = true; 
            }                                      
        }
        stack_saverestore(stackptrs[2*turn+1],stackptrs[2*turn]);

        if(shellstate.fiber_sch.done[turn]){
            char answer[100];
            int alength = 0;
            int ans = shellstate.fiber_sch.ansint[turn];
            if(ans < 0){
                ans *= -1;
            }
            if(ans==0){
                answer[alength] = 0 + 48;
                alength++;
            }

            while(ans>0){
                answer[alength] = ans%10 + 48;
                ans = ans/10;
                alength++;
            }
            shellstate.fiber_sch.answer_length[turn] = alength;
            for(int i=0;i<alength;i++){
                shellstate.fiber_sch.answers[turn][i] = answer[alength-1-i];
            }
            shellstate.fiber_sch.done[turn] = true;
        }
    }
    shellstate.fiber_sch.turn = (turn+1)%5;
}

void snprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count,bool* done){
  addr_t& main_stack = *pmain_stack;
  addr_t& f_stack    = *pf_stack;
  int& pi          = *i;
  int& pcount       = *count;
  bool& pdone       = *done;

  for(int i=2;i<pi;i++){
      int j;
      int flag=0;
      for(j=2;j<=i/2;j++){
          if(i%j == 0){
              flag = 1;
              break;
          }
      }
      pdone = false;
      if(flag==0){
          pcount++;
      }
      pi = pi;
      pcount = pcount;
      pdone = pdone;
      stack_saverestore(f_stack,main_stack);
  }
  for(;;){
    pi=pi;pcount=pcount;pdone=true;stack_saverestore(f_stack,main_stack);
  }
}

void slprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count,bool* done){
  addr_t& main_stack = *pmain_stack;
  addr_t& f_stack    = *pf_stack;
  int& pi          = *i;
  int& pcount       = *count;
  bool& pdone       = *done;

  for(int i=2;i<pi;i++){
      int j;
      int flag=0;
      for(j=2;j<=pi/2;j++){
          if(i%j == 0){
              flag = 1;
              break;
          }
      }
      pdone = false;
      if(flag==0){
          pcount = i;
      }
      pi = pi;
      pcount = pcount;
      pdone = pdone;
      stack_saverestore(f_stack,main_stack);
  }
  for(;;){
    pi=pi;pcount=pcount;pdone=true;stack_saverestore(f_stack,main_stack);
  }
}

void ssprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count,bool* done){
  addr_t& main_stack = *pmain_stack;
  addr_t& f_stack    = *pf_stack;
  int& pi          = *i;
  int& pcount       = *count;
  bool& pdone       = *done;

  for(int i=2;i<pi;i++){
      int j;
      int flag=0;
      for(j=2;j<=i/2;j++){
          if(i%j == 0){
              flag = 1;
              break;
          }
      }
      pdone = false;
      if(flag==0){
          int sum = 0;
          int prime = i;
          while(prime>0){
              sum += prime%10;
              prime /= 10;
          }
          if(sum%2 != 0){
              pcount += i;
          }else{
              pcount -= i;
          }
      }
      pi = pi;
      pcount = pcount;
      pdone = pdone;
      stack_saverestore(f_stack,main_stack);
  }
  for(;;){
    pi=pi;pcount=pcount;pdone=true;stack_saverestore(f_stack,main_stack);
  }
}
