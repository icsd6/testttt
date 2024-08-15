import socket

# Detalii server
SERVER_IP = 'furien.devilz.ro'  # Adresa IP a serverului tău
SERVER_PORT = 27015            # Portul serverului tău

def send_packet(sock, data):
    """Funcție pentru a trimite pachete către server și a gestiona erorile."""
    try:
        sock.sendto(data, (SERVER_IP, SERVER_PORT))
        print(f"Pachet trimis către {SERVER_IP}:{SERVER_PORT}")
    except Exception as e:
        print(f"Eroare la trimiterea pachetului: {e}")

def receive_response(sock):
    """Funcție pentru a primi și afișa răspunsuri de la server."""
    try:
        sock.settimeout(5)  # Setează un timeout pentru așteptarea răspunsului
        response, addr = sock.recvfrom(4096)  # Așteaptă un răspuns de la server
        print(f"Răspuns primit de la {addr}: {response.decode(errors='ignore')}")
    except socket.timeout:
        print("Nu a fost primit niciun răspuns de la server.")
    except Exception as e:
        print(f"Eroare la primirea răspunsului: {e}")

def create_fake_player():
    """Funcție pentru a crea un jucător fals și a gestiona conexiunea."""
    try:
        # Creare socket UDP
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        print("Socket UDP creat cu succes.")
    except Exception as e:
        print(f"Eroare la crearea socketului: {e}")
        return
    
    # Pachet standard pentru solicitarea informațiilor serverului
    info_request_packet = b"\xFF\xFF\xFF\xFFinfo\n"  # Pachet de test simplificat
    print(f"Încercare de solicitare a informațiilor de la server...")
    send_packet(sock, info_request_packet)
    
    # Așteptare și afișare răspuns de la server
    receive_response(sock)
    
    # Închiderea socket-ului
    try:
        sock.close()
        print("Socket UDP închis.")
    except Exception as e:
        print(f"Eroare la închiderea socketului: {e}")

if __name__ == "__main__":
    create_fake_player()
