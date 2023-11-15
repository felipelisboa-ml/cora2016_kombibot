
//PINOS
const int avgSpeedDIR = 220; // 230 FUNCIONANDO
const int avgSpeedESQ = 255; // 255 FUNCIONANDO
const int pwmA = 3; //motor esq
const int pwmB = 2; //motor dir
const int IN1 = 12; // dir
const int IN2 = 11;
const int IN3 = 10; //esq
const int IN4 = 9;

//CONSTANTES DE CONTROLE
const int Kp=60; //60
const int Kd=230; //220
const int Ki=0;

//SENSORES E MODO DE OPERAÇÃO
unsigned int LF[7] = {0,0,0,0,0,0,0};
unsigned int OBS[2] = {0,0};
enum Mode {NORMAL,PARADO};
Mode Modo = NORMAL;
int dif = 0;
int cont_Z = 0;
int cont_X = 0;
int cont_W = 0;

//VARIÁVEIS PID
int prop = 0;
int derivativo = 0;
int integral = 0;
int erro = 0;
int U_erro = 0;
int PID = 0;

//VELOCIDADES
int velesq=0;
int veldir=0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(7, INPUT);
  pinMode(6, INPUT);  
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(24, OUTPUT);
  delay(3000);
}
void readSensors()
{ 
  LF[0] = digitalRead(A0);
  LF[1] = digitalRead(A1);
  LF[2] = digitalRead(A2);
  LF[3] = digitalRead(A3);
  LF[4] = digitalRead(A4);
  LF[5] = digitalRead(A5);
  LF[6] = digitalRead(A6);
}
void readObstaculo()
{
  OBS[0] = digitalRead(6); //ESQ
  OBS[1] = digitalRead(7); //DIR
}
void curvaDireita()
{   
    digitalWrite(24,HIGH); //LED VERDE
    digitalWrite(IN1,HIGH); // ROTAÇÃO REVERSA
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(pwmA, 255); //ESQ
    analogWrite(pwmB, 30); //DIR
    delay(400); // Durante 0,45 segundos
    dif = 0; // Reseta variáveis
    cont_Z = 0;
    cont_X = 0;
    digitalWrite(24,LOW); //Apaga LED VERDE
    digitalWrite(13,LOW); //Apaga LED VERMELHO
}
void curvaEsquerda()
{
    digitalWrite(24,HIGH);  
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 255);
    delay(400); // Durante 0,4 segundos
    dif = 0;
    cont_Z = 0;
    cont_X = 0;
    digitalWrite(24,LOW);
    digitalWrite(13,LOW);
}
void Faixa()
{
    digitalWrite(24,HIGH);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);
    delay(5000);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(pwmA, 255); //Levemente para a direita
    analogWrite(pwmB, 245);
    delay(750);
    dif = 0;
    cont_Z=0;
    cont_X=0;
    digitalWrite(24,LOW);
    digitalWrite(13,LOW);
}
void calculaErro()
{
  readSensors(); //PISTA DE TESTE
  if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==LOW && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH)
    erro = 0;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==LOW && LF[4]==LOW && LF[5]==HIGH && LF[6]==HIGH)
    erro = 1;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==LOW && LF[3]==LOW && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH)
    erro = -1;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==HIGH && LF[4]==LOW && LF[5]==HIGH && LF[6]==HIGH)
    erro = 2;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==LOW && LF[3]==HIGH && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH)
    erro = -2;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==HIGH && LF[4]==LOW && LF[5]==LOW && LF[6]==HIGH)
    erro = 3;
  else if(LF[0]==HIGH && LF[1]==LOW && LF[2]==LOW && LF[3]==HIGH && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH)
    erro = -3;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==LOW && LF[6]==HIGH)
    erro = 4;
  else if(LF[0]==HIGH && LF[1]==LOW && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH)
    erro = -4;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==LOW && LF[6]==LOW)
    erro = 5;
  else if(LF[0]==LOW && LF[1]==LOW && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH)
    erro = -5;
  else if(LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==HIGH && LF[6]==LOW)
    erro = 6;
  else if(LF[0]==LOW && LF[1]==HIGH && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH)
    erro = -6;
    
  else if(LF[0]==LOW && LF[1]==LOW && LF[2]==LOW && LF[3]==HIGH && LF[4]==LOW && LF[5]==LOW && LF[6]==LOW) //Dificuldade
    erro = 0;
  else if(LF[0]==LOW && LF[1]==LOW && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==LOW && LF[6]==LOW) //Dificuldade
    erro = 0;
  else if(LF[0]==LOW && LF[1]==LOW && LF[2]==HIGH && LF[3]==HIGH && LF[4]==LOW && LF[5]==LOW && LF[6]==LOW)//Dificuldade "torta"
    erro = -1;
  else if(LF[0]==LOW && LF[1]==LOW && LF[2]==LOW && LF[3]==HIGH && LF[4]==HIGH && LF[5]==LOW && LF[6]==LOW)//Dificuldade "torta"
    erro = 1;
  else if(LF[0]==LOW && LF[1]==LOW && LF[2]==LOW && LF[3]==LOW && LF[4]==LOW && LF[5]==LOW && LF[6]==LOW) // Faixa na Pista
    erro = 0;
}
void calculaPID()
{
   if(erro==0)
    integral = 0;
   prop = erro;
   integral = integral + erro;
    if(integral > 255)
      integral = 255;
    else if(integral < -255)
      integral = -255;
   derivativo = erro - U_erro;
   PID = ((Kp * prop) + (Ki * integral) + (Kd * derivativo));
   U_erro = erro;  
}
void controleMotor() 
{
    if(PID>=255) // ROTAÇÕES REVERSAS
    {
       digitalWrite(IN1,HIGH);
       digitalWrite(IN2,LOW);
       digitalWrite(IN3,LOW);
       digitalWrite(IN4,HIGH);
       analogWrite(pwmA,255);
       analogWrite(pwmB,100);
    }
    else if(PID<=-255)
    {
       digitalWrite(IN1,LOW);
       digitalWrite(IN2,HIGH);
       digitalWrite(IN3,HIGH);
       digitalWrite(IN4,LOW);
       analogWrite(pwmA,100);
       analogWrite(pwmB,255);
    }
    Serial.print(PID,10);
    Serial.print(" ");
    Serial.print(erro,10);
    Serial.print(" ");
    if(PID>=0) //VIRA PARA A DIREITA
    {
      velesq = avgSpeedESQ;
      veldir = avgSpeedDIR - PID;
    }
    else //VIRA PARA A ESQUERDA
    {
       velesq = avgSpeedESQ + PID;
       veldir = avgSpeedDIR;
    }
    Serial.print(velesq,10);
    Serial.print(" ");
    Serial.print(veldir,10);
    Serial.print(" ");
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(pwmA, velesq);
    analogWrite(pwmB, veldir);
}
void configMode() 
{
  readSensors();
  if (LF[0]==HIGH && LF[1]==HIGH && LF[2]==HIGH && LF[3]==HIGH && LF[4]==HIGH && LF[5]==HIGH && LF[6]==HIGH) //todos os sensores desativados
    Modo = PARADO; // PARADO
  else
    Modo = NORMAL; // NORMAL
}

