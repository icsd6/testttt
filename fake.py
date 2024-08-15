import socket
import time

# Detalii server
SERVER_IP = '135.125.166.224'  # Adresa IP a serverului tău
SERVER_PORT = 27015       # Portul serverului tău

# Funcție pentru a trimite un pachet către server
def send_packet(sock, data):
    try:
        sock.sendto(data, (SERVER_IP, SERVER_PORT))
    except Exception as e:
        print(f"Eroare la trimiterea pachetului: {e}")

# Funcție pentru a crea un jucător fals
def create_fake_player():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Conectare la server (simulare)
    player_name = "FakePlayer_" + str(int(time.time()) % 1000)
    connection_packet = f"\xFF\xFF\xFF\xFFconnect {player_name}\n"
    send_packet(sock, connection_packet.encode())

    # Menținerea conexiunii cu serverul
    try:
        while True:
            keep_alive_packet = f"\xFF\xFF\xFF\xFFkeepalive {player_name}\n"
            send_packet(sock, keep_alive_packet.encode())
            time.sleep(5)
    except KeyboardInterrupt:
        print("Jucător fals deconectat.")
    finally:
        sock.close()

if __name__ == "__main__":
    create_fake_player()
