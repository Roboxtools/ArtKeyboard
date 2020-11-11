/*********************************************************************
  This is an example for our nRF52 based Bluefruit LE modules

  Pick one up today in the adafruit shop!

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  MIT license, check LICENSE for more information
  All text above, and the splash screen below must be included in
  any redistribution

  -----Jorge Verdugo------------
  note: this code require to install adafruit nrf52 board libraries v0.9.3 newer versions are not compatible
  code base on hidscan sample from Adafruit
*********************************************************************/

#include <bluefruit.h>
#include <ButtonEvents.h>
BLEHidAdafruit blehid;

// original code (shortcuts that dont change from windows to ipad)
uint8_t pins[]    = {};
uint8_t hidcode[] = {};
uint8_t pincount  = sizeof(pins) / sizeof(pins[0]);

// Modifier keys, only take cares of Shift ATL, CTRL, CMD keys are left for user excersie.
// JV. my code to have double function on the keys sketchy :PV
// pin 16 and A0 fail in some of my adafruit nrf52 if you are getting double keys or a hold press try
// to understand what key is the problem and rewire the pin and change the pin number here

uint8_t b0 = A2;
uint8_t b1 = A0;
uint8_t b2 = 15;
uint8_t b3 = A4;
uint8_t b4 = A1;
uint8_t b5 = 27;
uint8_t b6 = 25;
uint8_t b7 = A5;
uint8_t b8 = 31;
uint8_t b9 = A3;
uint8_t b10 = 30;
uint8_t b11 = 12;

const byte b12 =  7;
const byte b13 = 11;
ButtonEvents mybuttonb12;
ButtonEvents mybuttonb13;
ButtonEvents mybuttonb11;

uint8_t b14 = 13;


//default starting mode
int num = 1;
bool keyPressedPreviously = false;
 bool    eraseKeyPressed = false;


//toggle
int State = 0;
int buttonNew;
int buttonOld = 1;
int State2 = 0;
int buttonNew2;

