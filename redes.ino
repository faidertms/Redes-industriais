
//-------------------------------INICIALIZAÇÃO--------------------------------------------\\
unsigned long tempo;
char alfa[] = "abcdefghijklmnopqrstuvwxyz";
char cripts[][27] = {{'8','F','H','G','3','l','1','L','E','I','w','o','M','X','6','Q','P','b','V','9','a','Z','S','D','j','r'},
                    {'z','-','k','m','x','n','B','u','0','-','-','O','A','v','5','p','-','R','y','f','4','g','-','-','-','-'},
                    {'K','-','s','N','q','-','-','-','J','-','-','-','c','-','2','-','-','T','e','Y','h','-','-','-','-','-'},
                    {'7','-','-','-','t','-','-','-','-','-','-','-','-','-','W','-','-','C','-','-','-','-','-','-','-','-'},
                    {'i','-','-','-','-','-','-','-','-','-','-','-','-','-','d','-','-','-','-','-','-','-','-','-','-','-'}};  
String mensagemNova;
bool recebeu = false;

int relay = 8;
Servo servo1;
Servo servo2;


void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT); // motor 
  servo1.attach(6); // 
  servo2.attach(5); //
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

String descripto(String mensagemCripto ){
  String mensagem;
  for (int x = 0; x < mensagemCripto.length(); x++){
    if(mensagemCripto[x] == ' '){
      mensagem += ' '; 
    }else{
      for (int y= 0 ; y< 26; y++){
        for (int z = 0; z < 5; z++){
          if(strcmp(String(mensagemCripto[x]).c_str(),String(cripts[z][y]).c_str()) == 0){
            mensagem += alfa[y];
            goto achou;
          }
        }
      }
    }achou:;
  }
  return mensagem;
}

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
    // tem que ter um sensor aqui...
    servo1.write(50);//ajustar os graus para segurar a garrafa
    enviarEstadoTrabalhando();
}
void tamparGarrafa(){
  servo2.write(50);// soltar tampa
  delay(1000);//tempo ncessario pra soltar
  servo2.write(0); // fecha
  //delay(5000);
  digitalWrite(relay, HIGH);// começa a tampar
  delay(1000);//tempo para tampar
  digitalWrite(relay, LOW);// desligar o motor que tampa
  enviarEstadoTerminado();
}
void liberarGarrafa(){
  servo1.write(0);//ajustar os graus para liberar a garrafa
  enviarEstadoLivre();
}
//-------------------------------LOOP-PRINCIPAL--------------------------------------------\\

void loop() {
  esperarMensagemDoMestre();
  if(strcmp(mensagemNova.c_str(),"comecar") == 0){//proceso 1
    limparMensagem();
    pararGarrafa();
    tamparGarrafa();
  }else if(strcmp(mensagemNova.c_str(),"liberar") == 0){ // processo 2
    limparMensagem();
    liberarGarrafa();
  }
  if(recebeu & ((millis()-tempo) == 1000)){ // Limpeza caso mensagem venha errado
    limparMensagem(); 
  }
}
