/*
 * This is an example to read analog data at high frequency using the I2S peripheral
 * Run a wire between pins 27 & 32
 * The readings from the device will be 12bit (0-4096) 
 */
 
#include <driver/i2s.h>

#define I2S_SAMPLE_RATE 1000 //1.2M
#define ADC_INPUT ADC1_CHANNEL_4 //pin 32

void i2sInit()
{
   i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate =  I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
   };
   i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
   i2s_set_adc_mode(ADC_UNIT_1, ADC_INPUT);
   i2s_adc_enable(I2S_NUM_0);
}

void dmaRead() {
  //I2S
  uint16_t buffer[512];
  size_t bytes_read;

  i2s_read(I2S_NUM_0, &buffer, sizeof(buffer), &bytes_read, 15);

  for(int i = 0; i < 512; i ++){
    Serial.println(buffer[i]&0xFFF);
  }
  
  //Analog
  int unsigned value = analogRead(26); //ADC2_Ch9
  //Serial.println(value);
}

void setup() {
  //Serial
  Serial.begin(115200);

  //PWM
  pinMode(33, OUTPUT);
  ledcSetup(0, 10000, 8);
  ledcAttachPin(33, 0);
  delay(100);
  ledcWrite(0, 128);

  //I2S
  i2sInit();
  
  //analogRead ADC2_Ch9
  pinMode(26, ANALOG);
}

void loop() {
  dmaRead();
  delay(1000);
}
