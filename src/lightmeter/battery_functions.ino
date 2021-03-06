//// Returns actual value of Vcc (x 100)
//int getBandgap(void) {
//#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
//  // For mega boards
//  const long InternalReferenceVoltage = 1115L;  // Adjust this value to your boards specific internal BG voltage x1000
//  // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc reference
//  // MUX4 MUX3 MUX2 MUX1 MUX0  --> 11110 1.1V (VBG)         -Selects channel 30, bandgap voltage, to measure
//  ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (0 << MUX5) | (1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
//
//#else
//  // For 168/328 boards
//  const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
//  // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
//  // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
//  ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
//
//#endif
//  delay(50);  // Let mux settle a little to get a more stable A/D conversion
//  // Start a conversion
//  ADCSRA |= _BV( ADSC );
//  // Wait for it to complete
//  while ( ( (ADCSRA & (1 << ADSC)) != 0 ) );
//  // Scale the value
//  int results = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L; // calculates for straight line value
//
//  return results;
//}

int readVcc() {
  int result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

uint8_t readBatteryLevel() {
  // first get the current vcc value im mV
  int vcc = readVcc();
  // then read the A0 (0-1023)
  unsigned int adcValue = analogRead(0);
  // calculate the actual voltage on the A0, but in mV/10, so it will fit in the uint8_t
  return (adcValue / 1024.0) * vcc / 10;
}
