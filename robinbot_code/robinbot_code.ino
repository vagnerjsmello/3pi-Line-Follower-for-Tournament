///**********************************************************
//    @título: 3pi Seguidor de linha
//    @autor: Vagner Mello
//    @descrição: Código para o seguidor de linha 3PI Pololu, baseado nas bibliotecas: Orangutan e Pololu.
//    Criado para a participação do 1º Torneio de Robótica CEFET-RJ. Esse código contempla:
//    - Mensagens de boas vindas;
//    - Calibração dos sensores;
//    - Total de bateria em mV;
//    - Otimização do PID para competição;
//    - Configurações de tempo;
//    - Configurações de velocidade;
//
//**********************************************************

// The 3pi include file must be at the beginning of any program that
// uses the Pololu AVR library and 3pi.
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>
#include <avr/pgmspace.h>

Pololu3pi robot;
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";
unsigned int sensors[5];
unsigned int last_proportional = 0;
int speed = 200;
long integral = 0;

// Variável para o controle de tempo de parada em millis 120000 = 2 min, 60000 = 1 min, 30000 = 30 seg.
unsigned long startMillisCount = 0;
unsigned long timeInSeconds = 0;
unsigned long stopCarTimeMillis = 0;


// Mensagens de boas vindas
const char welcome_line1[] PROGMEM = "Hi, I'm";
const char welcome_line2[] PROGMEM = "RobinBot";
const char welcome1_line1[] PROGMEM = "  Team ";
const char welcome1_line2[] PROGMEM = "Infoboto";
const char welcome2_line1[] PROGMEM = " INFNET";

// Data for generating the characters used in load custom characters
// and displayReadings.  By reading levels[] starting at various
// offsets, we can generate all of the 7 extra characters needed for a
// bargraph.  This is also stored in program space.
const char levels[] PROGMEM = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111};



void loadCustomCharacters()
{
  OrangutanLCD::loadCustomCharacter(levels + 0, 0);
  OrangutanLCD::loadCustomCharacter(levels + 1, 1);
  OrangutanLCD::loadCustomCharacter(levels + 2, 2);
  OrangutanLCD::loadCustomCharacter(levels + 3, 3);
  OrangutanLCD::loadCustomCharacter(levels + 4, 4);
  OrangutanLCD::loadCustomCharacter(levels + 5, 5);
  OrangutanLCD::loadCustomCharacter(levels + 6, 6);
  OrangutanLCD::clear();
}

void displayReadings(const unsigned int *calibrated_values)
{
  unsigned char i;
  for (i = 0; i < 5; i++)
  {
    // Initialize the array of characters that we will use for the
    // graph.  Using the space, an extra copy of the one-bar
    // character, and character 255 (a full black box), we get 10
    // characters in the array.
    const char display_characters[10] = {' ', 0, 0, 1, 2, 3, 4, 5, 6, 255};

    // The variable c will have values from 0 to 9, since
    // calibrated values are in the range of 0 to 1000, and
    // 1000/101 is 9 with integer math.
    char c = display_characters[calibrated_values[i] / 101];

    // Display the bar graph character.
    OrangutanLCD::print(c);
  }
}

void welcomeMessage()
{
  OrangutanLCD::printFromProgramSpace(welcome_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_line2);
  OrangutanBuzzer::playFromProgramSpace(welcome);
  delay(2000);

  OrangutanLCD::clear();
  OrangutanLCD::printFromProgramSpace(welcome1_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome1_line2);
  delay(2000);

  OrangutanLCD::clear();
  OrangutanLCD::printFromProgramSpace(welcome2_line1);
  delay(2000);
}

void displayBatteryVoltage()
{
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    int bat = OrangutanAnalog::readBatteryMillivolts();

    OrangutanLCD::clear();
    OrangutanLCD::print(bat);
    OrangutanLCD::print("mV");
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print("Press B");

    delay(100);
  }
}

