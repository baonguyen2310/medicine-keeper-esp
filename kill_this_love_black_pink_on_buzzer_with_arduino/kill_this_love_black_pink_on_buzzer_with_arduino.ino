//Code writen by vishal soni 
//youtube :- http://youtube.com/vishalsoniindia
// On 10 july 2021 
// realy took 24 hour to write this
//

// Define pin A0 for buzzer, you can use any other digital pins (Pin 0-13)
const int buzzer = D1;

// Change to 0.5 for a slower version of the song, 1.25 for a faster version
const float songSpeed = 1.0;

// Defining frequency of each music note
#define NOTE_D3 147
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_D_black_5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A_black_5 830
#define NOTE_A5 880
#define NOTE_B5 988
#define NOTE_D6 1175

#define rest 10

// Music notes of the song, 0 is a rest/pulse
int notes[] = {
    NOTE_D3,0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,NOTE_D_black_5,0,
    NOTE_A5,0,NOTE_D5,0,NOTE_D_black_5,0,NOTE_A5,0,
   
    NOTE_D3,0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    NOTE_D5,0,NOTE_D5,0,NOTE_A5,0,NOTE_A_black_5,0,
    NOTE_D5,0,NOTE_A5,0,NOTE_A_black_5,0,NOTE_D5,0,

    NOTE_D3,0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,NOTE_D_black_5,0,
    NOTE_A5,0,NOTE_D5,0,NOTE_D_black_5,0,NOTE_A5,0,
   
    NOTE_D3,0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    0,NOTE_D5,0,NOTE_D5,0,NOTE_D5,0,
    NOTE_D5,0,NOTE_D5,0,NOTE_A5,0,NOTE_A_black_5,0,
    NOTE_D5,0,NOTE_A5,0,NOTE_A_black_5,0,NOTE_D5,

    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_E5,0,NOTE_E5,0,NOTE_D5,0,

    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_E5,0,NOTE_E5,0,NOTE_D5,0,

    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D6,0,

    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D6,0,

    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D6,0,
    
    NOTE_D5,0,NOTE_E5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_E5,0,NOTE_E5,0,NOTE_D5,0,

    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_E5,0,NOTE_E5,0,NOTE_E5,0,NOTE_D5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_D5,0,NOTE_F5,0,

    NOTE_D5,0,NOTE_D5,0,NOTE_E5,0,NOTE_F5,0,
    NOTE_D5,0,NOTE_E5,0,NOTE_F5,0,
    NOTE_D5,0,

    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,

    NOTE_A5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,
    NOTE_A5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,NOTE_F5,0,

    NOTE_A5,0,NOTE_F5,0,NOTE_F5,0,NOTE_D5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D5,0,

    NOTE_E5,0,NOTE_E5,0,NOTE_E5,0,
    NOTE_F5,0,NOTE_F5,0,NOTE_D5,0,
    NOTE_G5,0,NOTE_G5,0,
    NOTE_A5,0,
    NOTE_F5,0,

//    NOTE_D5,0,NOTE_F5,0,NOTE_F5,0,

    
};
// Durations (in ms) of each music note of the song
// Quarter Note is 250 ms when songSpeed = 1.0
int durations[] = {
    200,70,100,80,100,80,400,rest,
    200,100,80,100,80,400,70,
    200,rest,125,rest,175,rest,175,rest,
    175,rest,175,rest,175,rest,50,5,

    200,70,100,80,100,80,400,rest,
    200,100,80,100,80,450,70,
    200,rest,125,rest,175,rest,175,rest,
    175,rest,175,rest,175,rest,175,5,

    200,70,100,80,100,80,400,rest,
    200,100,80,100,80,400,70,
    200,rest,125,rest,175,rest,175,rest,
    175,rest,175,rest,175,rest,50,5,

    200,70,100,80,100,80,400,rest,
    200,100,80,100,80,450,70,
    200,rest,125,rest,175,rest,175,rest,
    175,rest,175,rest,175,rest,175,

    375,rest,150,rest,375,rest,125,50,
    450,rest,375,rest,125,rest,375,rest,125,rest,
    375,rest,125,rest,450,50,

    375,rest,125,rest,375,rest,125,50,
    450,rest,375,rest,125,rest,375,rest,125,rest,
    375,rest,125,rest,450,50,

    375,rest,125,rest,375,rest,125,50,
    375,rest,125,rest,450,rest,

    375,rest,125,rest,375,rest,125,50,
    375,rest,125,rest,450,rest,

    375,rest,125,rest,375,rest,125,50,
    375,rest,125,rest,450,rest,

    375,rest,125,rest,375,rest,125,rest,
    375,rest,125,rest,450,100,

    150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,
    375,150,150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,375,150,
    150,rest,150,rest,150,rest,150,rest,150,rest,
    150,rest,450,70,

    150,rest,150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,
    450,rest,

    150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,

    450,rest,
    150,rest,150,rest,150,rest,
    450,rest,
    150,rest,150,rest,150,rest,150,rest,

    450,rest,150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,

    150,rest,150,rest,150,rest,
    150,rest,150,rest,150,rest,
    150,rest,150,rest,
    150,rest,
    150,rest,

//    375,rest,100,rest,100,rest,
    
    
};
void setup()
{
  const int totalNotes = sizeof(notes) / sizeof(int);
  // Loop through each note
  for (int i = 0; i < totalNotes; i++)
  {
    const int currentNote = notes[i];
    float wait = durations[i] / songSpeed;
    // Play tone if currentNote is not 0 frequency, otherwise pause (noTone)
    if (currentNote != 0)
    {
      tone(buzzer, notes[i], wait); // tone(pin, frequency, duration)
    }
    else
    {
      noTone(buzzer);
    }
    // delay is used to wait for tone to finish playing before moving to next loop
    delay(wait);
  }
}

void loop()
{
  // Nothing in loop. Music only plays once.
  // You can click reset on Arduino to replay the song.
}
