#include <Adafruit_NeoPixel.h>

#define LEFT_STRAND_PIN 6 // The pin that controls the strand on the left of the desk
#define LEFT_LED_COUNT 32 // Number of leds on the left
#define RIGHT_STRAND_PIN 5 // The pin that controls the strand on the right of the desk
#define RIGHT_LED_COUNT 27// Number of leds on the right
#define TOTAL_LED_COUNT (LEFT_LED_COUNT+RIGHT_LED_COUNT) // The total count of leds. 
#define WRENCH_SWITCH 7 // Pin for the switch to turn the thing on. 

Adafruit_NeoPixel leftStrip = Adafruit_NeoPixel(LEFT_LED_COUNT, LEFT_STRAND_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightStrip = Adafruit_NeoPixel(RIGHT_LED_COUNT, RIGHT_STRAND_PIN, NEO_GRB + NEO_KHZ800);
uint32_t mainBrightness = 0;
int8_t inc = 0;
bool wasOn = false;

struct Dot{
    uint8_t offSetRed = 0;
    uint8_t offSetGreen = 0;
    uint8_t offSetBlue = 0;
    uint8_t brightness = 0;
};

Dot dots[TOTAL_LED_COUNT];

void setup() {
    Serial.begin(115200);
    leftStrip.begin();
    leftStrip.clear();
    rightStrip.begin();
    rightStrip.clear();
    for (int i = 0; i < TOTAL_LED_COUNT; ++i) {
        dots[i].offSetRed = 1;
        dots[i].offSetGreen = 1;
        dots[i].offSetBlue = 2;
    }
    pinMode(WRENCH_SWITCH, INPUT_PULLUP);
}


void loop() {
    if (buttonReadLow(WRENCH_SWITCH) == true) {
        wasOn = true;
        while (mainBrightness < 255*(TOTAL_LED_COUNT)) {
            int i = 0;
            int32_t temp = mainBrightness;
            while (temp > 0) {
                if (temp > 255) {
                    dots[i].brightness = 255;
                } else {
                    dots[i].brightness = mainBrightness%255;
                }
                paintDot(dots[i], i);
                temp -= 255;
                i++;
            }
            showStrips();
            mainBrightness+=20;
            delay(1);
        }
    } else {
        if (wasOn) {
            for (int i = 255; i >= 0; --i) {
                for (int j = 0; j < TOTAL_LED_COUNT; ++j) {
                    dots[j].brightness = i;
                    paintDot(dots[j], j);
                }
                showStrips();
                delay(5);
            }
            mainBrightness  = 0;
        }
        wasOn = false;
    }

}

void paintDot(Dot dot, uint8_t index) {
    if (index <= RIGHT_LED_COUNT + LEFT_LED_COUNT) {
        if (index > RIGHT_LED_COUNT) {
            leftStrip.setPixelColor(index-RIGHT_LED_COUNT, dot.brightness/dot.offSetRed, dot.brightness/dot.offSetGreen, dot.brightness/dot.offSetBlue);
        } else {
            rightStrip.setPixelColor(RIGHT_LED_COUNT - index, dot.brightness/dot.offSetRed, dot.brightness/dot.offSetGreen, dot.brightness/dot.offSetBlue);
        }
    }
}

void showStrips(void) {
    leftStrip.show();
    rightStrip.show();
}

bool buttonReadLow(uint8_t pin) {
    if (digitalRead(pin) == LOW) {
        delay(100);
        if (digitalRead(pin) == LOW) {
            return true;
        }
    }
    return false;
}