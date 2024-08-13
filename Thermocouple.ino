// Cold junction compensation calculations
// Raj Dave

  float temperature[51];                  // creates a temperature array of 50 elements

  // below is the voltage-temperature look up table for k type thermocouple
  // float voltage[] = {                     // set up a voltage array of 51 elements
  //   0, 0.039, 0.079, 0.119, 0.158, 0.198, 0.238, 0.277, 0.317, 0.357, 0.397,
  //   0.437, 0.477, 0.517, 0.557, 0.597, 0.637, 0.677, 0.718, 0.758, 0.798,
  //   0.838, 0.879, 0.919, 0.960, 1.000, 1.041, 1.081, 1.112, 1.163, 1.203, 
  //   1.244, 1.285, 1.326, 1.366, 1.407, 1.448, 1.489, 1.530, 1.571, 1.612,
  //   1.653, 1.694, 1.735, 1.776, 1.817, 1.858, 1.899, 1.941, 1.982, 2.023
  // };             

  // below is the voltage-temperature look up table for t type thermocouple
  float voltage[] = {                 // set up a voltage array of 51 elements
    0, 0.039, 0.078, 0.117, 0.156, 0.195, 0.234, 0.273, 0.312, 0.352, 0.391,
    0.431, 0.470, 0.510, 0.549, 0.589, 0.629, 0.669, 0.709, 0.749, 0.790, 
    0.830, 0.870, 0.911, 0.951, 0.992, 1.033, 1.074, 1.114, 1.155, 1.196,
    1.238, 1.279, 1.320, 1.362, 1.403, 1.445, 1.486, 1.528, 1.570, 1.612,
    1.654, 1.696, 1.738, 1.780, 1.823, 1.865, 1.908, 1.950, 1.993, 2.036
  };


  float V_tj = 0;        

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  for (int i = 0; i <= 50; i++) {
    temperature[i] = {i};         // sets values from 0 to 50
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  float T_cj = 0;         // initialize the cold junction temperature

  float T_tj = 0;

  int pole = 0;

  int gain_arduino = 2471;            // arduino gain

  int gain_RTD = 10.2;                  // RTD gain


  if (Serial.available() > 0) {
    pole = Serial.parseFloat();
    
    // read the input on analog pin 0:
    // float T_tj = 0;

    int sensorValue = analogRead(A0);                 // Reads the input voltage at pin A0 to convert to temperature

    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage_gain_arduino = 1; // sensorValue * (5.0 / 1023.0);         // this would give the voltage with gain
    float voltage_arduino = voltage_gain_arduino / gain_arduino;               // voltage from arduino without gain in volts
    voltage_arduino = voltage_arduino * 1000;                          // voltage from arduino without gain in milliVolts
    
    // print out the value you read:
    Serial.print("The voltage read by arduino without gain: ");
    Serial.print(voltage_arduino, 4);
    Serial.println(" mV");

    int RTD_sensorValue = analogRead(A1);               //Serial.parseFloat();               Reads the input voltage from pin A1 which is connected to RTD

    float voltage_gain_RTD = 3; //RTD_sensorValue * (5.0 / 1023.0);              // convert the sensor value intot the voltage with gain from RTD
    float voltage_RTD = voltage_gain_RTD / gain_RTD;                        // RTD voltage without gain
                                           
    T_cj = voltage_RTD * 100;                                               // cold junction temperature:  10 mV/oC or 0.01 V/oC
    // float V_tj = 0;

    Serial.print("Cold Junction Temperature = ");
    Serial.print(T_cj, 4);
    Serial.print(" C");

    int index = 0;

    // Serial.println(voltage[5], 4);

    // read thermocouple voltage from analog pin A0
    // Divide the voltage by the gain
    // that would give you the voltage in mV
    // subtract the cold junction 
    // T_cj = 25;              // C, cold junction temperature (room temperature)
    
    // find the T_cj value in the temperature array and find the corresponding voltage

    
    
    for (int i = 0; i <= 50; i++) {

      // Serial.println(temperature[i]);
      if (T_cj == temperature[i]) {
        Serial.println();
        Serial.print("No decimal");
        index = i;

        float V_cj = voltage[index];

        V_tj = voltage_arduino + V_cj;

        
        Serial.println();
        Serial.print("Cold Junction Voltage = ");
        Serial.print(V_cj, 5);
        Serial.print(" mV");

        Serial.println();
        Serial.print("Thermocouple Junction Voltage = ");
        Serial.print(V_tj, 5);
        Serial.print(" mV");

        // Serial.println(i);

      }

      else if (T_cj > temperature[i] && T_cj < temperature[i+1]) {
        Serial.println();
        Serial.print("There is decimal");

        // if there is decimal then interpolate
        index = i;

      // interpolation. check it

      float V_cj = ( ((T_cj - temperature[i]) / (temperature[i + 1] - temperature[i])) * (voltage[i + 1] - voltage[i]) ) + voltage[i];

      // float V_cj = (voltage[i] + voltage[i + 1]) / 2;

        V_tj = voltage_arduino + V_cj;

        Serial.println();
        Serial.print("Cold Junction Voltage = ");
        Serial.print(V_cj, 5);
        Serial.print(" mV");

        Serial.println();
        Serial.print("Thermocouple Junction Voltage = ");
        Serial.print(V_tj, 5);
        Serial.println(" mV");
        // Serial.println(voltage[5]);

          // if (V_tj == voltage[i]) {
          // Serial.print("I am here");
          // T_tj = temperature[i];
          // } 

          // else if (V_tj >= voltage[i] && V_tj <= voltage[i]) {
          // Serial.println();
          // Serial.print("in the last");
          // Serial.print(i);
          // }

        //Serial.println(i);
        // delay(1000);
      }
    }

    for (int i = 0; i <= 50; i++) {

      //Serial.println(V_tj, 3);
      // float v_diff = V_tj - voltage[45];
      // Serial.println(v_diff);

      //Serial.println(voltage[45], 3);

      if (V_tj == voltage[i]) {
          Serial.print("I am here");
          T_tj = temperature[i];
          Serial.println();
          Serial.print("Thermocouple Junction temperature = ");
          Serial.print(T_tj);
          Serial.print(" C");
      }

      else if (V_tj > (voltage[i]) && V_tj < (voltage[i + 1])) {
        // Serial.println();
        // Serial.println(V_tj, 5);
        // Serial.println(V_tj - voltage[i + 1], 5);//"in the last");


        // this is not interpolation yet. this is just the average between two which needs to be changed so work on that.
        T_tj = ( ((V_tj - voltage[i]) / (voltage[i + 1] - voltage[i])) * (temperature[i + 1] - temperature[i]) ) + temperature[i];

        // T_tj = (temperature[i] + temperature[i + 1]) / 2;
        Serial.println();
        Serial.print("Thermocouple Junction temperature = ");
        Serial.print(T_tj);
        Serial.print(" C");
      }
    }

  }

}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("Cold Junction Temperature Not Initialized");  // send an initial string
    delay(300);
  }
}
