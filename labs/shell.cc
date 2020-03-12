#include "labs/shell.h"
#include "labs/vgatext.h"

//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  state.num_key_pressed = 0;
  for(int i=0;i<20;i++){
    state.input_length[i] = 0;
  }
  int l = 0;
  state.line_num = l;
  state.space_pressed = false;
  state.enter_pressed = false;
  state.command_set = false;
  state.coro_init = false;
  state.fiber_init = false;
  state.command_invalid = false;
  state.command_length = 10;
  state.input[l][0] = 'u';
  state.input[l][1] = 's';
  state.input[l][2] = 'e';
  state.input[l][3] = 'r';
  state.input[l][4] = '@';
  state.input[l][5] = 'h';
  state.input[l][6] = 'o';
  state.input[l][7] = 'h';
  state.input[l][8] = '$';
  state.input[l][9] = ' ';
  state.input_length[l] = 10;
  state.screen_roll = false;
  state.fiber_sch.slot = 0;
  for(int i=0;i<5;i++){
    state.fiber_sch.done[i] = true;
    cpystr("                    ",state.fiber_sch.input[i],20);
    cpystr("          ",state.fiber_sch.answers[i],10);
    state.fiber_sch.input_length[i] = 20;
    state.fiber_sch.answer_length[i] = 0;    
  }
}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//
void shell_update(uint8_t scankey, shellstate_t& stateinout){

    hoh_debug("Got: "<<unsigned(scankey));
    stateinout.num_key_pressed += 1;
    if(stateinout.result_set){
      stateinout.line_num += 1;
      if(stateinout.line_num>=19){
        stateinout.screen_roll = true;
        stateinout.line_num = stateinout.line_num%19;
      }
      stateinout.input_length[stateinout.line_num] = 0;
      stateinout.result_set = false;
      stateinout.command_set = false;
      stateinout.command_length = 10;
    }
    int line_num = stateinout.line_num;
    if(stateinout.input_length[line_num]==0){
      stateinout.input[line_num][0] = 'u';
      stateinout.input[line_num][1] = 's';
      stateinout.input[line_num][2] = 'e';
      stateinout.input[line_num][3] = 'r';
      stateinout.input[line_num][4] = '@';
      stateinout.input[line_num][5] = 'h';
      stateinout.input[line_num][6] = 'o';
      stateinout.input[line_num][7] = 'h';
      stateinout.input[line_num][8] = '$';
      stateinout.input[line_num][9] = ' ';
      stateinout.input_length[line_num] = 10;
    }
    if(scankey==unsigned(2)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '1';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(3)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '2';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(4)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '3';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(5)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '4';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(6)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '5';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(7)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '6';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(8)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '7';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(9)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '8';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(10)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '9';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(11)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = '0';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(14)){
     if(stateinout.input_length[line_num]>stateinout.command_length){
        stateinout.input_length[line_num]--;
     }
    }else if(scankey==unsigned(16)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'q';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(17)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'w';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(18)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'e';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(19)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'r';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(20)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 't';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(21)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'y';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(22)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'u';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(23)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'i';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(24)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'o';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(25)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'p';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(30)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'a';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(31)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 's';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(32)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'd';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(33)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'f';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(34)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'g';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(35)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'h';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(35)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'j';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(37)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'k';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(38)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'l';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(44)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'z';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(45)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'x';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(46)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'c';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(47)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'v';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(48)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'b';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(49)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'n';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(50)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = 'm';
      stateinout.input_length[line_num] += 1;
    }else if(scankey==unsigned(57)){
      stateinout.input[line_num][stateinout.input_length[line_num]] = ' ';
      stateinout.input_length[line_num] += 1;
      stateinout.space_pressed = true;
    }else if(scankey==unsigned(28)){
      stateinout.line_num += 1;
      if(stateinout.line_num>=19){
        stateinout.screen_roll = true;
        stateinout.line_num = stateinout.line_num%19;
      }
      stateinout.enter_pressed = true;
    }
}


