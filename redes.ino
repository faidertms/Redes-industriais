void setup() {
  Serial.begin(9600);
 }
unsigned long tempo;
char alfa[] = "abcdefghijklmnopqrstuvwxyz";
char cripts[][27] = {{'8','F','H','G','3','l','1','L','E','I','w','o','M','X','6','Q','P','b','V','9','a','Z','S','D','j','r'},{'z','-','k','m','x','n','B','u','0','-','-','O','A','v','5','p','-','R','y','f','4','g','-','-','-','-'},{'K','-','s','N','q','-','-','-','J','-','-','a','c','-','2','-','-','T','e','Y','h','-','-','-','-','-'},{'7','-','-','-','t','-','-','-','-','-','-','-','-','-','W','-','-','C','-','-','-','-','-','-','-','-'},{'i','-','-','-','-','-','-','-','-','-','-','-','-','-','d','-','-','-','-','-','-','-','-','-','-','-','-'}};  
String mensagemNova;
bool recebeu = false;

void enviarEstadoLivre(){
  enviarMensagem("Livre");
}
void enviarEstadoTrabalhando(){
  enviarMensagem("Trabalhando");
}
void enviarEstadoTerminado(){
  enviarMensagem("Terminado");
}
void enviarEstadoChegou(){
  enviarMensagem("Chegou");
}
void enviarMensagem(String mensagem){
  Serial.write(cripto(mensagem).c_str());
}
void esperarMensagemDoMestre(){//string
  if(Serial.available()>0) {  
    mensagemNova += descripto(String((char)Serial.read()));
    Serial.println(mensagemNova);
    recebeu = true;
    tempo = millis();
  }
}

void pararGarrafa(){
  enviarEstadoTrabalhando();
}
void tamparGarrafa(){
  
  //ações
  enviarEstadoTerminado();
}
void liberarGarrafa(){
  esperarMensagemDoMestre();
  //libera garrafa

  enviarEstadoLivre();
}

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



void loop() {
  esperarMensagemDoMestre();
  if(strcmp(mensagemNova.c_str(),"comecar") == 0){
    mensagemNova=""; // momentaneo
    enviarMensagem("teste");
    delay(1500);
  }else if(strcmp(mensagemNova.c_str(),"liberar") == 0){
    mensagemNova=""; // momentaneo
    enviarMensagem("thiago");
   delay(1500);
  }
  
  if(recebeu & ((millis()-tempo) == 1000)){
    mensagemNova=""; // momentaneo
  }
}
  // if(esperarMensagemDoMestre()== "Começar"){pararGarrafa()tamparGarrafa()liberarGarrafa()}
