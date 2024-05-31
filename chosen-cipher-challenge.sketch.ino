#include <WiFi.h>
#include <stdint.h>
#include <esp_random.h>
#include <BigNumber.h>

#define KEY_LENGTH 128
#define FLAG "flag_here"

// Use these pins to connect to the suppossed LED strips d:
#define RED_PIN 33
#define BLUE_PIN 32

const char* ssid = "MyAccessPoint";
const char* password = "12345678";

WiFiServer server(80);

String header;

unsigned long current_time = millis();
unsigned long previous_time = 0;
const long timeout_time = 2000;
const BigNumber two = 2;
const BigNumber one = 1;

void show_encrypt();
void win();

BigNumber encrypted_flag;
char* encrypted_flag_string;

// Key generation functions, not part of the challenge but its probably
// also breakable
BigNumber private_key = 0;
BigNumber public_key = 0;
BigNumber e = 65537;

BigNumber random_huge_number(int key_length);
BigNumber obtain_prime(int key_length);
int pass_miller_rabin(BigNumber n);
void generate_key();
void encrypt_flag();

// starting the connection to the wifi and initializing RSA key pairs
void setup() {
  Serial.begin(115200); // interface with serial port
  Serial.println("Initializing..");

  BigNumber::begin();
  randomSeed(esp_random());
  generate_key();  
  encrypt_flag();

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Will try for about 10 seconds (20x 500ms)
  int tryDelay = 500;
  int numberOfTries = 20;

  // Wait for the WiFi event
  while (true) {
    Serial.print(".");
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("[WiFi] SSID not found");
        break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        return;
        break;
      case WL_CONNECTION_LOST:
        Serial.println("[WiFi] Connection was lost");
        break;
      case WL_SCAN_COMPLETED:
        Serial.println("[WiFi] Scan is completed");
        break;
      case WL_DISCONNECTED:
        Serial.println("[WiFi] WiFi is disconnected");
        break;
      case WL_CONNECTED:
        Serial.println("[WiFi] WiFi is connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
        server.begin();
        return;
      default:
        Serial.print("[WiFi] WiFi Status: ");
        Serial.println(WiFi.status());
        break;
    }
    delay(tryDelay);

    if (numberOfTries <= 0) {
      Serial.print("[WiFi] Failed to connect to WiFi!");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();
      return;
    }

    numberOfTries--;

  }

}

char* web_app =                "HTTP/1.1 200 OK\n"
                               "Content-type:text/html\n"
                               "Connection: close\n"
                               "\n"
                               "<!DOCTYPE html><html>\n"
                               "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                               "<link rel=\"icon\" href=\"data:,\">\n"
                               // Styling
                               "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n"
                               "body { background-color: black; color: white; margin: 0; padding: 0;\n"
                               "text-decoration: none; font-size: 30px; cursor: pointer;}\n"
                               ".button2 {background-color: #555555;}</style></head>\n"
                               // Actual html
                               "<body><h1>Neo, you are the chosen one</h1>\n"
                               "<h2> I have a secret message and a mechanism to create more using RSA encryption.. </h2>\n"
                               "<h3> enter http://ipaddress/decrypt(cipher) to decrypt a ciphertext </h3>\n"
                               "<h3> enter http://ipaddress/encrypt(message) to encrypt a plaintext , result is a large number </h3>\n"
                               "<h3> Neo, you must uncover this secret message and travel through its portal (uri)\n"
                               " however, the slug agents of the matrix don't let you directly decrypt the secret message </h3>\n"
                               "<h2> You are the chosen one neo I believe in you... </h2>\n"
                               "\n";


void loop() {
  WiFiClient client = server.available(); // Listen to incomings

  if (!client) {
    return;
  }

  current_time = millis();
  previous_time = current_time;

  Serial.println("New Client: ");
  String current_line = "";

  while (client.connected() &&
         current_time - previous_time <= timeout_time) {

    current_time = millis();
    if (!client.available()) {
      continue;
    }

    char c = client.read();
    Serial.write(c);

    header += c;

    if ( c == '\n' ) {
      if ( current_line.length() == 0 ) {
        client.println(web_app);

        if (header.indexOf("GET /decrypt") >= 0) {
          // parse and obtain value in () after decrypt..
          // if the parsed value equals to the encrypted flag, kick it out

          // if the decrypted value equals the plaintext flag, kick it out

          // else, return the value

        } else if (header.indexOf("GET /encrypt") >= 0) {
          client.println(header);

        } else {
          client.println("Secret key is :");
          client.println(encrypted_flag_string);
        }

        break;

      } else {
        current_line = ""; // clear that sucker
      }

    } else if ( c != '\r') {
      current_line += c;
    }

  }

  header = "";
  client.stop();
  Serial.println("Client disconnected");
  Serial.println("");
}

void encrypt_flag() {
  BigNumber flag_to_num = FLAG;
  encrypted_flag = flag_to_num.powMod(e, public_key);
  encrypted_flag_string = encrypted_flag.toString();
}

BigNumber obtain_prime(int key_length) {
  while (1) {
    BigNumber n = random_huge_number(key_length);

    if (pass_miller_rabin(n)) {
      return n;
    }
  }
}



// Find a random number in hopes of it being prime
BigNumber random_huge_number(int key_length) {
  BigNumber low_r = two.pow(key_length - 1) + one;
  BigNumber high_r = two.pow(key_length) - one;

  BigNumber random_number = low_r;
  BigNumber strange_rand = 0;
  while (1) {
    for (int i = 0; i < 5; i++) {
      strange_rand = random(1, INT32_MAX);
      strange_rand *= two.pow(key_length / 2);
      random_number += strange_rand;

    }

    // We need to be within valid range so we can find primes with high probability
    if (random_number > high_r ) {
      random_number = low_r;
      continue;
    }
    break;
  }

  return random_number;
}

// Calculates the likelihood of n being prime
int pass_miller_rabin(BigNumber n) {

  if (n % two == 0) {
    return 0;
  }

  BigNumber s = n - one;

  while ( s % two == 0 ) {
    s /= two;
  }


  // With 20 iterations you are almost guaranteed a prime
  for (int i = 0; i < 20; i++) {
    BigNumber randval = random(1, INT32_MAX);

    BigNumber a = randval % (n - one) + one;
    BigNumber temp = s;
    BigNumber mod = a.powMod(temp, n);
    while (temp != n - one && mod != one && mod != n - one) {
      mod = mod.powMod(2, n);
      temp = temp * two;
    }
    if (mod != n - one && temp % two == 0 ) {
      return 0;
    }

  }

  return 1;
}


BigNumber mod_inv(BigNumber a, BigNumber b) {
  BigNumber y = 0; // ;(
  BigNumber x = 1;
  BigNumber A = a;
  BigNumber M = b;
  BigNumber m0 = M;

  while (A > 1) {
    BigNumber q = A / M;
    BigNumber t = M;

    M = A % M;
    A = t;
    t = y;

    y = x - q * y;
    x = t;
  }

  if (x < 0) {
    x += m0;
  }
  return x;
}

void generate_key() {
  BigNumber q = obtain_prime(KEY_LENGTH);
  BigNumber p = obtain_prime(KEY_LENGTH);

  BigNumber n = q * p;
  BigNumber phi_n = (p - one) * (q - one);


  BigNumber d = mod_inv(e, phi_n);

  public_key = n;
  private_key = d;

  return;
}
