import socket
import time
import struct

# Configurarea adresei serverului CS 1.6
SERVER_IP = '135.125.166.224'  # Înlocuiește cu IP-ul serverului tău
SERVER_PORT = 27015       # Portul serverului CS 1.6

# Definește pachetele de conectare și "keep-alive" în format binar
CONNECT_PACKET = struct.pack('!4s', b'info')
KEEP_ALIVE_PACKET = struct.pack('!4s', b'keepalive')

def create_socket():
    """Crează un socket UDP și se conectează la server."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return sock

def send_packet(sock, packet):
    """Trimite un pachet UDP către server."""
    try:
        sock.sendto(packet, (SERVER_IP, SERVER_PORT))
        print(f'Pachet trimis: {packet.hex()}')
    except Exception as e:
        print(f'Eroare la trimiterea pachetului: {e}')

def main():
    sock = create_socket()
    print(f'Simulez jucători pe server {SERVER_IP}:{SERVER_PORT}')
    
    # Trimite pachet de conectare
    send_packet(sock, CONNECT_PACKET)

    # Așteaptă răspunsul serverului
    sock.settimeout(5)
    try:
        response, _ = sock.recvfrom(4096)
        print(f'Răspuns primit: {response.hex()}')
    except socket.timeout:
        print('Eroare: Nu a fost primit niciun răspuns.')

    # Trimitere pachete "keep-alive" pentru a menține conexiunea
    while True:
        send_packet(sock, KEEP_ALIVE_PACKET)
        time.sleep(10)  # Pauză de 10 secunde între pachete

if __name__ == '__main__':
    main()
