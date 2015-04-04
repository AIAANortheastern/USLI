void encoder_cb(unsigned long diff) {
  static int val1 = 0, val2 = 0;
  static int oldVal1 = 0, oldVal2 = 0;
  static int pos = 0, oldPos = 0;
  static int turn = 0, oldTurn = 0;
  static boolean dir = false;  

  // read it values from encoder
  val1 = digitalRead(ENCR_BELT_LINEAR_A_PIN);
  val2 = digitalRead(ENCR_BELT_LINEAR_B_PIN);

  // Detect changes
  if ( val1 != oldVal1 || val2 != oldVal2) {

    // decode graycode to position mod 4
    if ( val1 == 1 && val2 == 1 )
      pos = 0;
    else if ( val1 == 0 && val2 == 1 )
      pos = 1;
    else if ( val1 == 0 && val2 == 0 )
      pos = 2;
    else if ( val1 == 1 && val2 == 0 )
      pos = 3;

    // difference from previous position mod 4
    turn = pos-oldPos;

    if (abs(turn) == 2) { // undetermined motion, guess from previous motion
      encoder_position += (dir) ? 2 : -2;
      encoder_errors++;
    } 
    else {
      if (turn == -1 || turn == 3) {       // turning forward
        encoder_position++;
        dir = true;
      }      
      else if (turn == 1 || turn == -3) {       // turning backard
        encoder_position--;
        dir = false;
      }
    }

    // latch previous values
    oldVal1 = val1;
    oldVal2 = val2;
    oldPos  = pos;
    oldTurn = turn;
  }
}







