#include <SPI.h>
#include <LoRa.h>


#define ss 15
#define rst 4
#define dio0 2

int counter = 0;


String getStringPartByNr(String data, char separator, int index)
{
  int stringdata = 0;
  String datapart ="";
  for (int i=0; i<data.length()-1; i++)
  {
    if(data[i]==separator)
    {
      stringdata++;
    }
    else if (stringdata== index)
    {
      datapart.concat(data[i]);
    }
    else if (stringdata>index)
    {
      return datapart;
      break;
    }
  }
  return datapart;
}


void setup() {
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa rece");


  LoRa.setPins(ss, rst, dio0);
  
 
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
 
  Serial.println("LoRa SET");
}

void loop() {
 
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
   
    Serial.print("Received packet '");

    
    while (LoRa.available()) {
      
      String rcv =LoRa.readString();
      Serial.print(rcv);
      Serial.print("' with RSSI ");
      Serial.print(LoRa.packetRssi());
      Serial.println("\n");
      String MOIS=getStringPartByNr(rcv,':',0);
      String TEMP=getStringPartByNr(rcv,':',1);
      String HUMI=getStringPartByNr(rcv,':',2);
      String PUMP=getStringPartByNr(rcv,':',3);
      String SPRINKLER=getStringPartByNr(rcv,':',4);
      Serial.println("Moisture:"+MOIS);
      Serial.println("Temperature"+TEMP);
      Serial.println("Humidity:"+HUMI);
      Serial.println("Pump_state:"+PUMP);
      Serial.println("Sprinkler_state:"+SPRINKLER);
      Serial.println("\n");
      delay(1000);

    
    }

    
    
  }
}