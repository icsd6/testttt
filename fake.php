<?php
$server_ip = '135.125.166.224'; // Adresa IP a serverului
$server_port = 27015;           // Portul serverului

$connect_packet = "\xFF\xFF\xFF\xFFinfo\n"; // Pachet de conectare simplificat
$keep_alive_packet = "\xFF\xFF\xFF\xFFkeepalive\n"; // Pachet "keep-alive"

// Creare socket UDP
$socket = stream_socket_client("udp://$server_ip:$server_port", $errno, $errstr, 30);
if (!$socket) {
    die("Eroare la crearea socketului: $errstr ($errno)\n");
}

function sendPacket($socket, $data) {
    $bytes_sent = fwrite($socket, $data);
    if ($bytes_sent === false) {
        echo "Eroare la trimiterea pachetului.\n";
    } else {
        echo "Pachet trimis: " . bin2hex($data) . "\n";
    }
}

sendPacket($socket, $connect_packet);

// Primire răspunsuri
stream_set_timeout($socket, 5);
$response = fread($socket, 4096);
if ($response === false) {
    echo "Eroare la primirea răspunsului.\n";
} else {
    echo "Răspuns primit: " . bin2hex($response) . "\n";
}

// Trimitere pachet "keep-alive"
while (true) {
    sendPacket($socket, $keep_alive_packet);
    sleep(10); // Pauză de 10 secunde
}

fclose($socket);
?>
