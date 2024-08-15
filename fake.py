import socket
import time

# Detalii server
SERVER_IP = '127.0.0.1'  # Adresa IP a serverului tău
SERVER_PORT = 27015       # Portul serverului tău

def send_packet(sock, data):
    """Funcție pentru a trimite pachete către server și a gestiona erorile."""
    try:
        sock.sendto(data, (SERVER_IP, SERVER_PORT))
        print(f"Pachet trimis către {SERVER_IP}:{SERVER_PORT}")
    except Exception as e:
        print(f"Eroare la trimiterea pachetului: {e}")

def create_fake_player():
    """Funcție pentru a crea un jucător fals și a gestiona conexiunea."""
    try:
        # Creare socket UDP
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        print("Socket UDP creat cu succes.")
    except Exception as e:
        print(f"Eroare la crearea socketului: {e}")
        return
    
    # Conectare la server (simulare)
    player_name = "FakePlayer_" + str(int(time.time()) % 1000)
    connection_packet = f"\xFF\xFF\xFF\xFFconnect {player_name}\n"
    
    # Trimitere pachet de conectare
    print(f"Încercare de conectare a jucătorului {player_name} la server...")
    send_packet(sock, connection_packet.encode())
    
    # Menținerea conexiunii cu serverul
    try:
        while True:
            # Trimitere pachet "keep-alive"
            keep_alive_packet = f"\xFF\xFF\xFF\xFFkeepalive {player_name}\n"
            send_packet(sock, keep_alive_packet.encode())
            
            # Pauză pentru a simula activitatea jucătorului
            time.sleep(5)
    except KeyboardInterrupt:
        print("Deconectare jucător fals.")
    except Exception as e:
        print(f"Eroare în timpul menținerii conexiunii: {e}")
    finally:
        try:
            sock.close()
            print("Socket UDP închis.")
        except Exception as e:
            print(f"Eroare la închiderea socketului: {e}")

if __name__ == "__main__":
    create_fake_player()
