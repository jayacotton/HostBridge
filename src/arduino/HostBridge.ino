// Host Bridge reads/writes data via 2 pio ports, implemented with
// mcp23017 i2c chips.  The remote host is a z80pio chip using
// pio port a and b.
//
#include <Wire.h>
#include <MCP23017.h>

#define Mcp1 0x20
#define Mcp2 0x21
MCP23017 OutPut = MCP23017(Mcp1);
MCP23017 InPut = MCP23017(Mcp2);

#define ASTRB 8+0
#define ARDY 8+1
#define TXRDY ARDY
#define BSTRB 8+2
#define BRDY 8+3
#define RXRDY BRDY
#define ETX 3
void setup() {
  Wire.begin();
  Serial.begin(115200);
  OutPut.init();
  InPut.init();
  InPut.portMode(MCP23017Port::A,0b11111111);
  OutPut.portMode(MCP23017Port::A,0);
  InPut.writeRegister(MCP23017Register::GPIO_A,0);
  OutPut.writeRegister(MCP23017Register::GPIO_A,0);
  //port b of InPut is the strobe and ready bits for all
  InPut.portMode(MCP23017Port::B,0b11111100);
  InPut.digitalWrite(ASTRB,1);
  InPut.digitalWrite(BSTRB,1);
// see if we have the expanders on line
  Wire.beginTransmission(Mcp1);
  if(Wire.endTransmission())
    Serial.print("Port A missing ");
  Wire.beginTransmission(Mcp2);
  if(Wire.endTransmission())
    Serial.print("Port B missing");
  Serial.println();
}
void MCPWrite(const char *string)
{
  while(*string != 0){
    while(!InPut.digitalRead(TXRDY)){
      OutPut.writeRegister(MCP23017Register::GPIO_B,*string++);
      // pulse the strobe bit
      InPut.digitalWrite(BSTRB,0);
      InPut.digitalWrite(BSTRB,1);
    }
  }
  OutPut.writeRegister(MCP23017Register::GPIO_B,char(ETX));
  InPut.digitalWrite(BSTRB,0);
  InPut.digitalWrite(BSTRB,1);
}
char lstr[130];
const char *MCPRead()
{
  char *plstr = &lstr[0];
  if(InPut.digitalRead(RXRDY))
  {
    while(InPut.digitalRead(RXRDY)){
      *plstr = InPut.readRegister(MCP23017Register::GPIO_A);
      if(*plstr == char(ETX)) return &lstr[0];
    }
    
  }
  else
    return "Error";
}

void loop() {
    MCPWrite("this is a test");
    Serial.print(MCPRead());
    Serial.println();
}