void autoCalibrateSensor()
{
  OrangutanLCD::clear();        
  OrangutanLCD::gotoXY(0, 0);
  OrangutanLCD::print("Calibrat");
  
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::print("sensors"); 

  delay(1500);
  
  unsigned int counter;
  for (counter = 0; counter < 80; counter++)
  {
    if (counter < 20 || counter >= 60)
      OrangutanMotors::setSpeeds(40, -40);
    else
      OrangutanMotors::setSpeeds(-40, 40);

    // This function records a set of sensor readings and keeps
    // track of the minimum and maximum values encountered.  The
    // IR_EMITTERS_ON argument means that the IR LEDs will be
    // turned on during the reading, which is usually what you
    // want.
    robot.calibrateLineSensors(IR_EMITTERS_ON);

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
}

void displayCalibratedValues(unsigned int *sensors)
{
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    // Read the sensor values and get the position measurement.
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON, 1);

    // Display the position measurement, which will go from 0
    // (when the leftmost sensor is over the line) to 4000 (when
    // the rightmost sensor is over the line) on the 3pi, along
    // with a bar graph of the sensor readings.  This allows you
    // to make sure the robot is ready to go.
    OrangutanLCD::clear();
    OrangutanLCD::print(position);
    OrangutanLCD::gotoXY(0, 1);
    displayReadings(sensors);

    delay(100);
  }
}

void runRobot(unsigned int *sensors)
{
  //Aqui eu reseto o tempo por causa do tempo de boas vindas e configuração
  startMillisCount = millis();
  delay(10);

  //Para a corrida se tempo foi configurado ou se apertar botão B
  while (StopCar(millis()) | !OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    // Exibir tempo de corrida no display
    displayRaceTime(millis());

    // Get the position of the line.  Note that we *must* provide
    // the "sensors" argument to read_line() here, even though we
    // are not interested in the individual sensor readings.
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON, 1);

    // The "proportional" term should be 0 when we are on the line.
    int proportional = ((int)position) - 2000;

    // Compute the derivative (change) and integral (sum) of the position.
    int derivative = proportional - last_proportional;
    integral += proportional;

    // Remember the last position.
    last_proportional = proportional;

    // Compute the difference between the two motor power settings,
    // m1 - m2.  If this is a positive number the robot will turn
    // to the right.  If it is a negative number, the robot will
    // turn to the left, and the magnitude of the number determines
    // the sharpness of the turn.
    // This PID Default
    // int power_difference = proportional/20 + integral/10000 + derivative*3/2;

    // New PID for the tournament
    int power_difference = proportional / 3 + integral / 50000 + derivative * 4 / 1;

    // Compute the actual motor settings.  We never set either motor
    // to a negative value.
    const int max = speed;
    if (power_difference > max)
      power_difference = max;
    if (power_difference < -max)
      power_difference = -max;

    if (power_difference < 0)
      OrangutanMotors::setSpeeds(max + power_difference, max);
    else
      OrangutanMotors::setSpeeds(max, max - power_difference);
  }

  OrangutanMotors::setSpeeds(0, 0);
  OrangutanPushbuttons::waitForRelease(BUTTON_B);

  // debounce botão B
  delay(200);
}

// ***** Inicio dos scripts de configuração de Velocidade
void displaySettingSpeed()
{
  OrangutanLCD::clear();
  OrangutanLCD::gotoXY(0, 0);
  OrangutanLCD::print(" Setup");
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::print(" Speed");
  delay(1500);
}

void displaySettingSpeedOption()
{
  OrangutanLCD::clear();
  OrangutanLCD::gotoXY(0, 0);
  OrangutanLCD::print("Down  Up");
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::print("A ");

  if (speed < 100)
  {
    OrangutanLCD::print(" ");
  }

  OrangutanLCD::print(speed);
  OrangutanLCD::gotoXY(7, 1);
  OrangutanLCD::print("C");
}

void displayAndSettingSpeedValue()
{
  if (OrangutanPushbuttons::isPressed(BUTTON_A))
  {
    speed -= 5;
    if (speed < 30)
    {
      speed = 30;
    }
    // debounce botão A
    delay(200);
  }

  if (OrangutanPushbuttons::isPressed(BUTTON_C))
  {
    speed += 5;
    if (speed > 255)
    {
      speed = 255;
    }
    // debounce botão C
    delay(200);
  }
}

