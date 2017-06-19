//Made By TSDrake

int Simon[100];
int Round=0;
int reset=1;
int posic;
int valor=0;
int test=0;
int LTurn=0;
float dificultad=1;

#define Rojo 2
#define Verde 3
#define Azul 4
#define Amarillo 5

#include <Servo.h>
Servo ServoTurn;
Servo ServoHombro;
Servo ServoCodo;

void setup() {
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);           //Configuramos los botones
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);      //Los ponemos con resistencia en pull up
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);         //Ahora las matrices
  ServoTurn.attach(A0);
  ServoHombro.attach(A1);
  ServoCodo.attach(A2);       //Y los servos del Little Arm
  randomSeed(analogRead(A5)); //Utilizamos una seed aleatoria para las probabilidades

  ServoTurn.write(40);
  delay(30);
  ServoHombro.write(115);
  ServoCodo.write(180);       //Ponemos el brazo en la posicion inicial
  for(int l=0;l<3;l++){
    digitalWrite(11,HIGH);
    delay(300);
    digitalWrite(9,HIGH);
    delay(300);
    digitalWrite(8,HIGH);
    delay(300);
    digitalWrite(10,HIGH);
    delay(300);
    digitalWrite(11,LOW);
    delay(300);
    digitalWrite(9,LOW);
    delay(300);
    digitalWrite(8,LOW);
    delay(300);
    digitalWrite(10,LOW);
    delay(300);
  }                              //Y hacemos una pequeña introduccion al juego iluminando y apagando las matrices
}

void loop() {
  if (reset==1){             //Aqui pregunta si hay que resetear y coje toda la memoria y la escribe a 0s
    resetF();
    }
  turnoPlayer();             //Ejecutamos el turno del jugador
  if(reset==0){              //Si no hemos fallado ejecuta el turno del brazo
    turnoBrazo();
  }
}

void turnoPlayer (){
  test=0;
  while(test<Round){             //Lo primero es evaluar un array con las pulsaciones anteriores
    while(valor==0){             //Atasca el programa para que espere a que pulsemos algun boton
      while(digitalRead(Rojo)==0){
        valor=1;
        digitalWrite(8,HIGH);
        tone(6,2349,100);
        delay(100);
        }
      while(digitalRead(Verde)==0){
        valor=2;
        digitalWrite(9,HIGH);
        tone(6,2637,100);
        delay(100);
        }
      while(digitalRead(Azul)==0){
        valor=3;
        digitalWrite(10,HIGH);
        tone(6,2793,100);
        delay(100);
        }
      while(digitalRead(Amarillo)==0){
        valor=4;
        digitalWrite(11,HIGH);
        tone(6,3135,100);
        delay(100);
        }
      delay(100);
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      }
    if (valor==Simon[test]){         //Si nuestra pulsacion es correcta nos manda a pulsar la siguiente hasta que no queden mas
      test++;
      valor=0;
      }
    if (valor!=0){                  //Si nos equivocamos hace una animacion de derrota y pide un reseteo
      if (valor!=Simon[test]){
       FALLO();
       test=200;
       }
      }
    }
  if(reset==0){                    //Si no nos hemos equivocado en ninguna pulsacion nos pide un boton mas para escribirlo en la memoria
    valor=0;
    while(valor==0){
      while(digitalRead(Rojo)==0){
        valor=1;
        digitalWrite(8,HIGH);
        tone(6,2349,100);
        delay(100);
        }
      while(digitalRead(Verde)==0){
        valor=2;
        digitalWrite(9,HIGH);
        tone(6,2637,100);
        delay(100);
        }
      while(digitalRead(Azul)==0){
        valor=3;
        digitalWrite(10,HIGH);
        tone(6,2793,100);
        delay(100);
        }
      while(digitalRead(Amarillo)==0){
        valor=4;
        digitalWrite(11,HIGH);
        tone(6,3135,100);
        delay(100);
        }
      delay(100);
      }
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    Simon[Round]=valor;
    valor=0;
    Round++;
  }
}

void resetF (){
  for (int h=0;h<101;h++){     //Resetea la memoria
    Simon[h]=0;
    }
  Round=0;
  reset=0;
  test=0;
  LTurn=0;
}

void FALLO (){                      //Animacion de fallo
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
  delay(1000);
  tone(6,270,100);
  delay(180);
  tone(6,270,100);
  delay(180);
  tone(6,270,100);
  delay(180);
  tone(6,270,100);
  delay(1460);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  reset=1;
  dificultad=dificultad+0.3;    //Disminuye la dificultad
}

void turnoBrazo (){
  for(int c=1;c<=Round;c++){
    int prob=random(0,1001);           //Genera la probabilidad de acierto
    float probFallo= dificultad*c*10.0;    //Calcula la probabilidad de fallo
    if(prob>=probFallo){              //Lo compara con la ronda para aumentar su fallo poco a poco
      Movimientos(Simon[c-1]);
    }
    else{                             //Si falla mueve el brazo a una posicion distinta de donde deberia ir
      Movimientos(Simon[c-1]+1);
      reset=1;
      c=100;
      delay(500);
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
    }
  }
  if(reset==0){                          //Si ha acertado todas escribe una mas en la memoria
    int NewArm=random(1,5);
    Movimientos(NewArm);
    Simon[Round]=NewArm;
    Round++;
    delay(500);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    ServoTurn.write(40);
    ServoHombro.write(45);
    ServoCodo.write(135);
  }
  else{                                //Si falla ejecuta una animacion
    delay(1000);
    ServoTurn.write(40);
    ServoHombro.write(115);
    ServoCodo.write(120);
    delay(1000);
    for(int i=120;i<181;i++){         //Hace una reverencia como simbolo de su fallo
      ServoCodo.write(i);
     delay(15);
    }
    for(int i=115;i>34;i--){
      ServoHombro.write(i);
      delay(15);
    }
    delay(2300);
    dificultad=dificultad-0.2;       //Aumenta la dificultad
    if(dificultad<=0){
      dificultad=0.1;
    }
  }
  delay(500);                         //Vuelve a la posicion inicial
  ServoTurn.write(40);
  delay(30);
  ServoHombro.write(115);
  ServoCodo.write(180);
}

void Movimientos(int mov){      //Ejecuta el movimiento asignado con una posicion intermedia para elevar el brazo
  if(mov==5){
    mov=1;
  }
  delay(500);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  ServoTurn.write(40);
  ServoHombro.write(45);
  ServoCodo.write(135);
  delay(400);
  if(mov==1){ //Rojo
    ServoTurn.write(50);
    delay(30);
    ServoHombro.write(5);
    ServoCodo.write(100);
    delay(100);
    digitalWrite(8,HIGH);
    tone(6,2349,200);
  }
  if(mov==2){  //Verde
    ServoTurn.write(60);
    delay(30);
    ServoHombro.write(30);
    ServoCodo.write(165);
    delay(100);
    digitalWrite(9,HIGH);
    tone(6,2637,200);
  }
  if(mov==3){  //Azul
    ServoTurn.write(20);
    delay(30);
    ServoHombro.write(5);
    ServoCodo.write(100);
    delay(100);
    digitalWrite(10,HIGH);
    tone(6,2793,200);
  }
  if(mov==4){  //Amarillo
    ServoTurn.write(15);
    delay(30);
    ServoHombro.write(30);
    ServoCodo.write(165);
    delay(100);
    digitalWrite(11,HIGH);
    tone(6,3135,200);
  }
}
