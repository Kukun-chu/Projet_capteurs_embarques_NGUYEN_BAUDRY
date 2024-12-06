# Projet_capteurs_embarqués_Nguyen_Baudry

Ceci est le projet de capteurs embarqués de NGUYEN Canh-tom et BAUDRY Téo. 
Voici le lien vers le github : https://github.com/Kukun-chu/Projet_capteurs_embarques_NGUYEN_BAUDRY

Pour les différents exercices, nous avons crées des fonctions que nous appelons dans le main.cpp.
Pour jouer les différents exercices, il faut donc décommenter la ligne correspondant à l'appel de la fonction pour l'exercice désiré.

## **Récupérer le dépôt**:

**Cloner le dépôt** :
    git clone <lien_du_depot>



## **Commandes de base pour tester le projet**:

**Se placer dans l’environnement virtuel** :

    source <env_name>/bin/activate

**Compiler** :

    mbed compile

**Flasher la carte** :

    sixtron_flash

**Afficher les communications de la cible** :

    minicom -D /dev/ttyUSB0 -b 9600


## **Réalisation des exercices**:
**Pour toute la section exercice, il faut se placer dans la branche 'exos' sauf contre-indication pour l'exercice ping pong** :
```
git checkout exos
```

Suivre les indications pour chaque exercice.

**Blinky** :
Décommenter la ligne appelant la fonction blinking_test() dans le main, voici la fonction blinking_test() : 

```C    
void blinking_test() {
    // Initialise the digital pin LED1 as an output
    #ifdef LED1
        DigitalOut led(LED1);
    #else
        bool led;
    #endif
    while (true) {
        led = 1;          // set LED1 pin to high
        printf("myled = %d \n\r", (uint8_t)led);
        ThisThread::sleep_for(500ms);

        led.write(0);     // set LED1 pin to low
        printf("led = %d \n\r", led.read());
        ThisThread::sleep_for(500ms);
        
        //led = !led;
        //ThisThread::sleep_for(BLINKING_RATE);
    }
}
```

**Button test** :
Décommenter la ligne appelant la fonction button_test() dans le main, voici la fonction button_test() : 

```C    
void button_test() {
    // Initialise the digital pin LED1 as an output
    #ifdef LED1
        DigitalOut led(LED1);
    #else
        bool led;
    #endif

    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        DigitalIn button(BUTTON1);
    #else
        bool button;
    #endif

    while (true) {
        if (button.read() == 1) {
            led = 1;          // set LED1 pin to high
        }
        else{
            led.write(0);     // set LED1 pin to low
        }
        printf("button_state = %d \n\r", button.read());
    }
}
```

**Button IRQ** :
Décommenter la ligne appelant la fonction button_irq() dans le main, voici la fonction button_irq() : 

```C    
void button_irq() {

    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        InterruptIn button(BUTTON1);
    #else
        bool button;
    #endif

    button.rise(&flip_irq);  // attach the address of the flip function to the rising edge
    button.fall(&flip_irq);  // attach the address of the flip function to the rising edge
    while(1) {           // wait around, interrupts will interrupt this!
        ThisThread::sleep_for(250ms);
    }
}
```
Sachant que flip_irq est définit de la manière suivante :
```C    
void flip_irq() {
    led = !led;
}
```

**Timer** :
Décommenter la ligne appelant la fonction timer() dans le main, voici la fonction timer() : 

```C    
void timer() {
    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        InterruptIn button(BUTTON1);
    #else
        bool button;
    #endif

    button.rise(&start_counting);  // attach the address of the flip function to the rising edge
    button.fall(&end_counting);  // attach the address of the flip function to the rising edge
    while(1) {           // wait around, interrupts will interrupt this!
        ThisThread::sleep_for(250ms);
        if(handlerTimer == 1) {
            printf("End time: %llu milliseconds\n", std::chrono::duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count());
            handlerTimer = 0;
        }
    }
}
```
Sachant que start_counting et end_couting sont définit de la manière suivante : 
```C
void start_counting() {
    t.start();
    led = !led;
}

void end_counting() {
    t.stop();
    led = !led;
    handlerTimer = 1;
}
```
**Ticker LED** :
Décommenter la ligne appelant la fonction ticker_func() dans le main, voici la fonction ticker_func() : 

```C    
void ticker_func() {
    ticker.attach(&flip_irq, 1s);
}
```
Sachant que flip_irq est définit de la manière suivante :
```C    
void flip_irq() {
    led = !led;
}
```

**Frequency LED** :
Décommenter la ligne appelant la fonction freq() dans le main, voici la fonction freq() : 

```C    
void freq() {
    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        InterruptIn button(BUTTON1);
    #else
        bool button;
    #endif

    std::chrono::milliseconds freq_rate = 100ms;
    ticker.attach(&flip_irq, freq_rate);
    button.fall(&increment_freq_rate);  // attach the address of the flip function to the rising edge
    while(1) {           // wait around, interrupts will interrupt this!
        ThisThread::sleep_for(250ms);
        if(handlerTimer == 1) {
            freq_rate *= 10;
            handlerTimer = 0;
            ticker.attach(&flip_irq, freq_rate);
        }
    }
}
```
Sachant que flip_irq et increment_freq_rate sont définit de la manière suivante :
```C    
void flip_irq() {
    led = !led;
}
void increment_freq_rate() {
    handlerTimer = 1;
}
```

