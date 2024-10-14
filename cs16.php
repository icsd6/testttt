<?php

// IP și portul serverului CS 1.6
$server_ip = '162.19.172.145'; // Înlocuiește cu IP-ul serverului tău
$server_port = 27015; // Portul serverului

// Numele jucătorilor falși
$fake_players = array(
    "Player1",
    "Player2",
    "Player3",
    "Player4",
    "Player5"
);

// Crearea socket-ului UDP
$sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
if (!$sock) {
    die("Eroare la crearea socket-ului");
}

// Mesajul pe care îl trimitem pentru a simula jucătorii falși
$message = "\xFF\xFF\xFF\xFF\x54Source Engine Query";

// Trimitem cererea de ping la server pentru fiecare jucător fals
foreach ($fake_players as $player) {
    $data = $message . $player;
    
    // Trimiterea datelor către server
    socket_sendto($sock, $data, strlen($data), 0, $server_ip, $server_port);
    echo "Am trimis date pentru $player la $server_ip:$server_port\n";

    // Poți ajusta sleep-ul pentru a simula conectări cu un interval de timp
    sleep(1);
}

// Închidem socket-ul după ce am terminat
socket_close($sock);

?>