//
// do computation
//
void shell_step(shellstate_t& stateinout){
  int line_curr = stateinout.line_num;
  if(stateinout.space_pressed){
    if(!stateinout.command_set || stateinout.command_invalid){
      stateinout.command_invalid = true;
      stateinout.command_set = true;
      if(stateinout.input_length[line_curr]==15 && 
      stateinout.input[line_curr][10]=='e' &&
      stateinout.input[line_curr][11]=='c' &&
      stateinout.input[line_curr][12]=='h' &&
      stateinout.input[line_curr][13]=='o'){

        stateinout.command_length = 15;
        stateinout.command_invalid = false;
        stateinout.command = 1;
      }else if(stateinout.input_length[line_curr]==15 && 
      stateinout.input[line_curr][10]=='f' &&
      stateinout.input[line_curr][11]=='a' &&
      stateinout.input[line_curr][12]=='c' &&
      stateinout.input[line_curr][13]=='t'){

        stateinout.command_length = 15;
        stateinout.command_invalid = false;
        stateinout.command = 2;
      }else if(stateinout.input_length[line_curr]==15 && 
      stateinout.input[line_curr][10]=='h' &&
      stateinout.input[line_curr][11]=='e' &&
      stateinout.input[line_curr][12]=='l' &&
      stateinout.input[line_curr][13]=='p'){

        stateinout.command_length = 15;
        stateinout.command_invalid = false;
        stateinout.command = 0;
      }else if(stateinout.input_length[line_curr]==14 && 
      stateinout.input[line_curr][10]=='f' &&
      stateinout.input[line_curr][11]=='i' &&
      stateinout.input[line_curr][12]=='b'){

        stateinout.command_length = 14;
        stateinout.command_invalid = false;
        stateinout.command = 3;
      }else if(stateinout.input_length[line_curr]==17 && 
      stateinout.input[line_curr][10]=='n' &&
      stateinout.input[line_curr][11]=='p' &&
      stateinout.input[line_curr][12]=='r' &&
      stateinout.input[line_curr][13]=='i' &&
      stateinout.input[line_curr][14]=='m' &&
      stateinout.input[line_curr][15]=='e'){

        stateinout.command_length = 17;
        stateinout.command_invalid = false;
        stateinout.command = 4;
      }else if(stateinout.input_length[line_curr]==18 && 
      stateinout.input[line_curr][10]=='c' &&
      stateinout.input[line_curr][11]=='n' &&
      stateinout.input[line_curr][12]=='p' &&
      stateinout.input[line_curr][13]=='r' &&
      stateinout.input[line_curr][14]=='i' &&
      stateinout.input[line_curr][15]=='m' &&
      stateinout.input[line_curr][16]=='e'){

        stateinout.command_length = 18;
        stateinout.command_invalid = false;
        stateinout.command = 5;
      }else if(stateinout.input_length[line_curr]==18 && 
      stateinout.input[line_curr][10]=='f' &&
      stateinout.input[line_curr][11]=='n' &&
      stateinout.input[line_curr][12]=='p' &&
      stateinout.input[line_curr][13]=='r' &&
      stateinout.input[line_curr][14]=='i' &&
      stateinout.input[line_curr][15]=='m' &&
      stateinout.input[line_curr][16]=='e'){

        stateinout.command_length = 18;
        stateinout.command_invalid = false;
        stateinout.command = 6;
      }else if(stateinout.input_length[line_curr]==18 && 
      stateinout.input[line_curr][10]=='s' &&
      stateinout.input[line_curr][11]=='n' &&
      stateinout.input[line_curr][12]=='p' &&
      stateinout.input[line_curr][13]=='r' &&
      stateinout.input[line_curr][14]=='i' &&
      stateinout.input[line_curr][15]=='m' &&
      stateinout.input[line_curr][16]=='e'){

        stateinout.command_length = 18;
        stateinout.command_invalid = false;
        stateinout.command = 7;
      }else if(stateinout.input_length[line_curr]==18 && 
      stateinout.input[line_curr][10]=='s' &&
      stateinout.input[line_curr][11]=='l' &&
      stateinout.input[line_curr][12]=='p' &&
      stateinout.input[line_curr][13]=='r' &&
      stateinout.input[line_curr][14]=='i' &&
      stateinout.input[line_curr][15]=='m' &&
      stateinout.input[line_curr][16]=='e'){

        stateinout.command_length = 18;
        stateinout.command_invalid = false;
        stateinout.command = 8;
      }else if(stateinout.input_length[line_curr]==18 && 
      stateinout.input[line_curr][10]=='s' &&
      stateinout.input[line_curr][11]=='s' &&
      stateinout.input[line_curr][12]=='p' &&
      stateinout.input[line_curr][13]=='r' &&
      stateinout.input[line_curr][14]=='i' &&
      stateinout.input[line_curr][15]=='m' &&
      stateinout.input[line_curr][16]=='e'){

        stateinout.command_length = 18;
        stateinout.command_invalid = false;
        stateinout.command = 9;
      }
      
    }
    stateinout.space_pressed = false;
  }else if(stateinout.enter_pressed){
    int line_prev = line_curr-1;
    if(line_prev<0){
      line_prev += 19;
    }
    if(!stateinout.command_set){
      if(stateinout.input_length[line_prev]==14 && 
      stateinout.input[line_prev][10]=='h' &&
      stateinout.input[line_prev][11]=='e' &&
      stateinout.input[line_prev][12]=='l' &&
      stateinout.input[line_prev][13]=='p'){
        stateinout.command_set = true;
        stateinout.command_length = 14;
        stateinout.command_invalid = false;
        stateinout.command = 0;
      }else if(stateinout.input_length[line_prev]==14 && 
      stateinout.input[line_prev][10]=='e' &&
      stateinout.input[line_prev][11]=='c' &&
      stateinout.input[line_prev][12]=='h' &&
      stateinout.input[line_prev][13]=='o'){
        stateinout.command_set = true;
        stateinout.command_length = 14;
        stateinout.command_invalid = false;
        stateinout.command = 1;
      }else if(stateinout.input_length[line_prev]==13 && 
      stateinout.input[line_prev][10]=='r' &&
      stateinout.input[line_prev][11]=='e' &&
      stateinout.input[line_prev][12]=='s'){
        stateinout.command_set = true;
        stateinout.command_length = 13;
        stateinout.command_invalid = false;
        stateinout.command = 10;
      }else if(stateinout.input_length[line_prev]>10){
        stateinout.command_set = true;
        stateinout.command_invalid = true;
      }
    }

    if(stateinout.command_set && !stateinout.command_invalid){
      if(stateinout.command == 0){
        cpystr("available commands: fact, fib, echo, nprime, help, nprime",stateinout.input[line_curr],56);
        stateinout.input_length[line_curr] = 56;
        stateinout.result_set = true;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;
      }else if(stateinout.command == 1){
        for(int i=0;i < stateinout.input_length[line_prev]-stateinout.command_length;i++){
          stateinout.input[line_curr][i] = stateinout.input[line_prev][i+stateinout.command_length];
        }
        stateinout.result_set = true;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;
        stateinout.input_length[line_curr] = stateinout.input_length[line_prev]-stateinout.command_length;  
      }else if(stateinout.command==2){
        int length = stateinout.input_length[line_prev]-stateinout.command_length;
        char argument[length];
        for(int i=0;i < stateinout.input_length[line_prev]-stateinout.command_length;i++){
          argument[i] = stateinout.input[line_prev][i+stateinout.command_length];
        }

        int num = stoi(argument,length);
        int fact=1;
        for(int i=0;i<num;i++){
          fact *= (i+1);
        }

        char answer[100];
        int alength = 0;

        while(fact>0){
          answer[alength] = fact%10 + 48;
          fact = fact/10;
          alength++;
        }
        stateinout.input_length[line_curr] = alength;
        for(int i=0;i<alength;i++){
          stateinout.input[line_curr][i] = answer[alength-1-i];
        }
               
        stateinout.result_set = true;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;

      }else if(stateinout.command==3){
        int length = stateinout.input_length[line_prev]-stateinout.command_length;
        char argument[length];
        for(int i=0;i < stateinout.input_length[line_prev]-stateinout.command_length;i++){
          argument[i] = stateinout.input[line_prev][i+stateinout.command_length];
        }

        int num = stoi(argument,length);
        int fib[num];
        fib[0] = 0;
        fib[1] = 1;

        for(int i=2;i<=num;i++){
          fib[i] = fib[i-1] + fib[i-2];
        }

        char answer[100];
        int alength = 0;

        if(fib[num]==0){
          answer[alength] = 0 + 48;
          alength++;
        }
        while(fib[num]>0){
          answer[alength] = fib[num]%10 + 48;
          fib[num] = fib[num]/10;
          alength++;
        }
        stateinout.input_length[line_curr] = alength;
        for(int i=0;i<alength;i++){
          stateinout.input[line_curr][i] = answer[alength-1-i];
        }
               
        stateinout.result_set = true;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;

      }else if(stateinout.command==4){

        int length = stateinout.input_length[line_prev]-stateinout.command_length;
        char argument[length];
        for(int i=0;i < stateinout.input_length[line_prev]-stateinout.command_length;i++){
          argument[i] = stateinout.input[line_prev][i+stateinout.command_length];
        }

        int num = stoi(argument,length);
        int count=0;
        if(num>1){
          int i,j;
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
        stateinout.input_length[line_curr] = alength;
        for(int i=0;i<alength;i++){
          stateinout.input[line_curr][i] = answer[alength-1-i];
        }
              
        stateinout.result_set = true;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;
      }else if(stateinout.command==7){

        int length = stateinout.input_length[line_prev]-stateinout.command_length;
        char argument[length];
        for(int i=0;i < stateinout.input_length[line_prev]-stateinout.command_length;i++){
          argument[i] = stateinout.input[line_prev][i+stateinout.command_length];
        }

        int num = stoi(argument,length);
        int slot = stateinout.fiber_sch.slot;
        stateinout.fiber_sch.argument[slot] = num;
        stateinout.fiber_sch.answer_length[slot] = 0;
        stateinout.fiber_sch.done[slot] = false;
        stateinout.fiber_sch.fiber_init[slot]=false;
        stateinout.fiber_sch.command[slot] = 7;
        length = stateinout.input_length[line_prev]-10;
        stateinout.fiber_sch.input_length[slot] = length;
        for(int i=0;i<length;i++){
          stateinout.fiber_sch.input[slot][i] = stateinout.input[line_prev][i+10];
        }
        slot = (slot+1)%5;
        stateinout.fiber_sch.slot = slot;
        stateinout.result_set = false;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;
      }else if(stateinout.command==8){

        int length = stateinout.input_length[line_prev]-stateinout.command_length;
        char argument[length];
        for(int i=0;i < stateinout.input_length[line_prev]-stateinout.command_length;i++){
          argument[i] = stateinout.input[line_prev][i+stateinout.command_length];
        }

        int num = stoi(argument,length);
        int slot = stateinout.fiber_sch.slot;
        stateinout.fiber_sch.argument[slot] = num;
        stateinout.fiber_sch.answer_length[slot] = 0;
        stateinout.fiber_sch.done[slot] = false;
        stateinout.fiber_sch.fiber_init[slot]=false;
        stateinout.fiber_sch.command[slot] = 8;
        length = stateinout.input_length[line_prev]-10;
        stateinout.fiber_sch.input_length[slot] = length;
        for(int i=0;i<length;i++){
          stateinout.fiber_sch.input[slot][i] = stateinout.input[line_prev][i+10];
        }
        slot = (slot+1)%5;
        stateinout.fiber_sch.slot = slot;
        stateinout.result_set = false;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;
      }else if(stateinout.command==9){

        int length = stateinout.input_length[line_prev]-stateinout.command_length;
        char argument[length];
        for(int i=0;i < stateinout.input_length[line_prev]-stateinout.command_length;i++){
          argument[i] = stateinout.input[line_prev][i+stateinout.command_length];
        }

        int num = stoi(argument,length);
        int slot = stateinout.fiber_sch.slot;
        stateinout.fiber_sch.argument[slot] = num;
        stateinout.fiber_sch.answer_length[slot] = 0;
        stateinout.fiber_sch.done[slot] = false;
        stateinout.fiber_sch.fiber_init[slot]=false;
        stateinout.fiber_sch.command[slot] = 9;
        length = stateinout.input_length[line_prev]-10;
        stateinout.fiber_sch.input_length[slot] = length;
        for(int i=0;i<length;i++){
          stateinout.fiber_sch.input[slot][i] = stateinout.input[line_prev][i+10];
        }
        slot = (slot+1)%5;
        stateinout.fiber_sch.slot = slot;
        stateinout.result_set = false;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;
      }else if(stateinout.command == 10){
        int slot = stateinout.fiber_sch.slot;
        for(int i=0;i<50;i++){
          stateinout.input[line_curr][i]=' ';
        }
        for(int i=0;i<5;i++){
          int turn = (slot+i)%5;
          if(!stateinout.fiber_sch.done[turn]){
            stateinout.input[line_curr][i*10+0] = 'P';
            stateinout.input[line_curr][i*10+1] = 'e';
            stateinout.input[line_curr][i*10+2] = 'n';
            stateinout.input[line_curr][i*10+3] = 'd';
            stateinout.input[line_curr][i*10+4] = 'i';
            stateinout.input[line_curr][i*10+5] = 'n';
            stateinout.input[line_curr][i*10+6] = 'g';
          }else if(stateinout.fiber_sch.answer_length[turn]==0){
            stateinout.input[line_curr][i*10+0] = 'E';
            stateinout.input[line_curr][i*10+1] = 'm';
            stateinout.input[line_curr][i*10+2] = 'p';
            stateinout.input[line_curr][i*10+3] = 't';
            stateinout.input[line_curr][i*10+4] = 'y';
          }else{
            for(int j=0;j<stateinout.fiber_sch.answer_length[turn];j++){
              stateinout.input[line_curr][i*10+j] = stateinout.fiber_sch.answers[turn][j];
            }
          }
        }
        stateinout.input[line_curr][50] = '(';
        stateinout.input[line_curr][51] = ' ';
        stateinout.input[line_curr][52] = 'l';
        stateinout.input[line_curr][53] = 'a';
        stateinout.input[line_curr][54] = 's';
        stateinout.input[line_curr][55] = 't';
        stateinout.input[line_curr][56] = ' ';
        stateinout.input[line_curr][57] = 't';
        stateinout.input[line_curr][58] = 'a';
        stateinout.input[line_curr][59] = 's';
        stateinout.input[line_curr][60] = 'k';
        stateinout.input[line_curr][61] = ' ';
        stateinout.input[line_curr][62] = 'l';
        stateinout.input[line_curr][63] = 'a';
        stateinout.input[line_curr][64] = 's';
        stateinout.input[line_curr][65] = 't';
        stateinout.input[line_curr][66] = ' ';
        stateinout.input[line_curr][67] = ')';
        stateinout.result_set = true;
        stateinout.command_set = false;
        stateinout.command_invalid = false;
        stateinout.command_length = 10;
        stateinout.enter_pressed = false;
        stateinout.input_length[line_curr] = 68;  
      }

    }else if(stateinout.command_invalid){
      cpystr("command not found. for avialable commands type 'help'",stateinout.input[line_curr],53);
      stateinout.input_length[line_curr] = 53;
      stateinout.result_set = true;
      stateinout.command_set = false;
      stateinout.command_invalid = false;
      stateinout.command_length = 10;
      stateinout.enter_pressed = false;
    }else{
      stateinout.input[line_curr][0] = 'u';
      stateinout.input[line_curr][1] = 's';
      stateinout.input[line_curr][2] = 'e';
      stateinout.input[line_curr][3] = 'r';
      stateinout.input[line_curr][4] = '@';
      stateinout.input[line_curr][5] = 'h';
      stateinout.input[line_curr][6] = 'o';
      stateinout.input[line_curr][7] = 'h';
      stateinout.input[line_curr][8] = '$';
      stateinout.input[line_curr][9] = ' ';
      stateinout.command_length = 10;
      stateinout.input_length[line_curr] = 10;
      stateinout.command_set = false;
      stateinout.command_invalid = false;
      stateinout.command_length = 10;
      stateinout.enter_pressed = false;

    }
  }
  
}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){
  render.num_key_pressed = shell.num_key_pressed;
  int line_num = shell.line_num;
  render.input_length[line_num] = shell.input_length[line_num];
  for(int i=0;i<shell.input_length[line_num];i++){
    render.input[line_num][i] = shell.input[line_num][i];
  }
  render.line_num = shell.line_num;
  render.command_set = shell.command_set;
  render.command_invalid = shell.command_invalid;
  if(shell.command_set){
    if(!shell.command_invalid){
      render.command = shell.command;
    }
  }
  render.result_set = shell.result_set;
  render.screen_roll = shell.screen_roll;
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
  if(a.num_key_pressed!=b.num_key_pressed){
    return false;
  }else if(a.result_set!=b.result_set){
    return false;
  }else{
    return true;}
}


