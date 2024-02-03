#include <Servo.h>

/* Program Otto v 1.1.1 by Fernandezaginer
 * April 2017 (1.1.0 en Mayo 2017)
 * Originally made by Andrés Fernández Muñoz  */


// --------------------------------------------------------------
//                        INSTRUCTIONS
// --------------------------------------------------------------
 

/*                         ENGLISH
 *  
 *  1 Connect the pins as indicated in the variables named PINES
 *    on the Arduino. (Lines 80-110)
 *  
 *  2 Calibrate the Servomotors by setting the angles that
 *    correspond to the positions where the robot is straight. 
 *    Use the variables VCA, VCB, VCC, and VCD. Try different
 *    values; they should be around 90 degrees. (Lines 130-135).
 *    
 *  3 Select your type of LED (Negative or Positive common) on 
 *    (Line 139). If it is Negative, put 1; if it is positive, put 2.
 *   
 *  4 If you are using the Chinese Arduino Nano, you need to
 *    install the drivers.
 *  
 *  5 In the Tools menu, select the Board (Arduino Nano), Processor
 *    (ATmega328), and the port. If you don't know the port, you
 *    can find it in the Device Manager in the control panel. For
 *   jhexample, it could be (COM3).
 *    
 *  6 Now we are ready to upload the progran, remember to conect
 *    the board and clic at upload or Ctrl+U.
 *    
 */


 /*                        ESPAÑOL
 *  
 *  1 Pon los pines que has conectado en tu arduino en la seccion
 *    de pines (Lineas 80-110)
 *  
 *  2 Calibra los servomotores en la seccion de variables valores
 *    calibrados, poniendo un valor aproximado en el que coincida
 *    cuando el robot esté recto, tendras que cargar varias veces
 *    el programa con los valores que has puesto.
 *    (Lineas 130-135)
 *    
 *  3 Selecciona el tipo de led (Negativo o Positivo común) en la
 *    (linea 139), pon 1 si es de negativo común o 2 si es de  
 *    positivo común
 *   
 *  4 Si has usado el Arduino Nano chino instala sus drivers
 *  
 *  5 Selecciona la placa en Herramientas/Placa (Arduino Nano),
 *    selecciona el procesador en Herramientas/Procesador 
 *    (ATmega328) y sececciona le puerto en Herramientas/Puerto
 *    si no saves el puerto puedes saberlo en el Panel de
 *    Control / Hardware y sonido / Administrador de 
 *    dispositivos, un ejemplo seria (COM3).
 *    
 *  6 Ahora ya podemos cargar el programa a nuestro arduino, 
 *    recuerda conectar la placa y cargalo en 'Subir' o Ctrl+U.
 *    
 */


// --------------------------------------------------------------
//                          PINES
// --------------------------------------------------------------

//PIN 0 BT
//PIN 1 BT

//Ultrasonidos
byte pinuttrig=3;
byte pinutecho=2;

//Servos
byte pinaservo=4;
byte pinbservo=5;
byte pincservo=6;
byte pindservo=7;

//Led RGB
byte pinLRGBr=11;
byte pinLRGBg=9;
byte pinLRGBb=10;

//Otros
byte pinbuzzer=14;
byte pinbot=13;

//Carita
byte pincaa=15;
byte pincab=16;
byte pincac=17;
byte pincad=18;

//Servos
Servo aservo;
Servo bservo;
Servo cservo;
Servo dservo;

/*       Positions of the Servos
            --------------- 
           |     O   O     |
           |---------------|
 Servo B→  |               | ← Servo D
            --------------- 
               ║     ║
               ║     ║
 Servo A→   -----   ------  ← Servo D */


// --------------------------------------------------------------
//                           VARIABLES
// --------------------------------------------------------------

// Valores Calibrados 
/*Put here the values calibrated (degrees) */
int VCA=85;
int VCB=70;
int VCC=72;
int VCD=88;

// Led RGB
/*Put here the tipe of your led*/
int Led=2;       // 1=Negativo Comun , 2=Positivo Comun


