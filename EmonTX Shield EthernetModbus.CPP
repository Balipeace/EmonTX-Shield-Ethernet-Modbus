/*
  EmonTx CT123 Voltage and Current serving as a Modbus slave

  EmonTx is part of the openenergymonitor.org project
  Licence: GNU GPL V3
  Author: Trystan Lea

  Mudbus.cpp - an Arduino library for a Modbus TCP slave.
  Copyright (C) 2011  Dee Wykoff

  Adjustments made by Gustave
  Bali, Indonesia
*/

#include <EmonLib.h>  //Always
#include <Ethernet.h> //used with ether shield
#include <Mudbus.h>   //Modbus version for IP


// Create  instances for each CT channel
EnergyMonitor ct1,ct2,ct3, ct4;

// On-board emonTx LED
const int LEDpin = 9;  

Mudbus Mb;     //used with ether

void setup() 
{
  delay(1000); // not really needed but provides starts stability
  Serial.begin(9600);
  Serial.println("emonTX Shield CT123 Voltage Serial and Modbus example"); 
  Serial.println("OpenEnergyMonitor.org");

  //used with ether
  uint8_t mac [] = {0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06};
  uint8_t ip [] = {192, 168, 1, 131};
  uint8_t gateway [] = {192, 168, 1, 3};
  uint8_t subnet [] = {255, 255, 255, 0};
  Ethernet.begin (mac, ip, gateway, subnet);
  Serial.print("Modbus Slave is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("");
  Serial.println("");
  //used with ether

  // Calibration factor = CT ratio / burden resistance = (100A / 0.05A) / 33 Ohms = 60.606
  ct1.current(1, 60.606);
  ct2.current(2, 60.606);                                     
  ct3.current(3, 60.606);
  ct4.current(4, 60.606); 
  
  // (ADC input, calibration, phase_shift) 
  // calibration based on actual readings in Berawa which is supposed to be 220V, but mostly not
  ct1.voltage(0, 284.0, 1.7);                                
  ct2.voltage(0, 284.0, 1.7);                                
  ct3.voltage(0, 284.0, 1.7);
  ct4.voltage(0, 284.0, 1.7);
  
  // Setup indicator LED
  pinMode(LEDpin, OUTPUT);                                              
  digitalWrite(LEDpin, HIGH);                                                                                  
}

void loop() 
{ 
  Mb.Run();    // start MbTcp subroutine
  //delay(1000); // Calm things down

  // Calculate all. No.of crossings, time-out 
  ct1.calcVI(20,2000);                                                  
  ct2.calcVI(20,2000);
  ct3.calcVI(20,2000);
  ct4.calcVI(20,2000);
    
  // Print power 
  Serial.print(ct1.realPower);     
  Serial.print(" "); 
  Serial.print(ct2.realPower);
  Serial.print(" "); 
  Serial.print(ct3.realPower);
  Serial.print(" "); 
  Serial.print(ct4.realPower);
  Serial.print(" "); 
  Serial.print(ct1.Vrms);
  Serial.println();
  Serial.println("");

  // Modbus Registers
  Mb.R[0] = ct1.Vrms;
  Mb.R[1] = ct1.realPower;
  Mb.R[2] = ct2.realPower;
  Mb.R[3] = ct3.realPower;
  Mb.R[4] = ct4.realPower;
  Mb.R[5] = 500;
  Mb.R[6] = 600;
  Mb.R[7] = 700;
    
  // Available properties: ct1.realPower, ct1.apparentPower, ct1.powerFactor, ct1.Irms and ct1.Vrms

  delay(4000);
}