static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){
  // welcome message
  drawrect(0,0,80,3,4,0,w,h,vgatext_base);
  drawtext(1,1,"                hash v0.0.1. For available commands type help.                ",78,3,0,w,h,vgatext_base);
  int line_num = state.line_num;

  // key count bar
  char text[24];
  cpystr("Number of keys pressed: ",text,24);
  int text_size = 24;
  drawrect(45,22,80,25,6,0,w,h,vgatext_base);
  drawtext(47,23,text,text_size,2,0,w,h,vgatext_base);
  drawnumberinhex(71,23,state.num_key_pressed,8,2,0,w,h,vgatext_base);


  // show the user input/result
  if(!state.screen_roll){
    // clear screen
    fillrect(state.input_length[line_num],line_num+3,80,line_num+4,0,0,w,h,vgatext_base);
    //user input
    drawtext(0,state.line_num+3,state.input[line_num],state.input_length[line_num],2,0,w,h,vgatext_base);
    if(!state.result_set){
      drawtext(0,state.line_num+3,state.input[line_num],10,1,0,w,h,vgatext_base);
    }
  }else{
    int line_curr = line_num;
    // roll over the screen
    for(int i=18;i>=0;i--){
      // clear screen
      fillrect(0,i+3,80,i+4,0,0,w,h,vgatext_base);
      //user input
      drawtext(0,i+3,state.input[line_curr],state.input_length[line_curr],2,0,w,h,vgatext_base);
      if(state.input[line_curr][4] == '@' && state.input_length[line_curr]>=10){
        drawtext(0,i+3,state.input[line_curr],10,1,0,w,h,vgatext_base);
      }     
      line_curr -= 1;
      if(line_curr < 0){
        line_curr = 18;
      } 
    }
  }

  // command set display
  char cmd[13];
  drawrect(0,22,25,25,6,0,w,h,vgatext_base);
  cpystr("Command set: ",cmd,13);
  int cmd_size = 13;
  drawtext(2,23,cmd,cmd_size,2,0,w,h,vgatext_base);


  // show command set
  if(state.command_set){
    if(!state.command_invalid){
      if(state.command==0){
        drawtext(cmd_size+2,23,"help     ",9,2,0,w,h,vgatext_base);
      }else if(state.command==1){
        drawtext(cmd_size+2,23,"echo     ",9,2,0,w,h,vgatext_base);
      }else if(state.command==2){
        drawtext(cmd_size+2,23,"factorial",9,2,0,w,h,vgatext_base);
      }else if(state.command==3){
        drawtext(cmd_size+2,23,"fibonacci",9,2,0,w,h,vgatext_base);
      }else if(state.command==4){
        drawtext(cmd_size+2,23,"numprimes",9,2,0,w,h,vgatext_base);
      }else if(state.command==5){
        drawtext(cmd_size+2,23,"numprimes",9,2,0,w,h,vgatext_base);
      }else if(state.command==6){
        drawtext(cmd_size+2,23,"numprimes",9,2,0,w,h,vgatext_base);
      }else if(state.command==7){
        drawtext(cmd_size+2,23,"numprimes",9,2,0,w,h,vgatext_base);
      }else if(state.command==8){
        drawtext(cmd_size+2,23,"lar prime",9,2,0,w,h,vgatext_base);
      }else if(state.command==9){
        drawtext(cmd_size+2,23,"sum*prime",9,2,0,w,h,vgatext_base);
      }else if(state.command==10){
        drawtext(cmd_size+2,23,"resultsch",9,2,0,w,h,vgatext_base);
      }
    }else{
      drawtext(cmd_size+2,23,"invalid  ",9,2,0,w,h,vgatext_base);
    }
  }else{
    fillrect(14,23,23,23,0,0,w,h,vgatext_base);
  }
}


//
//
// helper functions
//
//

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  vgatext::writechar(y*w+x,c,bg,fg,vgatext_base);
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int y=y0;y<y1;y++){
    for(int x=x0;x<x1;x++){
      writecharxy(x,y,0,bg,fg,w,h,vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){

  writecharxy(x0,  y0,  0xc9, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y0,  0xbb, bg,fg, w,h,vgatext_base);
  writecharxy(x0,  y1-1,0xc8, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y1-1,0xbc, bg,fg, w,h,vgatext_base);

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y0, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y1-1, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x0,y, 0xba, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x1-1,y, 0xba, bg,fg, w,h,vgatext_base);
  }
}

static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
    if(!str[i]){
      break;
    }
  }
}

static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%16);
    number=number/16;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

// function to convert string to numbers
int stoi(char* str,int length){
  int num = 0;
  int i=0;
  while(i<length){
    int temp = ((int)str[i]) - 48;
    if(temp>=0 && temp<=9){
      num *= 10;
      num += temp;
    }else{
      break;
    }
    i++;
  }
  return num;
}

// function to copy one string to another (char *)
void cpystr(char* from,char* to,int length){
  for(int i=0;i<length;i++){
    to[i] = from[i];
  }
}

