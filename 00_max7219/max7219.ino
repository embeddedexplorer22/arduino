#define DECODE_MODE_REG     0x09
#define INTENSITY_REG       0x0A
#define SCAN_LIMIT_REG      0x0B
#define SHUTDOWN_REG        0x0C
#define DISPLAY_TEST_REG    0x0F

class MAX7219 {
  private:
    int CS;
    int CLK;
    int MOSI;
    void transfer(uint8_t * p_data, uint8_t len);
    void write_reg(uint8_t reg, uint8_t value);
  public:
    MAX7219(int CS, int CLK, int MOSI);
    void set_led(uint8_t row_index, uint8_t col_index);
    void set_row(uint8_t row_index);
    void set_col(uint8_t col_index);
    void clear(void);
};

MAX7219::MAX7219(int CS, int CLK, int MOSI) {
  this->CS = CS;
  this->CLK = CLK;
  this->MOSI = MOSI;

  pinMode(this->CS, OUTPUT);
  pinMode(this->CLK, OUTPUT);
  pinMode(this->MOSI, MOSI);

  digitalWrite(this->CS, HIGH);
  digitalWrite(this->CLK, LOW);
  digitalWrite(this->MOSI, LOW);

  write_reg(DISPLAY_TEST_REG, 0);
  write_reg(SCAN_LIMIT_REG, 7);
  write_reg(DECODE_MODE_REG, 0);
  write_reg(SHUTDOWN_REG, 1);
  
  clear();

  
}

void MAX7219::transfer(uint8_t * p_data, uint8_t len) {
  uint8_t mask;

  digitalWrite(CS, LOW);
  delayMicroseconds(1);
  
  for (int i = 0; i < len; i++) {
    mask = 0x80;
    do {
      if (p_data[i] & mask) {
        digitalWrite(MOSI, HIGH);
      } else {
        digitalWrite(MOSI, LOW);
      }

      delayMicroseconds(1);
      digitalWrite(CLK, HIGH);
      delayMicroseconds(1);
      digitalWrite(CLK, LOW);

      mask >>= 1;
    } while (mask != 0);
  }

  digitalWrite(CS, HIGH);
}

void MAX7219::write_reg(uint8_t reg, uint8_t value) {
  uint8_t tx_data[2] = { reg, value };
  transfer(tx_data, 2);
}

void MAX7219::set_led(uint8_t row_index, uint8_t col_index) {
  write_reg(row_index + 1, 0x01 << col_index);
}

void MAX7219::set_row(uint8_t row_index) {
  write_reg(row_index + 1, 0xFF);
}

void MAX7219::set_col(uint8_t col_index) {
  for (int i = 0; i < 8; i++) {
    write_reg(i + 1, 0x01 << col_index);
  }
}

void MAX7219::clear(void) {
  for (int i = 0; i < 8; i++) {
    write_reg(i + 1, 0x00);
  }
}

MAX7219 max7219(10, 11, 12);

void setup() {
  Serial.begin(115200);
  Serial.println("Program started");
  max7219.set_led(7, 7);
  delay(2000);
 
}

void loop() {
  Serial.println("Controlling rows");
  for (int i = 0; i < 8; i++) {
    max7219.clear();
    max7219.set_row(i);
    delay(1000);
  }

  Serial.println("Controlling columns");
  for (int i = 0; i < 8; i++) {
    max7219.clear();
    max7219.set_col(i);
    delay(1000);
  }
}
