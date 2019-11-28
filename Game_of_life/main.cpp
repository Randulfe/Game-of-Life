#include "mbed.h"
#include "N5110.h"
#include "tone.h"

#define PI 3.14159265359 //constant for the music part of the program
 
 
//    VCC,SCE,RST,D/C,MOSI,SCLK,LED
N5110 lcd(PTC9,PTC0,PTC7,PTD2,PTD1,PTC11);//nokia screen connections
BusOut display(p22,p23,p24,p25,p26,p27,p28,p29);//bus connections for the cell counter display
char nextstep[84][48];//array for the next screen after applying to the first screen the rules of life
char step[84][48];//array for the first screen where the rules of life will be applyied
DigitalIn stopbutton(PTB9);//stop button 
DigitalIn resetbutton(PTD0);//reset button
DigitalIn startgame(PTC17);//button for starting the game
AnalogIn brightness(PTC12);//potentiometer connection for controling the nokia LEDs brightness
AnalogIn speed(PTB2);//potentiometer for controling the speed of the game of life

int i;//variable i
int j;//variable j
int cellcounter=0;//variable cell counter set to 0.This variable will count the alive cells in the screen

char neigbours();//variable that counts the number of neighbours a cell has








//HERE I SET ALL THE PINS AND VARIABLES THAT I WILL USE DURING THE MUSIC AND LEDs FEEDBACK
BusOut leds(PTA1,PTA2,PTC2,PTC3);//LEDS bus of the mbed that will be synchronized with the music pattern
Timer noteTimer;  // timer for note duration
AnalogOut aout(PTC10);  // DAC on pin 18 for the music output


void init_array();  // function to initialise sample array
void tone(float frequency,float duration);  // play a tone of set frequency for given duration


int n = 32;  // number of samples for the music pattern
float y[32]; // array to store samples in the music pattern
float BPM = 100.0;  // beats per minute of the song


//LEDs specific pattern that will be synchronized with each note of the song
int leds_values[66]={
    1,2,4,8,4,2,1,
    9,6,9,15,8,12,14,
    15,7,3,1,3,7,
    15,14,12,8,9,6,
   15,9,6,15,9,6,
    15,7,3,1,3,7,15,14,12,
    8,15,7,3,1,3,7,
    15,14,12,8,1,2,4,8,
    0,1,3,7,15,0,
  1,3,7,15};

//note array that will be synchronized with the LEDs pattern and with the note duration. The song is the imperial march from StarWars
float note_array[66] = {
    NOTE_A4,NOTE_A4,NOTE_A4,NOTE_F4,NOTE_C5,NOTE_A4,
    NOTE_F4,NOTE_C5,NOTE_A4,NOTE_E5,NOTE_E5,NOTE_E5,
    NOTE_F5,NOTE_C5,NOTE_GS4,NOTE_D5,NOTE_F4,NOTE_C5,
    NOTE_A4,NOTE_A5,NOTE_A4,NOTE_A4,NOTE_A5,
    NOTE_GS5,NOTE_G5,NOTE_FS5,NOTE_F5,NOTE_FS5,
    NOTE_AS4,NOTE_DS5,NOTE_D5,NOTE_CS5,NOTE_C5,NOTE_B4,
    NOTE_C5,NOTE_F4,NOTE_GS4,NOTE_F4,NOTE_A4,
    NOTE_C5,NOTE_A4,NOTE_C5,NOTE_E5,NOTE_A5,NOTE_A4,
    NOTE_A4,NOTE_A5,NOTE_GS5,NOTE_G5,NOTE_FS5,NOTE_F5,
    NOTE_FS5,NOTE_AS4,NOTE_DS5,NOTE_D5,NOTE_CS5,
    NOTE_C5,NOTE_B4,NOTE_C5,NOTE_F4,NOTE_GS4,NOTE_F4,
    NOTE_C5,NOTE_A4,NOTE_F4,NOTE_C4
    };
    float note_duration[66]={//beats of the song, that is, the duration each note will have
        2,2,2,3,7,2,3,7,1,2,
        2,2,3,7,2,3,7,1,2,3,
        7,2,4,4,8,8,4,4,2,
       4,2,8,8,4,8,2,3,7,
        2,3,8,1,2,3,7,2,4,
        4,8,8,4,4,2,4,4,8,
        8,4,4,2,3,8,2,3,8,1,};
 