//Variables
int bt;
int conta=1;
int contb=59;

//Varialbles de movimiento
byte estado=0;
byte car=0;
byte esp=0;
byte EstabaPuntillas=0;
byte EstabaAsustado=0;
int vel=150;

//Ultrasonidos
int duracion;
int distancia;
int distanciamax=20;
boolean ut=1;

// Notas Buzzer
int nota[12] = {262,277,294,311,329,349,369,392,415,440,466,494};
/* NOTAS
 * 1=DO 2=DO# 3=RE 4=RE# 5=MI 6=FA 7=FA# 
 * 8=SOL 9=SOL# 10=LA 11=SIb 12=SI
 */

// ------------------------- SETUP ---------------------------------------

void setup() {
  
  Serial.begin(9600);
  
  /*Inicializar Pines*/
  pinMode(pinuttrig,OUTPUT);
  pinMode(pinutecho,INPUT);
  aservo.attach(pinaservo);
  bservo.attach(pinbservo);
  cservo.attach(pincservo);
  dservo.attach(pindservo);
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(1000);
  pinMode(pinLRGBr,OUTPUT);
  pinMode(pinLRGBg,OUTPUT);
  pinMode(pinLRGBb,OUTPUT);
  LedOff();
  pinMode(pinbuzzer,OUTPUT);
  pinMode(pinbot,INPUT);
  pinMode(pincaa,OUTPUT);
  pinMode(pincab,OUTPUT);
  pinMode(pincac,OUTPUT);
  pinMode(pincad,OUTPUT);


  if(digitalRead(pinbot)==0){
    SonidoInicioconLed();
  }
  else{
    LedRojo();
    delay(100);
    LedOff();
    delay(100);
    LedRojo();
    delay(100);
    LedOff();
    delay(100);
    LedRojo();
    delay(100);
    LedOff();
    delay(100);
  }

}

// ------------------------- LOOP ----------------------------------------

void loop() {

  /*Leer Bluetooth*/
  ReadBT();

  /*Movimientos*/
  if(estado==0){//Parado
    if(EstabaPuntillas==1){
      Quitarpuntillas();
      EstabaPuntillas=0;
    }
    if(EstabaAsustado==1){
      Desasustar();
      EstabaAsustado=0;
    }
    Parado();
  }
  if(estado==1){//Alante
    Alante();
  }
  if(estado==2){//IzquierdaN
    Izquierdan();
  }
  if(estado==3){//Derecha L
    Derechal();
  }
  if(estado==4){//Derecha N
    Derechan();
  }
  if(estado==5){//Izquierda L
    Izquierdal();
  }
  if(estado==6){//Atras
    Atras();
  }

  /*Ultrasonidos*/
  if(ut==1){
    ReadUT();
    if(distancia<distanciamax&&esp<1){
      ReadUT();
      ReadUT();
      if(distancia<distanciamax&&esp<1){
        ReadUT();
        ReadUT();
        if(distancia<distanciamax&&esp<1){
          Parado();
          Ponerpuntillas();
          LedRosa();
          for(distancia>distanciamax;distancia<distanciamax+2||ut==0;){
            ReadUT();
            if(distancia<distanciamax+2){
              ReadUT();
              if(distancia<distanciamax+2){
                ReadUT();
                if(distancia<distanciamax+2){
                  ReadUT();
                  if(distancia<distanciamax+2){
                    ReadUT();
                  }
                }
              }
            }
            delay(300);
          }
          Quitarpuntillas();
        }
      }
    }
  }

  
  if(ut==0){
    ReadUT();
  }

  /*Funciones especiales
   *- Baile              'o' 1
   *- Asustar            'p' 2
   *- Saludo             'q' 3
   *- Puntillas          'r' 4
   */
   if(esp==1){
     Baile();
   }
   if(esp==2){
    if(EstabaAsustado==0){
      Asustar();
      delay(500);
    }
    EstabaAsustado=1;
   }
   if(esp==3){
     Saludo();
     esp=0;
   }
   if(esp==4){
    if(EstabaPuntillas==0){
      Ponerpuntillas();
    }
    EstabaPuntillas=1;
   }

  /*Caritas*/
  if(car==0){//Sonriente
    LedVerde();
  }
  if(car==1){//Sorprendida
    LedCian();
  }
  if(car==2){//Regular
    LedAmarillo();
  }
  if(car==3){//Triste
    LedRojo();
  }
  if(car==4){//Off
    //LedOff();
  }
  
  delay(10);

  
}


