void setup() {
  Serial.begin(9600)

}

void enviarEstadoLivre(){
  enviarMensagem("Livre");
}
void enviarEstadoTrabalhando(){
  enviarMensagem("Trabalhando");
}
void enviarEstadoTerminado(){
  enviarMensagem("Terminado");
}
void enviarMensagem(String mensagem){
  serial.write(mensagem);
}
String esperarMensagemDoMestre(){//string
  if (Serial.available() > 0) { 
    String str = Serial.readString(); 
    //teste
    return str;
  }
}
String criptografiaHomophonic(){
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

void loop() {
  // if(esperarMensagemDoMestre()== "Começar"){pararGarrafa()tamparGarrafa()liberarGarrafa()}


}
