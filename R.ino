
//-------------------------------INICIALIZAÇÃO--------------------------------------------\\

#include <Servo.h>
#define pino_servo_barreira 6 // Porta PWM
Servo barreira;  // Variável Servo (barreira)
int pos_barreira;  // Posição Servo (barreira)
#define pino_servo_tampa 5  // Porta PWM
Servo tampa; // Variável Servo (tampa)
int pos_tampa; // Posição Servo (tampa)
int pino_motor_rosca = 12;
int pino_atuador_avancado = 7;
int pino_atuador_recuado = 9;
int pino_sensor = 11;
int estado_sensor = 0;
// Variaveis de comunicação
unsigned long tempo;
char alfa[] = "abcdefghijklmnopqrstuvwxyz";
char cripts[][27] = {{'8','F','H','G','3','l','1','L','E','I','w','o','M','X','6','Q','P','b','V','9','a','Z','S','D','j','r'},
                    {'z','-','k','m','x','n','B','u','0','-','-','O','A','v','5','p','-','R','y','f','4','g','-','-','-','-'},
                    {'K','-','s','N','q','-','-','-','J','-','-','-','c','-','2','-','-','T','e','Y','h','-','-','-','-','-'},
                    {'7','-','-','-','t','-','-','-','-','-','-','-','-','-','W','-','-','C','-','-','-','-','-','-','-','-'},
                    {'i','-','-','-','-','-','-','-','-','-','-','-','-','-','d','-','-','-','-','-','-','-','-','-','-','-'}};  
String mensagemNova;
bool recebeu = false;

void setup() {
  Serial.begin(9600);
  barreira.attach(pino_servo_barreira);
  tampa.attach(pino_servo_tampa);
  barreira.write(90); // Inicia barreira na posição 90º
  tampa.write(90); // Inicia tampa na posição 90º
  pinMode(pino_sensor, INPUT); // Define o pino do sensor como entrada  
  pinMode(pino_motor_rosca, OUTPUT); // Define os pinos dos servos e do atuador como saida
  pinMode(pino_atuador_avancado, OUTPUT);
  pinMode(pino_atuador_recuado, OUTPUT);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  enviarEstadoChegou();
 }
//-------------------------------MENSAGENS-SERIAL--------------------------------------------\\

void enviarEstadoLivre(){
  enviarMensagem("livre");
}
void enviarEstadoTrabalhando(){
  enviarMensagem("trabalhando");
}
void enviarEstadoTerminado(){
  enviarMensagem("terminou");
}
void enviarEstadoChegou(){
  enviarMensagem("chegou");
}
void enviarMensagem(String mensagem){
  Serial.write(cripto(mensagem).c_str());
}
void esperarMensagemDoMestre(){//string
  if(Serial.available()) {  
    mensagemNova += descripto((char)Serial.read());
    recebeu = true;
    tempo = millis();
  }
}
void limparMensagem(){
    mensagemNova=""; // momentaneo
    recebeu = false;
}

//-------------------------------CRIPTOGRAFIA--------------------------------------------\\

String descripto(char mensagemCripto ){
  String mensagem;
  for (int x = 0; x < 1; x++){
    if(mensagemCripto == ' '){
      mensagem += ' '; 
    }else{
      for (int y= 0 ; y< 26; y++){
        for (int z = 0; z < 5; z++){
          if(strcmp(&mensagemCripto,String(cripts[z][y]).c_str()) == 0){
            mensagem += alfa[y];
            goto achou;
          }
        }
      }
    }achou:;
  }
  return mensagem;
}

String cripto(String mensagem){
  int aux;
  String mensagemCripto;
  for (int z = 0; z <= mensagem.length(); z++){
    for (int x= 0 ; x< 26; x++){
      if(mensagem[z] == alfa[x]){
       aux = random(0,5);
        if(cripts[aux][x] != '-'){
          mensagemCripto += cripts[aux][x];
        }else{
          mensagemCripto += cripts[0][x];
        }
      }
    }
  }
  
  return mensagemCripto;
}
//-------------------------------PROCESSOS--------------------------------------------\\

void pararGarrafa(){
    for(pos_barreira = 90; pos_barreira >= 0; pos_barreira--){ // Fechar barreira 
        barreira.write(pos_barreira); 
    }    
    enviarEstadoChegou();
    //delay(3000);  
}
void tamparGarrafa(){
    enviarEstadoTrabalhando();
    // Acionamento do servo_tampa
     for(pos_tampa = 90; pos_tampa >= 22; pos_tampa--){
      tampa.write(pos_tampa);
     }
      delay(1000);
     for(pos_tampa = 22; pos_tampa <= 90; pos_tampa++){
      tampa.write(pos_tampa);  
     }
    delay(2000);
    //Serial.println("Prender Garrafa");
    delay(2000);
    // Atuador prende a garrafa
    digitalWrite(pino_atuador_recuado, HIGH);
    delay(500);
    digitalWrite(pino_atuador_avancado, LOW);
    delay(2000);
    //Serial.println("Garrafa Presa!");
    delay(2000);    
    // Aciona o motor da rosca 2 vezes
    for (int i = 1; i <= 2; i++){      
      //Serial.println("Garrafa sendo rosqueada");
      delay(1000);
      digitalWrite(pino_motor_rosca, LOW);
      delay(700);
      digitalWrite(pino_motor_rosca, HIGH);
      delay(2000);
    }
    delay(2000);
    enviarEstadoTerminado();
}
void liberarGarrafa(){
    digitalWrite(pino_atuador_avancado, HIGH);
    delay(500);
    digitalWrite(pino_atuador_recuado, LOW);
    delay(1000);  
    // Abrir Barreira
    for(pos_barreira = 0; pos_barreira <= 90; pos_barreira++){
      barreira.write(pos_barreira);  
    }
   enviarEstadoLivre();
}
//-------------------------------LOOP-PRINCIPAL--------------------------------------------\\

void loop() {
  esperarMensagemDoMestre();
  estado_sensor = digitalRead(pino_sensor);
  if (estado_sensor == 0){
      pararGarrafa();
  }
  if(strcmp(mensagemNova.c_str(),"comecar") == 0){//proceso 1
    limparMensagem();
    tamparGarrafa();
  }else if(strcmp(mensagemNova.c_str(),"liberar") == 0){ // processo 2
    limparMensagem();
    liberarGarrafa();
  }
  if(recebeu & ((millis()-tempo) == 1000)){ // Limpeza caso mensagem venha errado
    limparMensagem(); 
  }
}

 /* else{ // qualquer coisa um if abaixo se achar muda uma booleana se n reseta
    //Barreira aberta aguardando garrafa
    Serial.println("Aguardando garrafa");
    delay(500);
    barreira.write(90); // Barreira aberta
    tampa.write(90); // Tampa Fechada
    digitalWrite(pino_atuador_avancado, HIGH); // Atuador Recuado
    digitalWrite(pino_atuador_recuado, LOW); // Atuador Recuado
    digitalWrite(pino_motor_rosca, HIGH); // Motor_rosca desligado
  }
}*/