**Communiquer avec un capteur (P-T-RH)** :
Pour cet exercice, nous avons pu communiquer avec le capteur de température et d'humidité. Attention le capteur de température n'est pas présent sur toutes les cartes PTRH.

Décommenter la boucle while(1) ainsi que la ligne d'attente dans le main.
Pour le capteur de température, décommenter la ligne d'appel à la fonction temperature_sensor_handler()  et pour le capteur d'humidité décommenter la ligne d'appel à la fonction humidity_sensor_handler(). 
```C
ThisThread::sleep_for(1000ms); // Attendre une seconde avant la prochaine lecture
```

Voici la fonction de communication avec le capteur de température.
```C
#define TEMP_SENSOR_ADDR (0x48 << 1) // mbed utilise des adresses 8 bits
#define TEMP_REGISTER 0x00            // Registre de température 

void temperature_sensor_handler() {
    char read_data[2] =  {0};
    char reg_adress[1] = {0x00};
    

    //i2c.write(TEMP_SENSOR_ADDR, &reg_adress, 1);
    if (i2c.write(TEMP_SENSOR_ADDR, reg_adress, 1) != 0) {
        printf("Erreur lors de l'envoi de la commande au capteur.\n");
        //return;
    }

    // Lire les données de température (2 octets)
    // i2c.read(TEMP_SENSOR_ADDR, read_data, 2);
    if (i2c.read(TEMP_SENSOR_ADDR, read_data, 2) != 0) {
        printf("Erreur lors de la lecture de la temperature.\n");
        //return;
    }

    int temp_raw = (read_data[0] << 8) | read_data[1];

    // Simuler deux décimales en multipliant par 100 et en affichant sous forme d'entier
    int temperature = temp_raw * 100 / 128; // Multiplier par 100 pour simuler 2 décimales

    // Affichage de la température sur le terminal avec printf
    printf("Temperature : %d.%02d deg Celsius\n", temperature / 100, temperature % 100);
}
```

Voici la fonction de communication avec le capteur d'humidité.
```C
#define HUM_SENSOR_ADDR (0x40 <<1) // mbed utilise des adresses 8 bits
#define HUM_REGISTER 0xE5            // Registre de l'humidite

void humidity_sensor_handler() {
    //char cmd[1];
    char read_hum_data[2] =  {0};
    char reg_adress[1] = {0x00};
    reg_adress[0] = HUM_REGISTER;

    //i2c.write(HUM_SENSOR_ADDR, &reg_adress, 1);
    if (i2c.write(HUM_SENSOR_ADDR, reg_adress, 1) != 0) {
        printf("Erreur lors de l'envoi de la commande au capteur.\n");
        //return;
    }

    // Ajouter un délai pour permettre la conversion (selon la datasheet)
    ThisThread::sleep_for(10ms);

    // Lire les données de température (2 octets)
    // i2c.read(HUM_SENSOR_ADDR, read_data, 2);
    if (i2c.read(HUM_SENSOR_ADDR, read_hum_data, 2) != 0) {
        printf("Erreur lors de la lecture de l'humidite.\n");
        //return;
    }

    int hum_raw = (read_hum_data[0] << 8) | read_hum_data[1] & 0xFFFC; // Ignore les bits de status


    // Conversion en pourcentage avec 2 décimales (sans float)
    int humidity = -600 + (12500 * hum_raw) / 65536;
    
    // Affichage de la température sur le terminal avec printf
    printf("Humidite : %d.%02d \n", humidity / 100, humidity % 100);
}

```

**Ping Pong** :
Pour l'exercice Ping pong, la procédure est différente. Il faut changer de branche sur git, et se positionner sur la branche 'exo_ping_pong'.

    git checkout exo_ping_pong

## **Projet LoRa**:
Pour tester ce projet, il suffit d'utiliser la dernière version de la branche main. Il devrait apparaître sur le ThingsBoard un graphique montrant l'humidité 

## **Liens ressources**:

• Ressource sur le Zest_Sensor_P_T_RH :

https://6tron.io/zest/zest_sensor_p_t_rh_1_0_0

• Ressource sur l'API :

https://os.mbed.com/docs/mbed-os/v6.16/apis/index.html

## **Zest_Sensor_P_T_RH** :

• A humidity sensor HTU21DF measure air humidity between 0 ~ 100 %HR.

• A temperature sensor AS6212-AWLT-S provides a 16-bit resolution over the full temperature range from -40°C to +125°C and integrated alarm functionality enables micro-controller wake-up calls at defined temperature thresholds.

• A pressure sensor 2SMPB-02E measure the pressure of air between 30kPa and 110kPa and integrates a temperature compensation circuit to ensure accurate absolute pressure measurements.

Power Supply : 3.3 to 12V from stack, 3.3V internal

Connectivity : 80-pin 6TRON header