void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
  Serial.println("ArtKeyboardV10L-KDS");
  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  //name of the keyboard v14c
  Bluefruit.setName("ArtKeyboardV10L-KDS");

  // set up pin as input
  for (uint8_t i = 0; i < pincount; i++)
  {
    pinMode(pins[i], INPUT_PULLUP);
  }

  pinMode(b0,  INPUT_PULLUP);
  pinMode(b1,  INPUT_PULLUP);
  pinMode(b2,  INPUT_PULLUP);
  pinMode(b3,  INPUT_PULLUP);
  pinMode(b4,  INPUT_PULLUP);
  pinMode(b5,  INPUT_PULLUP);
  pinMode(b6,  INPUT_PULLUP);
  pinMode(b7,  INPUT_PULLUP);
  pinMode(b8,  INPUT_PULLUP);
  pinMode(b9,  INPUT_PULLUP);
  pinMode(b10, INPUT_PULLUP);
  pinMode(b11, INPUT_PULLUP);
  pinMode(b12, INPUT_PULLUP);
  pinMode(b13, INPUT_PULLUP);
  mybuttonb12.attach(b12);
  mybuttonb13.attach(b13);
  mybuttonb11.attach(b11);

  pinMode(b14, INPUT_PULLUP);


  /* Start BLE HID
     Note: Apple requires BLE device must have min connection interval >= 20m
     ( The smaller the connection interval the faster we could send data).
     However for HID and MIDI device, Apple could accept min connection interval
     up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
     connection interval to 11.25  ms and 15 ms respectively for best performance.
  */

  blehid.begin();
  // Set callback for set LED from central
  blehid.setKeyboardLedCallback(set_keyboard_led);

  /* Set connection interval (min, max) to your perferred value.
     Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
     min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms
  */
  /* Bluefruit.setConnInterval(9, 12); */

  // Set up and start advertising
  startAdv();
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  // Include BLE HID service
  Bluefruit.Advertising.addService(blehid);
  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();
  /* Start Advertising
     - Enable auto advertising if disconnected
     - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     - Timeout for fast mode is 30 seconds
     - Start(timeout) with timeout = 0 will advertise forever (until connected)
     For recommended advertising interval
     https://developer.apple.com/library/content/qa/qa1931/_index.html
  */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void loop()
{
  bool    anyKeyPressed = false;
  uint8_t modifier = 0;
  uint8_t modifier2 = 0;
  uint8_t count = 0;
  uint8_t keycode[6] = {0};
  uint8_t ce = HID_KEY_C;           // ipad
  uint8_t alt = HID_KEY_ALT_LEFT;   // win
  int delaybutton = 80;



  //MODE QUICKMENU
  if ( 0 == digitalRead(b0) )
  {
    if (num == 1) {
      Serial.println("QuickMenu");
      blehid.keyboardReport(0, HID_KEY_SPACE);
      blehid.keyRelease();
      anyKeyPressed = true;
      keyPressedPreviously = true;
      delay(delaybutton);
    }
    else {
      Serial.println("Eraser Clip");
      blehid.keyboardReport(0, HID_KEY_E);
      blehid.keyRelease();
      eraseKeyPressed = true;
      delay(delaybutton);
      
    }
  }

  //UNDO
  if ( 0 == digitalRead(b1) )
  {
    if (num == 1) {
      Serial.println("UndoCmdZ");
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI;
      blehid.keyboardReport(modifier2, HID_KEY_Z);
      blehid.keyRelease();
      delay(delaybutton);
    }
    else {
      Serial.println("UndoCmdZClip");
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI;
      blehid.keyboardReport(modifier2, HID_KEY_Z);
      blehid.keyRelease();
      delay(delaybutton);
    }

    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //REDO
  if ( 0 == digitalRead(b2) )
  {
    if (num == 1) {
      Serial.println("RedoCmdShiftZ");
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI + KEYBOARD_MODIFIER_LEFTSHIFT;
      blehid.keyboardReport(modifier2, HID_KEY_Z);
      blehid.keyRelease();
      delay(delaybutton);
    }
    else {
      Serial.println("RedoCmdShiftZClip");
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI + KEYBOARD_MODIFIER_LEFTSHIFT;
      blehid.keyboardReport(modifier2, HID_KEY_Z);
      blehid.keyRelease();
      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //color picker ALT
  if ( 0 == digitalRead(b3) )
  {
    if (num == 1) {
      Serial.println("Altpro");
      modifier2 |= KEYBOARD_MODIFIER_LEFTALT;
      blehid.keyboardReport(modifier2, alt);
      blehid.keyRelease();
      delay(delaybutton);
    }
    else {
      Serial.println("Altclip");
      modifier2 |= KEYBOARD_MODIFIER_LEFTALT;
      blehid.keyboardReport(modifier2, alt);
      blehid.keyRelease();
      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //FULLSCREEN
  if ( 0 == digitalRead(b4) )
  {
    if (num == 1) {
      Serial.println("FullScreen");
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI;
      blehid.keyboardReport(modifier2, HID_KEY_0);
      blehid.keyRelease();
      delay(delaybutton);
    }
    else {
      Serial.println("TabClip");
      blehid.keyboardReport(0, HID_KEY_TAB);
      blehid.keyRelease();
      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }


  //Colors Menu/ previuos frame
  if ( 0 == digitalRead(b5) )
  {
    if (num == 1) {
      Serial.println("Colors");
      blehid.keyboardReport(0, HID_KEY_C);
      blehid.keyRelease();
      delay(delaybutton);
    }
    else {
      //   Serial.println("1pfClip");
      //   blehid.keyboardReport(0, HID_KEY_1);
      Serial.println("C");
      blehid.keyboardReport(0, HID_KEY_C);
      blehid.keyRelease();

      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //QUICKMENU
  if ( 0 == digitalRead(b6) )
  {
    if (num == 1) {
      Serial.println("QuickMenu");
      blehid.keyboardReport(0, HID_KEY_SPACE);
      blehid.keyRelease();
      delay(delaybutton);
    }
    else {
//      Serial.println("ShiftLeftClip");
//      blehid.keyboardReport(0, HID_KEY_SHIFT_LEFT);
//      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //Transform
  if ( 0 == digitalRead(b7) )
  {
    if (num == 1) {
      Serial.println("transform");
      blehid.keyboardReport(0, HID_KEY_V);
      blehid.keyRelease();
      delay(delaybutton);
    }
    else {
      Serial.println("transformClip");
      blehid.keyboardReport(0, HID_KEY_K);
      blehid.keyRelease();

      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //selection S
  if ( 0 == digitalRead(b8) )
  {
    if (num == 1) {
      blehid.keyboardReport(0, HID_KEY_S);
      blehid.keyRelease();
      Serial.println("Selection");
      delay(delaybutton);
      
    }
    else {
      blehid.keyboardReport(0, HID_KEY_M);
      blehid.keyRelease();
      Serial.println("SelectionClip");
      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }


  if( num == 1 )
  {
    buttonNew=digitalRead(b9); 
   if(buttonOld==0 && buttonNew==1){
       if (State==0){     
          blehid.keyboardReport(0, HID_KEY_B);
          blehid.keyRelease();
          Serial.println("Brush");  

          State=1;
          anyKeyPressed = true;
          keyPressedPreviously = true;
          delay(delaybutton); 
          }
       else{
          blehid.keyboardReport(0, HID_KEY_E); 
          blehid.keyRelease();
          Serial.println("Eraser"); 
          State=0;
          anyKeyPressed = true;
          keyPressedPreviously = true;
          delay(delaybutton); 
          }
       };
  buttonOld=buttonNew;

  } else if( 0 == digitalRead(b9) )
  {
  
      buttonNew = digitalRead(b9);
      blehid.keyboardReport(0, HID_KEY_P);
      blehid.keyRelease();
      Serial.println("CSP Pencil / Pen");
      delay(delaybutton);

      eraseKeyPressed = false;
      anyKeyPressed = true;
      keyPressedPreviously = true;
    
    }



  //COLORtoggle and nextsframe
  if ( 0 == digitalRead(b10) )
  {
    if (num == 1) {
      blehid.keyboardReport(0, HID_KEY_X);
      blehid.keyRelease();
      Serial.println("x");
      delay(delaybutton);
    }
    else {
      //   blehid.keyboardReport(0, HID_KEY_2);
      //   Serial.println("2nfClip");
      blehid.keyboardReport(0, HID_KEY_X);
      blehid.keyRelease();
      Serial.println("x");
      delay(delaybutton);
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //action /play
  //KDS - rewiring this to switch CSP/Procreate
//  if ( 0 == digitalRead(b11) )
    mybuttonb11.update();

  {
//Serial.println("Toggle mode button pressed ...");
    if (mybuttonb11.held() == true) {
      ledOn( LED_RED );
      Serial.println("Switch mode to CSP");
      num = 2;
      delay(500);
      ledOff( LED_RED );
    }
    if (mybuttonb11.tapped() == true) {
      Serial.println("Switch mode to Procreate");
      num = 1;
      ledOn( LED_RED );
      delay(100);
      ledOff(LED_RED);
    }


//    if ( num == 1 )
//    {
//      ledOn( LED_RED );
//      Serial.println("Switch mode to CSP");
//      num = 2;
//      delay(200);
//      ledOff( LED_RED );
//
//    } else
//    {
//      Serial.println("Switch mode to Procreate");
//      num = 1;
//      ledOn( LED_RED );
//      delay(50);
//      ledOff(LED_RED);
//
//    }
    delay(delaybutton);
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }

  //increase brush size
  if (num == 1) {
    mybuttonb12.update();
    if (mybuttonb12.tapped() == true) {
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI;
      blehid.keyboardReport(0 ,  HID_KEY_BRACKET_RIGHT);
      Serial.println("InBrS5");
      blehid.keyRelease();
      delay(20);
    }
    if (mybuttonb12.doubleTapped() == true) {
      modifier2 |= KEYBOARD_MODIFIER_LEFTSHIFT;
      blehid.keyboardReport(modifier2 ,  HID_KEY_BRACKET_RIGHT);
      Serial.println("SHIFT-InBrS10");
      blehid.keyRelease();
      delay(20);
    }
    if (mybuttonb12.held() == true) {
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI;
      blehid.keyboardReport(modifier2,  HID_KEY_BRACKET_RIGHT);
      Serial.println("GUI-InBrS1");
      blehid.keyRelease();
      delay(20);
    }
  }
  else {
    if ( 0 == digitalRead(b12) ) {
      if(eraseKeyPressed)
      {
        Serial.println("Reminding CSP that its in eraser mode ...");
        blehid.keyboardReport(0 , HID_KEY_E);        
        blehid.keyRelease();
          
        delay(delaybutton);

      }
      blehid.keyboardReport(0 , HID_KEY_BACKSLASH);
      blehid.keyRelease();
      Serial.println("Increase Brush Clip ");
      delay(delaybutton);
      anyKeyPressed = true;
      keyPressedPreviously = true;

    }
  }

  //decrease brush size
  if (num == 1) {
    mybuttonb13.update();
    if (mybuttonb13.tapped() == true) {
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI;
      blehid.keyboardReport(0 ,  HID_KEY_BRACKET_LEFT);
      Serial.println("deBrS5");
      blehid.keyRelease();
      delay(20);
    }
    if (mybuttonb13.doubleTapped() == true) {
      modifier2 |= KEYBOARD_MODIFIER_LEFTSHIFT;
      blehid.keyboardReport(modifier2 ,  HID_KEY_BRACKET_LEFT);
      Serial.println("SHIFT-deBrS10");
      blehid.keyRelease();
      delay(20);
    }
    if (mybuttonb13.held() == true) {
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI;
      blehid.keyboardReport(modifier2,  HID_KEY_BRACKET_LEFT);
      Serial.println("GUI-deBrS1");
      blehid.keyRelease();
      delay(20);
    }
  }
  else {
    if ( 0 == digitalRead(b13) ) {
      if(eraseKeyPressed)
      {
        Serial.println("Reminding CSP that its in eraser mode ...");
        blehid.keyboardReport(0 , HID_KEY_E);        
        blehid.keyRelease();
        delay(delaybutton);      
      }

      blehid.keyboardReport(0 , HID_KEY_BRACKET_RIGHT);
      blehid.keyRelease();
      Serial.println("Decrease Brush Clip ");
      delay(delaybutton);
      anyKeyPressed = true;
      keyPressedPreviously = true;

    }
  }

  //layers / quick access
  if ( 0 == digitalRead(b14) )
  {
    if (num == 1) {
      blehid.keyboardReport(0, HID_KEY_L);
      blehid.keyRelease();
      Serial.println("Layers");
      delay(delaybutton);
    }
    else {
      //   blehid.keyboardReport(0, HID_KEY_3);
      //   Serial.println("3playClip");
      modifier2 |= KEYBOARD_MODIFIER_LEFTGUI + KEYBOARD_MODIFIER_LEFTSHIFT;
      blehid.keyboardReport(modifier2, HID_KEY_Q);
      blehid.keyRelease();
      delay(delaybutton);
      Serial.println("Quick Access CSP");
    }
    anyKeyPressed = true;
    keyPressedPreviously = true;
  }



  // scan normal key and send report
  for (uint8_t i = 0; i < pincount; i++)
  {
    if ( 0 == digitalRead(pins[i]) )
    {
      // if pin is active (low), add its hid code to key report
      keycode[count++] = hidcode[i];

      // 6 is max keycode per report
      if ( count == 6)
      {
        blehid.keyboardReport(modifier, keycode);


        // reset report
        count = 0;
        memset(keycode, 0, 6);
      }
      Serial.println("button");
      // used later
      anyKeyPressed = true;
      keyPressedPreviously = true;
    }
  }

  // Send any remaining keys (not accumulated up to 6)
  if ( count )
  {
    blehid.keyboardReport(modifier, keycode);
    Serial.println("button");
  }

  // Send All-zero report to indicate there is no keys pressed
  // Most of the time, it is, though we don't need to send zero report
  // every loop(), only a key is pressed in previous loop()
  if ( !anyKeyPressed && keyPressedPreviously )
  {
    keyPressedPreviously = false;
    blehid.keyRelease();
  }

  // Poll interval if you have double clicks increase value default is 50
  delay(10);
}

/**
   Callback invoked when received Set LED from central.
   Must be set previously with setKeyboardLedCallback()

   The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
      Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
*/
void set_keyboard_led(uint8_t led_bitmap)
{
  // light up Red Led if any bits is set
  if ( led_bitmap )
  {
    ledOn( LED_RED );
  }
  else
  {
    ledOff( LED_RED );
  }
}
