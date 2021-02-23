#include <16f877a.h>
#use delay(clock=4M)
#fuses xt,nowdt,noprotect,noput
#use fast_io (b)
#use fast_io (d)
#use fast_io (e)
#byte ultra=0x06       //PortB
#byte llantas=0x08     //PortD
#byte ultra2=0x09      //PortE
#define echoIz pin_b0   // Echo Entrada
#define trigIz pin_b1   // Trig Salida
#define echoDe pin_b2
#define trigDe pin_b3
#define echoCe pin_b4
#define trigCe pin_b5
#define echoIzD pin_b6  //IzquierdaDetras
#define trigIzD pin_b7
#define echoDeD pin_e0  
#define trigDeD pin_e1
#define high   output_high
#define low    output_low
////////////////////////////////////ENTRADAS////////////////////////////////////
//////1 BOTON COMIENZO//////////
//////4 SENSORES INFRAROJOS/////
//////5 SENSORES ULTRASONICOS///
////////////////////////////////////SALIDAS/////////////////////////////////////
//////4 CONTROLADORES PARA 2 MOTORES///////
////////////////////////////////////////////////////////////////////////////////
//ASIGNACIÓN DE PINES DE ENTRADA: RB0 -> RB7 + E0 -> E1
//ASIGNACIÓN DE PINES  DE SALIDA: RD0 -> RD3

///////////Asignación de etiquetas/nombres a los comandos Hexa//////////////////   
int adelante=0x0F;  //Se le asigna a "adelante" el codigo Hexa
int atras=0x00;


int trig[5]={trigIz,trigDe,trigCe,trigIzD,trigDeD};
int echo[5]={echoIz,echoDe,echoCe,echoIzD,echoDeD};
int Sense[5]={};
int a,direc=2;
float DCentro;         //Equivale a la distancia

void UltraSensor(){
 for(a=0;a<=4;a++){


   float T,D;
   high(trig[a]);
   delay_us(10);
   low(trig[a]);
      while(!input(echo[a]))
      {}
      set_timer1(0);
      while(input(echo[a]))
      {}
      T=get_timer1();
      D=(T/2)/(29.15);
      if(a==2){
         DCentro=(T/2)/(29.15);
      }
      if (D<10){
      Sense[a]=1;
      }
      else{
      Sense[a]=0;
      }


 }
}


void main(){
   set_tris_b(0x55);
   set_tris_d(0xF0);
   set_tris_e(0x01);
   llantas=0x00;
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   setup_timer_0(T0_INTERNAL|T1_DIV_BY_1);
   
   for(;;){
      UltraSensor();
      
      if(Sense[0]==1) direc=1;
      if(Sense[1]==1) direc=6; //#6 ya que el 2 daba problemas
      if(Sense[3]==1) direc=4;
      if(Sense[4]==1) direc=5;
      if(Sense[2]==1) direc=3; //US Centro/Frontal
      
      
      switch(direc){
      case 2:{          //Case de Inicio, Se cambió de ->0 a ->2, ya que siempre comenzaba en el 2
         llantas=atras;
      }
      break;
      case 1:{
         llantas=atras;
      }
      break;
      case 6:{
         llantas=adelante;
      }
      break;
      case 3:{
         if((DCentro>0)&&(DCentro<10)){
            set_timer0(53035);
            while(get_timer0()>=53035)      //Para un parpadeo de 100ms con un postcaler de 1
            {}
            output_toggle(pin_d2);
         }
      }
      break;
      case 4:{
         llantas=adelante;
      }
      break;
      case 5:{
         llantas=adelante;
      }
      break;
      }
   }
}

