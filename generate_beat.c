#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double last_val = 0;
double r2(){
    double random_value;
    srand ( time ( NULL) * last_val);
    random_value = (double)rand()/RAND_MAX; //float in range 0 to 1
    random_value * 2 - 1;
    last_val = random_value;
    return random_value;
}

	double amp_array[2000000];

int main() {

	int i; 

	int beat_per_measure = 16;
	double bpm = 140*beat_per_measure;
	int sample_rate = 44100;
	int seconds = 30;

	int beat;
	int since_last_beat;

	//Silence
	for(i=0;i<(sample_rate*seconds);i++){
		amp_array[i] = 0.0;
	}

	//Rising tone
	beat = 0;
	since_last_beat = 0;
	double rising_iter = 0;
	for(i=0;i<(sample_rate*seconds);i++){

		if(sin(i*rising_iter)>1)break;
 		amp_array[i] += sin(i*rising_iter);
 		rising_iter += 0.0000001 * r2();


   	since_last_beat++;
   	if((since_last_beat/sample_rate)>(60/bpm)){ 
   		since_last_beat = 0;
   	}
   	if(since_last_beat==0){
   		beat += 1;
   	}
   	if(beat>(beat_per_measure)){
   		beat = 0;
   	}
   	if(beat==0){
   		rising_iter = 0;
   	}
	}

	//Hi tats
	beat = 0;
	since_last_beat = 0;

	for(i=0;i<(sample_rate*seconds);i++){

   	since_last_beat++;
   	if((since_last_beat/sample_rate)>(60/bpm)){ 
   		since_last_beat = 0;
   	}
   	if(since_last_beat==0){
   		beat += 1;
   	}
   	if(beat>(beat_per_measure)){
   		beat = 0;
   	}
   	//sprintf("%d\r\n",beat);
   	if((beat%4)==0){
   		amp_array[i] += r2();
   	}
	}

	//Clipping
	for(i=0;i<(sample_rate*seconds);i++){
		//if((amp_array[i] > 1)||(amp_array[i] > 1))amp_array[i] = 0.0;
	}

	//Writing to file
	FILE *fp;
	fp = fopen("music.dat", "w+");
	fputs("; Sample Rate 44100\n", fp); //TODO have sample rate be dynamic here
	fputs("; Channels 1\n", fp);
	char amp_string[300];
	for(i=0;i<(sample_rate*seconds);i++){
 		sprintf(amp_string,"%f %f \n",(double)(i/sample_rate),amp_array[i]);
 		fputs(amp_string, fp);
   }
   fclose(fp);

   //Converting to wav
   system("sox music.dat music.wav");

   return 1;
}