//Código para cálculo de velocidade a partir de 
//um tubo de pitot e um sensor MPXV7002DP para pressão diferencial
#include <SD.h>

File velocidade;

float V_0 = 5.0; // provide voltagem para o sensor de pressão
float rho = 1.204; // densidade do ar

// parâmetros para cálculo da média e deslocamento
int offset = 0;
int offset_size = 10;
int veloc_mean_size = 20;
int zero_span = 2;

// define e calcula deslocamento
void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  velocidade = SD.open("velocidade.txt", FILE_WRITE);

  for (int ii=0;ii<offset_size;ii++){
    offset += analogRead(A0)-(1023/2);
  }
  offset /= offset_size;
}

void loop() {
  float adc_avg = 0; float veloc = 0.0;
  
// media algumas leituras CAD para estabilidade
  for (int ii=0;ii<veloc_mean_size;ii++){
    adc_avg+= analogRead(A0)-offset;
  }
  adc_avg/=veloc_mean_size;
  
  // certifica que se o CAD lê abaixo de 512, então a igualamos a uma velocidade negativa 
  if (adc_avg>512-zero_span and adc_avg<512+zero_span){
  } else{
    if (adc_avg<512){
      veloc = -sqrt((-10000.0*((adc_avg/1023.0)-0.5))/rho);
    } else{
      veloc = sqrt((10000.0*((adc_avg/1023.0)-0.5))/rho);
    }
  }
  Serial.println(veloc); // exibe velocidade 
  // if the file opened okay, write to it:
  if (velocidade) {
    velocidade.println(veloc);
	// close the file:
    velocidade.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening velocidade.txt");
  }
  delay(10); // delay para estabilidade
}
