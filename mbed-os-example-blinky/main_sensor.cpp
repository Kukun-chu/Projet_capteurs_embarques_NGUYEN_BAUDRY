#include <stdio.h>
#include "mbed.h"
#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"
#include "lora_radio_helper.h"

using namespace events;

// Configuration LoRaWAN
#define TX_TIMER                        10000  // 10 secondes
#define CONFIRMED_MSG_RETRY_COUNTER     3
#define LORA_APP_PORT                   2      // Choisir un port LoRaWAN pour ThingsBoard

// Capteur d'humidité
#define HUM_SENSOR_ADDR 0x40           // Adresse I2C du capteur d'humidité
#define HUM_REGISTER      0x00         // Registre pour lire l'humidité

uint8_t tx_buffer[30];
uint8_t rx_buffer[30];

// Variables globales
char *payload = "{\"humidity\": %d}";  // Format de message pour envoyer l'humidité

// Variables LoRaWAN
static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS * EVENTS_EVENT_SIZE);
static LoRaWANInterface lorawan(radio);

// Définition des événements LoRaWAN
static void lora_event_handler(lorawan_event_t event);

// Fonction pour lire et envoyer l'humidité via LoRaWAN
void humidity_sensor_handler() {
    char read_hum_data[2] = {0};
    char reg_adress[1] = {HUM_REGISTER};

    // Lire les données du capteur d'humidité
    if (i2c.write(HUM_SENSOR_ADDR, reg_adress, 1) != 0) {
        printf("Erreur lors de l'envoi de la commande au capteur d'humidité.\n");
        return;
    }

    // Attendre la conversion du capteur
    ThisThread::sleep_for(10ms);

    // Lire les données d'humidité
    if (i2c.read(HUM_SENSOR_ADDR, read_hum_data, 2) != 0) {
        printf("Erreur lors de la lecture de l'humidité.\n");
        return;
    }

    // Conversion des données brutes en pourcentage d'humidité
    int hum_raw = (read_hum_data[0] << 8) | (read_hum_data[1] & 0xFFFC);
    int humidity = -600 + (12500 * hum_raw) / 65536;

    // Formater les données d'humidité pour l'envoi
    char msg[64];
    snprintf(msg, sizeof(msg), payload, humidity);

    // Préparer les données pour l'envoi LoRaWAN
    memcpy(tx_buffer, msg, strlen(msg));
    int packet_len = strlen(msg);

    // Envoyer le message via LoRaWAN
    int retcode = lorawan.send(LORA_APP_PORT, tx_buffer, packet_len, MSG_UNCONFIRMED_FLAG);
    if (retcode < 0) {
        printf("Erreur d'envoi, code %d\n", retcode);
    } else {
        printf("Envoi réussi, %d octets envoyés\n", retcode);
    }
}

// Gestion des événements LoRaWAN
static void lora_event_handler(lorawan_event_t event) {
    switch (event) {
        case CONNECTED:
            printf("\r\nConnexion LoRaWAN réussie!\n");
            ev_queue.call_every(TX_TIMER, humidity_sensor_handler);  // Envoi toutes les 10 secondes
            break;
        case DISCONNECTED:
            ev_queue.break_dispatch();
            printf("\r\nDéconnexion réussie!\n");
            break;
        case TX_DONE:
            printf("\r\nMessage envoyé avec succès!\n");
            break;
        case TX_TIMEOUT:
        case TX_ERROR:
        case TX_CRYPTO_ERROR:
        case TX_SCHEDULING_ERROR:
            printf("\r\nErreur lors de l'envoi, code = %d\n", event);
            break;
        case RX_DONE:
            printf("\r\nMessage reçu!\n");
            break;
        case RX_TIMEOUT:
        case RX_ERROR:
            printf("\r\nErreur de réception, code = %d\n", event);
            break;
        case JOIN_FAILURE:
            printf("\r\nÉchec de la connexion OTAA - Vérifiez les clés!\n");
            break;
        case UPLINK_REQUIRED:
            printf("\r\nLe serveur de réseau demande un message uplink\n");
            humidity_sensor_handler();  // Essayer d'envoyer un message
            break;
        default:
            MBED_ASSERT("Événement inconnu");
    }
}

int main(void) {
    lorawan_status_t retcode;

    // Initialisation du stack LoRaWAN
    if (lorawan.initialize(&ev_queue) != LORAWAN_STATUS_OK) {
        printf("\r\nInitialisation LoRaWAN échouée!\n");
        return -1;
    }

    printf("\r\nStack LoRaWAN initialisé\n");

    // Définir les callbacks d'événements LoRaWAN
    lorawan_app_callbacks_t callbacks;
    callbacks.events = mbed::callback(lora_event_handler);
    lorawan.add_app_callbacks(&callbacks);

    // Configurer les paramètres de réessai pour les messages confirmés
    if (lorawan.set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER) != LORAWAN_STATUS_OK) {
        printf("\r\nÉchec de la configuration des réessais!\n");
        return -1;
    }

    // Activer l'Adaptive Data Rate (ADR)
    if (lorawan.enable_adaptive_datarate() != LORAWAN_STATUS_OK) {
        printf("\r\nÉchec de l'activation de l'ADR!\n");
        return -1;
    }

    // Connexion au réseau LoRaWAN
    retcode = lorawan.connect();
    if (retcode != LORAWAN_STATUS_OK && retcode != LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
        printf("\r\nÉchec de la connexion, code = %d\n", retcode);
        return -1;
    }

    printf("\r\nConnexion en cours...\n");

    // Lancer l'événement queue pour dispatcher en continu
    ev_queue.dispatch_forever();

    return 0;
}
