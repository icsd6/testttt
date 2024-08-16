import socket
import time

# Configurații
SERVER_IP = '135.125.166.224'  # IP-ul serverului CS 1.6
SERVER_PORT = 27015       # Portul serverului CS 1.6

# SteamID-uri (Exemplu, nu sunt reale)
STEAM_IDS = [
    "STEAM_2:1:380283877",
    "STEAM_0:1:14912173",
    "STEAM_0:1:14912573"
]

def create_fake_player(steam_id):
    # Crează un pachet de date cu SteamID-ul
    # Nota: Acesta este un exemplu și poate să nu fie compatibil cu protocolul real
    data = f"FakePlayer:{steam_id}"
    return data.encode('utf-8')

def send_fake_player_to_server():
    # Crează socket
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        for steam_id in STEAM_IDS:
            fake_player_data = create_fake_player(steam_id)
            sock.sendto(fake_player_data, (SERVER_IP, SERVER_PORT))
            print(f"Sent fake player with SteamID: {steam_id}")
            time.sleep(1)  # Așteaptă 1 secundă între trimiteri

if __name__ == "__main__":
    send_fake_player_to_server()