// --------------------------------------------------------------
//                        CONFIG APK
// --------------------------------------------------------------


void ReadBT(){
  if(Serial.available() > 0){
    int bt;
    bt=Serial.read();

    /*Movimientos*/
    if(bt=='a'){estado=1;esp=0;}//Alante
    if(bt=='b'){estado=2;esp=0;}//IzquierdaN
    if(bt=='c'){estado=3;esp=0;}//DerechaL
    if(bt=='d'){estado=0;esp=0;}//Stop
    if(bt=='e'){estado=4;esp=0;}//DerechaN
    if(bt=='f'){estado=5;esp=0;}//IzquierdaL
    if(bt=='g'){estado=6;esp=0;}//Atras

    /*Caritas*/
    if(bt=='h'){car=0;}
    if(bt=='i'){car=1;}
    if(bt=='j'){car=2;}
    if(bt=='k'){car=3;}

    /*Ultrasonidos*/
    if(bt=='s'){ut=1;}
    if(bt=='l'){ut=0;}

    /*Otros*/
    if(bt=='o'){esp=1;estado=9;}
    if(bt=='p'){esp=2;estado=9;}
    if(bt=='q'){esp=3;estado=9;}
    if(bt=='r'){esp=4;estado=9;}

    /*Working?*/
    if(bt=='?'){Serial.println("I'm working");}
    
  }
  
}

// --------------------------------------------------------------
//                    FUNCIONES DE MOVIMIENTO
// --------------------------------------------------------------


void Parado(){                       // Parado
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
}

void Alante(){
  int ang=20;

  

  /*Parte2*/  
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA+ang);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA);
  bservo.write(VCB-ang);//ANT
  cservo.write(VCC-ang);//ANT
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA-ang);//CONTRATIO 1
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD-ang);//CONTRATIO 1
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel+200);
  ReadBT();
  if(estado==0){return;}

  

  /*Parte1*/ 
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA-ang);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA);
  bservo.write(VCB+ang);//ANT
  cservo.write(VCC+ang);//ANT
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA+ang);//CONTRATIO 1
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD+ang);//CONTRATIO 1
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel+200);
  ReadBT();
  if(estado==0){return;}

  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel+200);
  ReadBT();
  if(estado==0){return;}

  Derechan();
  
}

void Atras(){                        // Atras
  int ang=22;
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD);
  delay(vel+50);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD);
  delay(vel+50);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD+ang);
  delay(vel+50);
  ReadBT();
  if(estado==0){return;}

  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD);
  delay(vel+50);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD);
  delay(vel+50);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  dservo.write(VCD-ang);
  delay(vel+100);
  ReadBT();
  if(estado==0){return;}


}

void Derechal(){                     // Derecha lateral
  int ang=30;
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
}

void Izquierdal(){                   // Izquierda Lateral
  int ang=30;
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
}

void Izquierdan(){                   // Izquierda Normal
  int ang=30;
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB-ang);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB-ang);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB-ang);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
}

void Derechan(){                   // Derecha Normal
  int ang=30;
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC+ang);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC+ang);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC+ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
}

void GirarIzquierda(){                // Giro Izquierda
  int ang=30;
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC+ang);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC+ang);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC+ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
}

void Derechant(){                     // Derecha Normal Trasera
  int ang=20;
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC-ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC-ang);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC-ang);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
}

void IzquierdaNormalTrasera(){
  int ang=30;
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA-ang);
  bservo.write(VCB);
  cservo.write(VCC-ang);
  dservo.write(VCD-ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC-ang);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC-ang);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+ang);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(vel);
  ReadBT();
  if(estado==0){return;}
}