void settingSpeed()
{
  displaySettingSpeed();
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    displaySettingSpeedOption();
    displayAndSettingSpeedValue();
  }
}
// ***** Fim dos scripts de configuração de Velocidade

// ***** Inicio dos scripts de configuração de tempo de parada
void displaySettingTimeStop()
{
  OrangutanLCD::clear();
  OrangutanLCD::gotoXY(0, 0);
  OrangutanLCD::print(" Setup");
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::print("TimeStop");
  delay(1500);
}

void displaySettingTimeStopOptions()
{
  OrangutanLCD::clear();
  OrangutanLCD::gotoXY(0, 0);
  OrangutanLCD::print("- time +");
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::print("A ");
  OrangutanLCD::print(timeInSeconds);
  OrangutanLCD::gotoXY(7, 1);
  OrangutanLCD::print("C");
}

void displayAndSettingTimeStopValue()
{
  if (OrangutanPushbuttons::isPressed(BUTTON_A))
  {
    timeInSeconds -= 1;
    if (timeInSeconds < 0)
    {
      timeInSeconds = 0;
    }
    // debounce botão A
    delay(200);
  }

  if (OrangutanPushbuttons::isPressed(BUTTON_C))
  {
    timeInSeconds += 1;
    if (timeInSeconds > 180)
    {
      timeInSeconds = 180;
    }
    // debounce botão C
    delay(200);
  }
}

void settingTimeStop()
{
  displaySettingTimeStop();

  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    displaySettingTimeStopOptions();
    displayAndSettingTimeStopValue();
  }

  delay(500);
  stopCarTimeMillis = convertSecondsInMillis();
}
// ***** Fim dos scripts de configuração de tempo de parada

// ***** Início dos scripts controle do tempo
void displayRaceTime(unsigned long timeProgressRaceMillis)
{
  if (timeProgressRaceMillis > (startMillisCount + 1000))
  {
    OrangutanLCD::clear();
    OrangutanLCD::print(timeProgressRaceMillis - startMillisCount);
  }
}

bool StopCar(unsigned long timeRaceMillis)
{
  //Parar o robô por 20 segundos
  if (stopCarTimeMillis != 0)
  {    
    if (timeRaceMillis > (startMillisCount + stopCarTimeMillis))
    {      
      OrangutanMotors::setSpeeds(0, 0);
      //Tempo de parada obrigatório são 10 segundos
      delay(20000);  
      //Recebe o tempo atual "Reset"
      startMillisCount = millis();    
      return true;
    }
  }
  return false;
}

long convertSecondsInMillis()
{
  return timeInSeconds * 1000;
}
// ***** Fim dos scripts controle do tempo

// Inicializando o robô: 3PI Pololu
// Carregando os caracteres personalizados
// Mensagens de boas vindas
// Exibe a tensão total da bateria
// Calibração automática do array de sensores
// Exibe os valores da calibração em barra gráfica.
void setup()
{
  robot.init(2000);    
  loadCustomCharacters();    
  welcomeMessage();    
  displayBatteryVoltage();    
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  delay(1000); 
  //Configurações de velocidade
  settingSpeed();
  //Configurações de tempo de parada
  settingTimeStop();
  // Calibrar sensores
  autoCalibrateSensor();
  OrangutanMotors::setSpeeds(0, 0);  
  displayCalibratedValues(sensors);
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  OrangutanLCD::clear();  
}

void loop()
{
  // Tocar música de início de corrida    
  OrangutanBuzzer::playFromProgramSpace(go);  
  OrangutanLCD::clear();
  OrangutanLCD::print("Go!");  
  delay(1500);  
  //Robô correndo
  runRobot(sensors); 
  //Configurações de velocidade
  settingSpeed();
  //Configurações de tempo de parada
  settingTimeStop();  
}