//here it comes the function for the counter and checker of the neighbours that will have each cell
//it checks everyone around the cell (i,j), and if the neighbour is alive the number of neighbours is one more (n++)
char neighbours(int i,int j){
   char n=0;
  
        if (lcd.getPixel(i-1,j-1)){
        n++;}
        if (lcd.getPixel(i-1,j)){
        n++;}
        if (lcd.getPixel(i-1,j+1)){
        n++;}
        if (lcd.getPixel(i,j-1)){
        n++;}
        if (lcd.getPixel(i,j+1)){
        n++;}
        if (lcd.getPixel(i+1,j-1)){
        n++;}
        if(lcd.getPixel(i+1,j)){
        n++;}
        if(lcd.getPixel(i+1,j+1)){
        n++;} 
        return n;} //it finally returns the final value of the number of neighbours alive
        


/*this function implemets the display counter of cells.Considering the counter is done in the code, depending on if the number is between some hundreds the
display will show one number or other. Therefore, if the display shows 1 it means that there is between 100 and 200 cells alive. If the display shows 
a 0 but with the decimal point on it means that 1000 cells are alive. The decimal point will indicates therefore that the number is higher or equals than
1000.For instance if the display is 1 with decimal point it means that 1100 cells are alive.If the display shows an E it means that there are more than
2000 cells alive (EXTREME).*/
      void cell_counter(int cellcounter){
                 if(cellcounter>2000){
                 display=0x4F;}
        else if(cellcounter>1900){
                display=0xE7;}
        else if(cellcounter>1800){
                    display=0xFF;}
        else if(cellcounter>1700){
                    display=0x87;}
        else if(cellcounter>1600){
                    display=0xFD;}
        else if(cellcounter>1500){
                    display=0xED;}
       else if(cellcounter>1400){
                    display=0xE6;}
        else if(cellcounter>1300){
                    display=0xCF;}
         else if(cellcounter>1200){
                    display=0xDB;}
         else if(cellcounter>1100){
                   display=0x86;}
          else if(cellcounter>1000){
                    display=0xBF;}
        else if(cellcounter>900){
                     display=0x67;}
        else if(cellcounter>800){
                    display=0x7F;}
        else if(cellcounter>700){
                    display=0x07;}
        else if(cellcounter>600){
                    display=0x7D;}
        else if(cellcounter>500){
                   display=0x6D;}
         else if(cellcounter>400){
                  display=0x66;}
        else if(cellcounter>300){
                display=0x4F; }
        else if(cellcounter>200){
                display=0x5B;}
        else if(cellcounter>100){
                display=0x06;}
        else if(cellcounter>0){
              display=0x3F;}}
                 








//HERE I DEFINE THE MUSIC FUNCTIONS THAT I WILL IMPLEMENT IN THE MAIN CODE
           
  //function for initializing the music array                
   void init_array()
{
    // create LUT - loop through array and calculate sine wave samples
    for (int i = 0; i < n ; i++) {
        y[i] = 0.5 + 0.5*sin(i*2*PI/n);
    }

}

//function for the tone and note duration of the music application.It synchronize both of them.
void tone(float frequency,float duration){
 
    if (frequency > 0) {  // a frequency of 0 indicates no note played so only play a note if frequency is not 0

        float dt = 1.0/(frequency*n) - (1.34e-6 + 1e-6);  // calculate time step - take into account DAC time and wait() offset

        noteTimer.start(); // start timer

        while(noteTimer.read() < duration) { // keep looping while timer less than duration

            for (int i = 0; i < n ; i++) {  // loop through samples and output analog waveform
                aout = y[i];
                wait(dt); // leave appropriate delay for frequency
            }
        }

        noteTimer.stop();  // stop the timer
        noteTimer.reset(); // reset to 0

    } else { // if no note played, have a simple delay
        wait(duration);
    }
}


 