void Moverservosbcconpausa(){        // Mover ByC
  int ang=50;
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  delay(vel);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  delay(vel*2);
  bservo.write(VCB);
  cservo.write(VCC);
  delay(vel*2);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  delay(vel);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  delay(vel*2);
  bservo.write(VCB);
  cservo.write(VCC);
  delay(vel*2);
}

void Moverservosbcsinpausa(){        // Mover ByC
  int ang=50;
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  delay(vel);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  delay(vel*2);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  delay(vel);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  delay(vel*2);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  delay(vel);
  bservo.write(VCB-ang);
  cservo.write(VCC-ang);
  delay(vel*2);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  delay(vel);
  bservo.write(VCB+ang);
  cservo.write(VCC+ang);
  delay(vel*2);
}

void Ponerpuntillas(){               // Poner de puntillas
  int cont=0;
  for(cont>0;cont<70;cont++){
    aservo.write(VCA-cont);
    bservo.write(VCB);
    cservo.write(VCC);
    dservo.write(VCD+cont);
    delay(5);  
  }
}
void Quitarpuntillas(){              // Quitar de puntillas
  int cont=70;
  for(cont<70;cont>0;cont--){
    aservo.write(VCA-cont);
    bservo.write(VCB);
    cservo.write(VCC);
    dservo.write(VCD+cont);
    delay(5);  
  }  
}

void Asustar(){                      // Asustar
   aservo.write(VCA+70);
   bservo.write(VCB);
   cservo.write(VCC);
   dservo.write(VCD-70);
   delay(200);
   aservo.write(VCA+70);
   bservo.write(VCB-40);
   cservo.write(VCC+40);
   dservo.write(VCD-70);
   delay(100);
}

void Desasustar(){                   // Desasustar
   aservo.write(VCA+70);
   bservo.write(VCB);
   cservo.write(VCC);
   dservo.write(VCD-70);
   delay(100);
   aservo.write(VCA);
   bservo.write(VCB);
   cservo.write(VCC);
   dservo.write(VCD);
   delay(100);
}

void Saludo(){                       // Saludo
  int ang=32;
  aservo.write(VCA+ang-10);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+90);
  delay(100);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+90);
  delay(400);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-40);
  delay(400);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+40);
  delay(400);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-40);
  delay(400);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+40);
  delay(400);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-40);
  delay(400);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD+40);
  delay(400);
  aservo.write(VCA+ang);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD-40);
  delay(400);
  aservo.write(VCA);
  bservo.write(VCB);
  cservo.write(VCC);
  dservo.write(VCD);
  delay(200);
}

void Servoread(){                    // Lectura de servos
  Serial.print("ServoA=");
  Serial.print(aservo.read());
  Serial.print(" ServoB=");
  Serial.print(bservo.read());
  Serial.print(" ServoC=");
  Serial.print(cservo.read());
  Serial.print(" ServoD=");
  Serial.println(dservo.read());
  delay(10);
}


// --------------------------------------------------------
//                        BAILE
// --------------------------------------------------------

void Baile(){
  Parado();
  int cont1=0;
  for(cont1>=0;cont1<5;cont1++){
    Ponerpuntillas();
    Quitarpuntillas();
    ReadBT();
    if(esp!=1){
      return;
    }
  }
  int cont2=0;
  for(cont2>=0;cont2<5;cont2++){
    Izquierdal();
    Izquierdal();
    Derechal();
    Derechal();
    Derechal();
    Derechal();
    Izquierdal();
    Izquierdal();
    ReadBT();
    if(esp!=1){
      return;
    }
  }
  int cont3=0;
  for(cont3>=0;cont3<5;cont3++){
    Ponerpuntillas();
    Quitarpuntillas();
    ReadBT();
    if(esp!=1){
      return;
    }
  }
  int cont4=0;
  for(cont4>=0;cont4<10;cont4++){
    Moverservosbcsinpausa();
    ReadBT();
    if(esp!=1){
      return;
    }
  }
  int cont5=0;
  for(cont5>=0;cont5<5;cont5++){
    Izquierdal();
    Izquierdal();
    Derechal();
    Derechal();
    Derechal();
    Derechal();
    Izquierdal();
    Izquierdal();
    ReadBT();
    if(esp!=1){
      return;
    }
  }
  int cont6=0;
  for(cont6>=0;cont6<5;cont6++){
    Ponerpuntillas();
    Quitarpuntillas();
    ReadBT();
    if(esp!=1){
      return;
    }
  }
  delay(2000);
}