void loop()
{
   configMode();
   switch(Modo)
   {
     case NORMAL:
     {
        //if(((LF[0]==LOW && LF[1]==LOW && LF[3]==HIGH && LF[5]==LOW && LF[6]==LOW) && !dif) //Detectou dificuldade
        if((LF[0] == LOW && LF[6] == LOW) && (LF[2]==HIGH || LF[3]==HIGH || LF[4]==HIGH))
        {
          dif = 1;
          digitalWrite(13,HIGH); // Acende Led Vermelho
        }

        if(dif)
          cont_W++;
          
        if(cont_W > 15)
        {
          if((LF[1]==LOW || LF[5]==LOW) && dif)
          {
            dif = 0;
            digitalWrite(13,LOW); // Apaga LED
            cont_W = 0;
          }
        }
       
        if(dif)
        {
          cont_Z++;
          if(cont_Z > 76) //100
          {
            dif = 0;
            digitalWrite(13,LOW);
          } 
          else 
          {
            cont_X++;
            if(cont_X > 45) //50
            {
              readObstaculo();
              if(OBS[1] == LOW && LF[3]==LOW && OBS[0]==HIGH)
              {
                delay(120);
                curvaDireita(); 
              }
              else if(OBS[0] == LOW && OBS[1] == LOW) //Caso Ambíguio: Decide Esquerda
              {
                delay(90);
                curvaDireita();
              }
              else if(OBS[0] == LOW && LF[3]==LOW && OBS[1] == HIGH)
              {
                delay(90);
                curvaEsquerda();
              }
            }
          }
        }     
        else
        {
          cont_Z = 0;
          cont_X = 0;
          cont_W = 0;
        }
        calculaErro();
        calculaPID();
        controleMotor();
        break;
     }
     case PARADO:
     {
          if(dif)
            Faixa();
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,HIGH);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,HIGH);
          analogWrite(pwmA, 0);
          analogWrite(pwmB, 0);
          break;
     }
   }       
   Serial.print(Modo,10);
   Serial.print("\n");
}
