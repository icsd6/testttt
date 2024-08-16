import socket
import random
import time

# Funcție pentru a trimite pachete UDP către server
def send_fake_packets(server_ip, server_port, num_packets):
    # Crearea unui socket UDP
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    for _ in range(num_packets):
        # Generarea unui mesaj de test (poate fi personalizat)
        message = b'\x00' * 1024  # Mesaj de 1024 bytes
        
        # Trimiterea mesajului
        sock.sendto(message, (server_ip, server_port))
        print(f"Sent packet to {server_ip}:{server_port}")
        
        # Așteptare între pachete (pentru a simula activitatea reală)
        time.sleep(random.uniform(0.1, 1.0))

# Funcția principală
def main():
    server_ip = '135.125.166.224'  # IP-ul serverului tău
    server_port = 27015       # Portul serverului tău
    num_packets = 1000        # Numărul de pachete de trimis
    
    send_fake_packets(server_ip, server_port, num_packets)

if __name__ == '__main__':
    main()
