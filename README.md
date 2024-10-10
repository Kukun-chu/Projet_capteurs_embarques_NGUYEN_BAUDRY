# Projet_capteurs_embarqu-s_Nguyen_Baudry

Ceci est le projet de capteurs embarqués de NGUYEN Canh-tom et BAUDRY Téo :

## **Commandes de base pour le projet**:

**Se placer dans l’environnement virtuel** :

    source <env_name>/bin/activate

**Compiler** :

    mbed compile

**Flasher la carte** :

    sixtron_flash

**Afficher les communications de la cible** :

    minicom -D /dev/ttyUSB0 -b 9600

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

• The graphics below gives an overview of the board features and connections:
![image](https://github.com/user-attachments/assets/22d59d39-acfd-4029-b4fa-516b6a467e33)

• The graphics below shows the pinout on Zest_Sensor_P_T_RH:
![image](https://github.com/user-attachments/assets/e1658355-960a-43b8-adba-904e1168084b)
