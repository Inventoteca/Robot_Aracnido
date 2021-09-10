#include <Servo.h> 

Servo servo1;
Servo servo2;

int d_izq = A2;
int d_der = A1;

int t1 = 10;
int e1 = 9;
int t2 = 3;
int e2 = 2;
//int t3 = A5;  Futuro tercer sensor ultrasonico
//int e3 = A4;  Futuro tercer sensor ultrasonico

int IN1 = 12;
int IN2 = 13;
int ENA = 11;
int IN3 = 4;
int IN4 = 5;
int ENB = 6;

void setup() 
{
  servo1.attach(7);
  servo2.attach(8);

  pinMode(t1,OUTPUT);
  pinMode(e1,INPUT);
  pinMode(t2,OUTPUT);
  pinMode(e2,INPUT);
  //pinMode(t3,OUTPUT);
  //pinMode(e3,INPUT);

  pinMode(d_izq,INPUT);
  pinMode(d_der,INPUT);

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);
} 
 
void loop() 
{ 
  int aux;
  int ultra1;
  int ultra2;
  int cont = 0;
  boolean izq = digitalRead(d_izq);   //Devuelve false cuando detecta algo
  boolean der = digitalRead(d_der);   //Devuelve false cuando detecta algo

  servo1.write(90);
  servo2.write(70);   // servo que no se utiliza del pan tilt

  ultra1 = ultra(t1,e1);  //Activamos Ultrasonico 1
  delay(20);
  ultra2 = ultra(t2,e2);  //Activamos Ultrasonico 2
  delay(20);
  
  if(ultra2 < 20 || ultra1 < 20)  //Se activa cuando los ultrasonicos detectan objetos a menos de 20cm
  {
    if(izq == LOW || der == LOW)  //Se activa cuando los infrarojos encuentran obstaculos
    {
      motor(-1,180,180);  //Funcion para controlar motores -1 = reversa
      delay(1000);
    }
    else
    {
      aux = sele();
      while(aux < 30 && ultra2 < aux)
      {
        aux = sele();
        delay(500);
      }
    }
  }
  else
  {
    if(izq == LOW)
    {
      while(izq == LOW)           //Se activa cuando el sensor infrarojo izquierdo detecta obstaculo
      {
        motor(1,120,250);         //Potencia mas el segundo motor para que el robot gire hacia la derecha
        izq = digitalRead(d_izq);
        cont++;
        delay(20);
        if(cont > 200)            //Se activa cuando el robot queda travado sin avanzar y retrosede
        {
          motor(-1,150,150);
          delay(500);
        }
      }
    }
    else if(der == LOW)           //Se activa cuando el sensor infrarojo derecho detecta obstaculo
    {
      while(der == LOW)
      {
        motor(1,250,120);         //Potencia mas el primer motor para que el robot gire hacia la izquierda
        der = digitalRead(d_der);
        cont++;
        delay(20);
        if(cont > 200)            //Se activa cuando el robot queda travado sin avanzar y retrosede
        {
          motor(-1,150,150);
          delay(500);
        }
      }
    }
    else
    {
      motor(1,180,180);           //Camina hacia adelante
    }
  }
} 

int ultra(int Trig, int Echo)     //Funcion para controlar Ultrasonicos
{
  long duracion;
  long distancia;
  
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  
  duracion = pulseIn(Echo,HIGH);
  distancia = (duracion/2)/29;
  
  return distancia;
}

void motor(int dir,int m1,int m2)   //Funcion para controlar los motores
{
  if(dir==1)
  {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  else if(dir==-1)
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }
  else if(dir==2)
  {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }
  else if(dir==-2)
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  else
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
  }

  analogWrite(ENA,m1);
  analogWrite(ENB,m2);
}

int sele()                //Funcion que detecta si el camino izquierdo o derecho es el mas largo girando hacia el mismo
{
  int iz;
  int de;
  int aux;
  motor(0,0,0);
  delay(200);
  
  servo1.write(0);
  delay(2000);
  de = ultra(t1,e1);
  delay(20);

  servo1.write(180);
  delay(2000);
  iz = ultra(t1,e1);
  delay(20);

  servo1.write(90);
  delay(500);
  
  if(de > iz)
  {
    aux = ultra(t2,e2);
    delay(20);
    while(aux < de)
    {
      motor(2,150,150);
      aux = ultra(t2,e2);
      delay(20);
    }
    return de;
  }
    
  else
  {
    aux = ultra(t2,e2);
    delay(20);
    while(aux < iz)
    {
      motor(-2,150,150);
      aux = ultra(t2,e2);
      delay(20);
    }
    return iz;
  }
}

