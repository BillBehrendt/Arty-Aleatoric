/*
+-------------------------------------------------+
| Arty The Aleatoric Arduino: random gaze 0       |
| Part I of the Arty series                       |
| Perambulations in the Field                     |
|                   of Artifical Music Generation |
| Brought to you by "Junkbox Junkie, the Musical" |
+-------------------------------------------------+
Copyright © 2015 by Bill L. Behrendt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdarg.h>
#include <stdio.h>
#include <SoftwareSerial.h>

//software serial port pins for MIDI
#define MIDI_OUT 11
#define MIDI_IN 12 //unused

//MIDI cmd
#define CMD_NOTE_ON 0x90 //channel 1
#define CMD_NOTE_OFF 0x80 //channel 1

//global variables
SoftwareSerial MIDI(MIDI_IN, MIDI_OUT); // RX, TX
byte noteVal;

void flashWorkLED(char start) {
  if (start == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
  for (char tm = 0; tm < 5; tm++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);
    delay(75);
  }
}

void sendMIDI(unsigned char cmd, unsigned char data1, unsigned char data2)
{
  MIDI.write(cmd);  //note on or note off
  MIDI.write(data1); //MIDI note 0-127
  MIDI.write(data2); //velocity (loudness) of note
}

void audioBootTest(void) {
  sendMIDI(CMD_NOTE_ON,36,64);
  sendMIDI(CMD_NOTE_ON,60,64);
  sendMIDI(CMD_NOTE_ON,64,64);
  sendMIDI(CMD_NOTE_ON,67,64);
  sendMIDI(CMD_NOTE_ON,72,64);
  flashWorkLED(1);
  delay(1000);
  flashWorkLED(0);
  sendMIDI(CMD_NOTE_OFF,36,64);
  sendMIDI(CMD_NOTE_OFF,60,64);
  sendMIDI(CMD_NOTE_OFF,64,64);
  sendMIDI(CMD_NOTE_OFF,67,64);
  sendMIDI(CMD_NOTE_OFF,72,64);  
}

void allNotesOff(void) {
   for (int anoff = 0; anoff < 16; anoff++) {
    sendMIDI(0xB0 + anoff, 0x7B, 0x00);
  } 
}

void setup() {
  Serial.begin(115200); // for looking at debug things
  MIDI.begin(31250); // our software serial MIDI connection
  //turn all notes off, all channels
  allNotesOff();
  audioBootTest();
  //get a random seed from an unattached analog pin
  randomSeed(analogRead(0));
}

void loop() {
  //trying out the Arduino pseudo-random nature
  noteVal=random(30,72); //change range to limit spread of notes do things like random(30,72)%random(60,96)
  sendMIDI(CMD_NOTE_ON,noteVal,32+random(64)); // or use random(32,96)
  //delay(130+random(130));  //a tempo of sorts or use random(130,260)
  delay(random(130,260)); //another way to limit the tempo variation
  sendMIDI(CMD_NOTE_OFF,noteVal,64);
}