// --------------------------------------------------------
//                     ULTRASONIDOS
// --------------------------------------------------------


void ReadUT(){
  digitalWrite(pinuttrig, HIGH);
  delay(0.01);
  digitalWrite(pinuttrig, LOW);
  duracion = pulseIn(pinutecho, 1);
  distancia =abs((duracion/2) / 29); 
  delay(2);
  //Serial.println(distancia);
}


// --------------------------------------------------------
//                        LED RGB
// --------------------------------------------------------

void LedBlanco(){
  analogWrite(pinLRGBr,-255);
  analogWrite(pinLRGBg,-255);
  analogWrite(pinLRGBb,-255);
}

void LedRojo(){
  analogWrite(pinLRGBr,-255);
  analogWrite(pinLRGBg,255);
  analogWrite(pinLRGBb,255);  
}

void LedVerde(){
  analogWrite(pinLRGBr,255);
  analogWrite(pinLRGBg,-255);
  analogWrite(pinLRGBb,255);  
}

void LedAzul(){
  analogWrite(pinLRGBr,255);
  analogWrite(pinLRGBg,255);
  analogWrite(pinLRGBb,-255);  
}

void LedAmarillo(){
  analogWrite(pinLRGBr,-255);
  analogWrite(pinLRGBg,-255);
  analogWrite(pinLRGBb,255);  
}

void LedCian(){
  analogWrite(pinLRGBr,255);
  analogWrite(pinLRGBg,-255);
  analogWrite(pinLRGBb,-255);  
}

void LedRosa(){
  analogWrite(pinLRGBr,-255);
  analogWrite(pinLRGBg,255);
  analogWrite(pinLRGBb,-255);  
}

void LedOff(){
  analogWrite(pinLRGBr,255);
  analogWrite(pinLRGBg,255);
  analogWrite(pinLRGBb,255);
}

void ProvandoLed(){
  LedBlanco();
  delay(1000);
  LedRojo();
  delay(1000);
  LedVerde();
  delay(1000);
  LedAzul();
  delay(1000);
  LedAmarillo();
  delay(1000);
  LedCian();
  delay(1000);
  LedRosa();
  delay(1000);
  LedOff();
  delay(1000);
}

// --------------------------------------------------------
//                        SONIDOS
// --------------------------------------------------------


void SonidoInicioconLed(){

  int n1=784;
  int n2=880;
  int n3=1047;
  
  int tempo = 130;
  int ligue = 40;
  LedVerde();
  delay(200);
  tone(pinbuzzer, n1);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n1);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n1);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n1);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n2);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n1);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n2);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n2);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(tempo * 2);
  tone(pinbuzzer, n1);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n2);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n3);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n3);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  delay(tempo * 2);
  tone(pinbuzzer, n2);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n3);
  LedVerde();
  delay(tempo - ligue);
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n2);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n2);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n1);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  tone(pinbuzzer, n1);
  LedVerde();
  noTone(pinbuzzer);
  LedOff();
  delay(ligue);
  delay(tempo * 2);
  LedOff();
  delay(100);
  tone(pinbuzzer, 784, 100);
  LedVerde();
  delay(100);
  tone(pinbuzzer, 880, 100);
  LedOff();
  delay(100);
  tone(pinbuzzer, 1047, 100);
  LedVerde();
  delay(100);
  tone(pinbuzzer, 880, 100);
  LedOff();
  delay(100);
  tone(pinbuzzer, 1040, 100);
  LedVerde();
  delay(100);
}