//HERE STARTS THE MAIN FUNCTION WITH ITS CODEa
             
 int main(){

    init_array(); // fill array with sine samples, initialize the music array

    // calculate number of notes in array
    //sizeof() returns number of BYTES in array,
    // so divide by size of a float in BYTES to get number of elements
    int notes = sizeof(note_array)/sizeof(float);//set the notes as an integer
  
      lcd.init();//initialize the nokia screen
      lcd.inverseMode();//set the nokia screen to an inversmode
      //displays a welcome and shows my name in the screen for five seconds
      lcd.printString("Welcome to",5,1);
      lcd.printString("Game of Life",10,2);
      lcd.printString("Enjoy it :)",5,3);
      lcd.drawRect(0,5,84,0.5,1);
      lcd.drawRect(0,33,84,0.5,1);
      lcd.printString("MateoRandulfe",3,5);
      
      lcd.refresh();
      wait(5);
      
      /*Shows in the screen the instructions for starting the game whenever the player wants. Meanwhile he does not press the button
      for starting , the music with the LEDs pattern that I created will be playing and displaying.*/
      
      lcd.clear();
      lcd.printString("Press the",5,1);
      lcd.printString("red button",5,2);
      lcd.printString("for starting",5,3);
      lcd.printString("Game of Life",5,4);
      lcd.refresh();
      
      





 while(startgame==0){//while the star game button is not pressed execute the music and LEDs feedback



            // loop through notes
        for(int i=0; i < notes; i++) {
           
           //whenever the satar button will be pressed, the music will stop and the game will start
            if(startgame==1){
                break;}
                
    //keep going with the music function            
   // equalizer on LEDs
leds=leds_values[i];
            // play note
            tone(note_array[i],60.0/(BPM*note_duration[i]));
            // leave a short pause between notes
          
            wait(60.0/(BPM*note_duration[i]));
        }       
        }
       lcd.normalMode();//set nokia screen to normal mode again
      
   
   
   
   
      while(1){//infinite loop
      
 
 
 
 
 
   srand( time( NULL ) );//this makes the random function to be different each time it is applyied
    
    //this two for loops will be really important along the code because they allow me to check every cell of the screen and to implement to every cell whatever I want
      for(i=0;i<84;i++){
           for(j=0;j<48;j++){
              
             step[i][j]= rand()%2;//implement a random function that can be or 1 or 0 in the "x" cell of the screen
             if(step[i][j]==1){//if the point (i,j) of the step array is equals to one, set that pixel on(alive)
                lcd.setPixel(i,j);}
                 else{//if it is a 0 in the array, set the pixel off(dead)
                lcd.clearPixel(i,j);}
                           }}
     lcd.refresh();//refresh screen
      wait(3);//wait 3 seconds untill the actual game starts in order to be able to check that the random function was implemented properly
     
 
 
 
 
  
  
    /*while the reset button is not pressed,perform the game of life.If it is pressed it will finish the code and therefore,it will go the code flow 
    to the original infinite loop,starting agalin the random seed so reseting the game*/ 
     while (resetbutton==0){
         if(stopbutton==0){
    //while stop button is not pressed,game of life will be performing.If it is pressed it will stop the flow of the game of life and then when it will be realised again it will keep flowing
         
 
 
 
 
 
         //apply to all the cells of the screen:
         for ( i=0;i<84;i++){
            for( j=0;j<48;j++){
               //apply the neighbours counter function and stores each value for each cell during each loop in "n" variable
                int n=neighbours(i,j);
                                //HERE I APPLY THE GAME OF LIFE RULES
                //I set as a one or a 0 in the next array in order not to affect the change flow to the upcoming cell checks.It stores the values obtained from the evaluation of the array step
                //(original one) in the nextstep array as 0s or 1s
                if(n>3){
                    nextstep[i][j]=0;}
                    if(n==3){
                        nextstep[i][j]=1;}
                        if(n==2){
                            if(lcd.getPixel(i,j)){
                                nextstep[i][j]=1;}
                                else{
                                    nextstep[i][j]=0;}}
                                    if(n<2){
                                        nextstep[i][j]=0;}}}
                                        
 
 
 
 
                                        cellcounter=0;//set the counter of cells alive to 0
                                        //aplying the following for each cell of the screen:
                                        for(i=0;i<84;i++){
                                            for(j=0;j<48;j++){
//Here I translate the 0s and 1s of the arrays into pixels on(1s) or off(0s) and if the pixel is on for "x" cell of the screen the cell counter will add one more.
                                                if(nextstep[i][j]==1){
                                                    lcd.setPixel(i,j);
                                                    cellcounter++;}
                                                    else{
                                                        lcd.clearPixel(i,j);}
//I finally, in order to keep the flow of the game of life, change the next array to be the original one so that the array obtained after evaluating and apppying the rules of life to the original
//array  will be now that original array after checking and changing every cell of the original array.
                                                        step[i][j]=nextstep[i][j];
                                                       
                                                        }}
lcd.refresh();






//here, with this wait I implement the velocity of the game flow that will be controller with the potentiometer
wait(speed*2);
//I also set the controller with the potentiometer of the LEDs brightness of the nokia screen 
lcd.setBrightness(brightness*0.5);
//I finally apply the display counter of cells calling the function already explained
cell_counter(cellcounter);
                               }}} 
                              }       